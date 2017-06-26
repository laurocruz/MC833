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

#define LISTEN_PORT 12345
#define MAX_PENDING 5 
#define MAX_LINE 256


void close_connection(int s);
void handle_new_connection(int s);
int handle_data(int s);

int main(int argc, char * argv[]) {
    struct sockaddr_in socket_address;
    unsigned int len;
    int s, new_s;
    int l_port;
    int pid;
    int i;

    printf("%d concurrent connections supported\n", FD_SETSIZE-1);

    int clients[FD_SETSIZE-1] = {0};
    int max_fd;

    fd_set sockets;

    if (argc == 2)
        l_port = atoi(argv[1]);
    else l_port = LISTEN_PORT;

    /* criação da estrutura de dados de endereço */
    bzero((char *)&socket_address, sizeof(socket_address));

    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
    socket_address.sin_port = htons(l_port);

    /* criação de socket passivo */
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("ERROR: Unable to create socket");
        exit(errno);
    }

    /* Associar socket ao descritor */
    if (bind (s, (struct sockaddr *)&socket_address, sizeof(socket_address))) {
        perror("ERROR: Unable to bind socket");
        exit(errno);
    }

    /* Criar escuta do socket para aceitar conexões */
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
                close_connection(new_s);
            }

            /* Chama a função que lida com a nova conexão */
            handle_new_connection(new_s);

        } 

        // procurar sockets prontos pra leitura
        for (i = 0; i < FD_SETSIZE-1; i++) {
            if (clients[i] > 0 && FD_ISSET(clients[i], &sockets)) {
                // lidar com dados, e remover socket se necessário
                if (handle_data(clients[i]) == 0) {
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

void close_connection(int s) {
    /* Fecha socket da conexão */
    if (close (s) == -1) {
        perror("ERROR: unable to close client socket");
        exit(errno);
    }
}

void handle_new_connection(int s) {
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

int handle_data(int s) {
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
        close_connection(s);
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
