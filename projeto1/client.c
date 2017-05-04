#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

#define SERVER_PORT 12345
#define MAX_LINE 256

int main(int argc, char * argv[]) {
    struct hostent *host_address;
    struct sockaddr_in socket_address;
    char *host;
    char buf[MAX_LINE];
    int s;
    int len;
    int port;


    /* verificação de argumentos */
    if (argc != 2 && argc != 3)  {
        printf("ERROR: Wrong number of arguments\n");
        printf("Usage: %s hostname [PORT]\n", argv[0]);
        exit(errno);
    } else if (argc == 3) {
        port = atoi(argv[2]);
    } else port = SERVER_PORT;

    host = argv[1];

    /* tradução de nome para endereço IP */
    host_address = gethostbyname(host);

    if (!host_address) {
        perror("ERROR: Unable to resolve hostname\n");
        exit(errno);
    }

    /* criação de socket ativo*/

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("ERROR: Unable to create socket");
        exit(errno);
    }

    /* criação da estrutura de dados de endereço */
    bzero((char *)&socket_address, sizeof(socket_address));

    socket_address.sin_family = AF_INET;
    socket_address.sin_addr = *(struct in_addr *)host_address->h_addr_list[0];
    socket_address.sin_port = htons(port);


    /* estabelecimento da conexão */
    if (connect(s, (struct sockaddr *) &socket_address, sizeof(socket_address)) == -1) {
        perror("ERROR: Unable to connect to server");
        exit(errno);
    }

    /* ler e enviar linhas de texto, receber eco */

    /* Envia texto */
    memset(&buf, '\0', MAX_LINE);
    fgets(buf, MAX_LINE, stdin);
    //sprintf(buf, "TESTE_%d", i);
    send(s, buf, MAX_LINE, 0);

    /* Recebe eco */
    recv(s, buf, MAX_LINE, 0);
    printf("%s", buf);

    close(s);

    return 0;
}
