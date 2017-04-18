#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <netdb.h>

#define SERVER_PORT 12345
#define MAX_LINE 256

int main(int argc, char * argv[]) {
        struct hostent *host_address;
        struct sockaddr_in socket_address;
        char *host;
        char buf[MAX_LINE];
        int s;
        int len;


	/* verificaÃ§Ã£o de argumentos */
	if (argc != 2)  {
        printf("ERROR: Wrong number of arguments\n");
        printf("%s hostname\n", argv[0]);
        exit(1);
    }
    host = argv[1];

	/* traduÃ§Ã£o de nome para endereÃ§o IP */
	host_address = gethostbyname(host);

    if (!host_address) {
        printf("ERROR: Unable to resolve hostname\n");
        exit(1);
    }

    /* criaÃ§Ã£o de socket ativo*/
    
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("ERROR: Unable to create socket\n");
        exit(1);
    }

    /* criaÃ§Ã£o da estrutura de dados de endereÃ§o */
    bzero((char *)&socket_address, sizeof(socket_address));
    
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr = *(struct in_addr *)host_address->h_addr_list[0];
    socket_address.sin_port = htons(SERVER_PORT);


	/* estabelecimento da conexÃ£o */
    if (connect(s, (struct sockaddr *) &socket_address, sizeof(socket_address)) == -1) {
        printf("ERROR: Unable to connect to server\n");
        exit(1);
    }

    /* ler e enviar linhas de texto, receber eco */

    /* Envia texto */
    memset(&buf, '\0', sizeof(buf));
    sprintf(buf, "TESTE_cli\n");
    send(s, buf, strlen(buf), 0);

    /* Recebe eco */
    recv(s, buf, strlen(buf), 0);
    printf("%s\n", buf);

    return 0;
}
