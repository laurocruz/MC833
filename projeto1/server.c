#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

#define LISTEN_PORT 12345
#define MAX_PENDING 5 
#define MAX_LINE 256

int main(int argc, char * argv[]) {
    struct sockaddr_in socket_address;
    char buf[MAX_LINE];
    unsigned int len;
    int s, new_s;
    int l_port;

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
        memset(buf, '\0', MAX_LINE);

        /* aguardar/aceita conexão, receber e imprimir texto na tela, enviar eco */
        if ((new_s = accept(s, (struct sockaddr *)NULL, NULL)) == -1) {
            perror("ERROR: Unable to get client socket");
            exit(errno);
        }

        while (1) {
            int has_data;

            if ((has_data = recv(new_s, buf, MAX_LINE, 0)) == -1) {
                perror("ERROR: unable to receive data");
                exit(errno);
            }

            if (!has_data)
                break;

            printf("%s", buf);

            if (send(new_s, buf, MAX_LINE, 0) == -1) {
                perror("ERROR: unable to send data");
                exit(errno);
            }
        }

        if (close (new_s) == -1) {
            perror("ERROR: unable to close client socket");
            exit(errno);
        }
    }

    if (close(s) == -1) {
        perror("ERROR: unable to close socket");
        exit(errno);
    }

    return 0;
}

