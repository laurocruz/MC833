#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <omp.h>

#include "server.h"

static inline int set_socket(struct sockaddr_in * socket_address, int * s, int port, int type) {
    /* criação da estrutura de dados de endereço */
    bzero((char *)socket_address, sizeof(*socket_address));

    *socket_address.sin_family = AF_INET;
    *socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
    *socket_address.sin_port = htons(port);

    /* criação de socket passivo */
    if ((*s = socket(AF_INET, type, 0)) == -1) {
        perror("ERROR: Unable to create socket");
        exit(errno);
    }

    /* Associar socket ao descritor */
    if (bind (*s, (struct sockaddr *)socket_address, sizeof(*socket_address))) {
        perror("ERROR: Unable to bind socket");
        exit(errno);
    }

    return 0;
}

static inline void tcp_close_connection(int s) {
    /* Fecha socket da conexão */
    if (close (s) == -1) {
        perror("ERROR: unable to close client socket");
        exit(errno);
    }
}

static inline void tcp_handle_new_connection(int s) {
    struct sockaddr_in client_address;
    socklen_t client_socklen = sizeof(client_address);
    char * ip;
    /* Obtem o IP e Porta do cliente */
    if (getpeername(s, (struct sockaddr *) &client_address, &client_socklen) == 0) {
        printf("CLIENT CONNECTED\n");
        ip = inet_ntoa(client_address.sin_addr);
        printf("  IP: %s\n", ip);
        printf("PORT: %d\n\n", ntohs(client_address.sin_port));
    } else {
        perror("ERROR: Could not resolve remote port and ip values\n");
        exit(errno);
    }
}

static inline int tcp_handle_data(int s, void (*app)(char *, char*)) {
    struct sockaddr_in client_address;
    socklen_t client_socklen = sizeof(client_address);
    char * ip;
    char buf[MAX_LINE] = {0};
    int has_data;

    /* Recebe dado do cliente */
    if ((has_data = recv(s, buf, MAX_LINE, 0)) == -1) {
        perror("ERROR: unable to receive data");
        exit(errno);
    }

    if (!has_data) {
        tcp_close_connection(s);
        return 0;
    }

    if (getpeername(s, (struct sockaddr *) &client_address, &client_socklen) == 0) {
        ip = inet_ntoa(client_address.sin_addr);
    } else {
        perror("ERROR: Could not resolve remote ip value\n");
        exit(errno);
    }

    /* Mostra o dado enviado */
    printf("From %s:\n%s\n", ip, buf);

    /* Envia eco */
    if (send(s, buf, MAX_LINE, 0) == -1) {
        perror("ERROR: unable to send data");
        exit(errno);
    }

    return has_data;
}


static inline int tcp_handler(int port, void (*app)(char *, char*)) {
    struct sockaddr_in socket;
    int s, new_s;
    unsigned int len;
    int pid;
    int clients[FD_SETSIZE-1] = {0};
    int max_fd;
    fd_set sockets;

    set_socket(&socket, &s, port, SOCK_STREAM);

    if (listen(s, MAX_PENDING)) {
        perror("ERROR: Unable to invoke listen method");
        exit(errno);
    }

    while (1) {
        FD_ZERO(&sockets);
        FD_SET(s, &sockets);

        max_fd = s;

        for (i = 0; i < FD_SETSIZE-1; i++) {
            if (clients[i] > 0) {
                FD_SET(clients[i], &sockets);
                if (clients[i] > max_fd)
                    max_fd = clients[i];
            }
        }

        if (select(max_fd + 1, &sockets, NULL, NULL, NULL) < 0) {
            if (errno ==  EINTR) {
                continue;
            }
            perror("ERROR: select() failed");
            exit(errno);
        }


        // caso haja nova conexão esperando
        if (FD_ISSET(s, &sockets)) {

            /* aguardar/aceita conexão, receber e imprimir texto na tela, enviar eco */
            if ((new_s = accept(s, (struct sockaddr *)NULL, NULL)) == -1) {
                perror("ERROR: Unable to get client socket");
                exit(errno);
            }

            for (i = 0; i < FD_SETSIZE-1; i++){
                if (clients[i] == 0) {
                    clients[i] = new_s;
                    break;
                }
            }

            if (i == FD_SETSIZE-1) {
                printf("ERROR: maximum clients reached, closing new connection\n");
                tcp_close_connection(new_s);
            }

            /* Chama a função que lida com a nova conexão */
            tcp_handle_new_connection(new_s);

        }

        // procurar sockets prontos pra leitura
        for (i = 0; i < FD_SETSIZE-1; i++) {
            if (clients[i] > 0 && FD_ISSET(clients[i], &sockets)) {
                // lidar com dados, e remover socket se necessário
                if (tcp_handle_data(clients[i], app) == 0) {
                    clients[i] = 0;
                }

            }
        }
    }

    /* Fecha o socket do servidor */
    if (close(s) == -1) {
        perror("ERROR: unable to close socket");
        exit(errno);
    }

    return 0;
}

static inline int udp_handler(int port, void (*app)(char *, char *)) {
    struct sockaddr_in socket, remote;
    int s, has_data;
    char bufin[MAX_LINE], bufout[MAX_LINE];
    socklen_t len = sizeof(remote);

    set_socket(&socket, &s, port, SOCK_DGRAM);

    //printf("Server UDP port: %d\n",ntohs(socket_address.sin_port));

    while (1) {
        memset(bufin, '\0', MAX_LINE);
        memset(bufout, '\0', MAX_LINE);

        has_data = recvfrom(s, bufin, MAX_LINE, 0, (struct sockaddr *)&remote, &len);

        if (!has_data)
            printf("ERROR: Unable to receive data\n");
        else {
            app(bufout, bufin)
            sendto(s, bufout, has_data, 0, (struct sockaddr *) &remote, len);
        }
    }

    if (close(s) == -1) {
        perror("ERROR: unable to close socket");
        exit(errno);
    }

    return 0;
}

int server(int sec_udp_p, int sec_tcp_p, int ec_tcp_p, int ec_udp_p, void (*sec)(char *, char *), void (*ent_con)(char *, char *)) {
    int tid;

    #pragma omp parallel private(tid) shared(sec_udp_p, sec_tcp_p, ec_tcp_p, ec_udp_p, sec, ent_con) num_threads(4)
    {
        tid = omp_get_thread_num();

        // Security - TCP
        if (tid == 0) {
            tcp_handler(sec_tcp_p, sec);

        // Security - UDP
        } else if (tid == 1) {
            udp_handler(sec_udp_p, sec);

        // Entertainment / Confort - TCP
        } else if (tid == 2) {
            tcp_handler(ec_tcp_p, ent_con);

        // Entertainment / Confort - UDP
        } else {
            udp_handler(ec_udp_p, ent_con);
        }
    }

    return 0;
}
