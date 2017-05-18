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

#define LISTEN_PORT 12345
#define MAX_PENDING 5 
#define MAX_LINE 256

void handle_connection(int s);

int main(int argc, char * argv[]) {
    struct sockaddr_in socket_address;
    unsigned int len;
    int s, new_s;
    int l_port;
    int pid;

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
        /* aguardar/aceita conexão, receber e imprimir texto na tela, enviar eco */
        if ((new_s = accept(s, (struct sockaddr *)NULL, NULL)) == -1) {
            perror("ERROR: Unable to get client socket");
            exit(errno);
        }

        /* Cria um novo processo */
        if ((pid = fork()) < 0) {
            perror("ERROR: fork");
            exit(errno);
        } 

        /* Chama a função que lida com a nova conexão */
        if (pid == 0) 
            handle_connection(new_s);
    }

    /* Fecha o socket do servidor */
    if (close(s) == -1) {
        perror("ERROR: unable to close socket");
        exit(errno);
    }

    return 0;
}

void handle_connection(int s) {
    char buf[MAX_LINE];
    char * ip;
    struct sockaddr_in client_address;
    memset(buf, '\0', MAX_LINE);

    /* Obtem o IP e Porta do cliente */
    socklen_t client_socklen = sizeof(client_address);
    if (getpeername(s, (struct sockaddr *) &client_address, &client_socklen) == 0) {
        printf("CLIENT CONNECTED\n");
        ip = inet_ntoa(client_address.sin_addr);
        printf("  IP: %s\n", ip);
        printf("PORT: %d\n\n", ntohs(client_address.sin_port));
    } else {
        perror("ERROR: Could not resolve remote port and ip values\n");
        exit(errno);
    }

    while (1) {
        int has_data;

        /* Recebe dado do cliente */
        if ((has_data = recv(s, buf, MAX_LINE, 0)) == -1) {
            perror("ERROR: unable to receive data");
            exit(errno);
        }

        if (!has_data)
            break;

        /* Mostra o dado enviado */
        printf("From %s:\n%s\n", ip, buf);

        /* Envia eco */
        if (send(s, buf, MAX_LINE, 0) == -1) {
            perror("ERROR: unable to send data");
            exit(errno);
        }
    }

    /* Fecha socket da conexão */
    if (close (s) == -1) {
        perror("ERROR: unable to close client socket");
        exit(errno);
    }

}

