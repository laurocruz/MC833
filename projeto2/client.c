#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "client.h"

static inline int get_socket(struct sockaddr_in * socket_address, int * s, char * hostname, int port, int type) {
    struct hostent * host_address;

    /* tradução de nome para endereço IP */
    host_address = gethostbyname(hostname);

    if (!host_address) {
        printf("ERROR: Unable to resolve hostname\n");
        exit(errno);
    }

    /* criação de socket ativo*/
    if ((*s = socket(AF_INET, type, 0)) == -1) {
        perror("ERROR: Unable to create socket");
        exit(errno);
    }

    /* criação da estrutura de dados de endereço */
    bzero((char *)socket_address, sizeof(*socket_address));

    *socket_address.sin_family = AF_INET;
    *socket_address.sin_addr = *(struct in_addr *)host_address->h_addr_list[0];
    *socket_address.sin_port = htons(port);


    if (type == SOCK_STREAM) {
        /* estabelecimento da conexão */
        if (connect(s, (struct sockaddr *) socket_address, sizeof(*socket_address)) == -1) {
            perror("ERROR: Unable to connect to server");
            exit(errno);
        }
    }

    return 0;
}

static inline void make_msg(char * buf, char app, Car * car) {
    buf[0] = app;

    if (app != SECURITY) {
        buf[1] = '\0';
        return;
    }

    buf[1] = ' '

    int l = (int) log10l(car->ts);
    l++;
    sprintf(buf, "%lu", car->ts);
    buf[l] = ' '; buf += l + 1;

    l = (int) log10(car->speed);
    l++;
    sprintf(buf, "%d", car->speed);
    buf[l] = ' '; buf += l + 1;

    sprintf(buf, "%d", car->dir);
    buf[1] = ' '; buf += 2;

    l = (int) log10(car->size);
    l++;
    sprintf(buf, "%d", car->size);
    buf[l] = ' '; buf += l + 1;

    if (car->pos.x < 0) {
        l = (int) log10(-car->pos.x);
        l += 2;
    } else {
        l = (int) log10(car->pos.x);
        l++;
    }
    sprintf(buf, "%d", car->pos.x);
    buf[l] = ' '; buf += l + 1;

    if (car->pos.y < 0) {
        l = (int) log10(-car->pos.y);
        l += 2;
    } else {
        l = (int) log10(car->pos.y);
        l++;
    }
    sprintf(buf, "%d", car->pos.y);
    buf[l] = ' '; buf += l + 1;
}

int client_tcp(char * hostname, int port, Car * car, char app, int (*app_fun)(char *, Car *)) {
    struct sockaddr_in socket_address;
    //struct sockaddr_in local_address;
    char buf[MAX_LINE];
    int s;

    int get_socket(&socket_address, &s, hostname, port, SOCK_STREAM);

    /* ler e enviar linhas de texto, receber eco */
    memset(&buf, '\0', MAX_LINE);

    while (1) {
        make_msg(buf, app, car);

        /* Envia texto */
        if (send(s, buf, MAX_LINE, 0) == -1) {
            perror("ERROR: unable to send data");
            exit(errno);
        }

        /* Recebe resposta */
        if (recv(s, buf, MAX_LINE, 0) == -1 )  {
            perror("ERROR: unable to receive data");
            exit(errno);
        }

        /******************************************/
        /**************** DO STUFF ****************/
        app_fun(buf, car);
        /******************************************/

        sleep(2);

        memset(&buf, '\0', MAX_LINE);
    }

    /* Fecha o socket do cliente */
    if (close(s) == -1) {
        perror("ERROR: unable to close socket");
        exit(errno);
    }

    return 0;
}

int client_udp(char * hostname, int port, Car * car, char app, int (*app_fun)(char *, Car *)) {
    struct sockaddr_in socket_address;
    //struct sockaddr_in local_address;
    char buf[MAX_LINE];
    int s;

    int get_socket(&socket_address, &s, hostname, port, SOCK_DGRAM);

    /* ler e enviar linhas de texto, receber eco */
    memset(&buf, '\0', MAX_LINE);

    while (1) {
        make_msg(buf, app, car);

        /* Envia mensagem */
        socklen_t len = sizeof(socket_address);
        if (sendto(s, buf, MAX_LINE, 0, (struct sockaddr *) &socket_address, len) == -1) {
            perror("ERROR: unable to send data");
            exit(errno);
        }

        /* Recebe resposta o servidor */
        if (recvfrom(s, buf, MAX_LINE, 0, (struct sockaddr *) &socket_address, &len) == -1 )  {
            perror("ERROR: unable to receive data");
            exit(errno);
        }

        /******************************************/
        /**************** DO STUFF ****************/
        app_fun(buf, car);
        /******************************************/

        sleep(2);

        memset(&buf, '\0', MAX_LINE);
    }

    /* Fecha o socket do cliente */
    if (close(s) == -1) {
        perror("ERROR: unable to close socket");
        exit(errno);
    }

    return 0;
}
