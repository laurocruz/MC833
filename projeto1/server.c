#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define LISTEN_PORT 12345
#define MAX_PENDING 5
#define MAX_LINE 256

int main() {
    struct sockaddr_in socket_address;
    char buf[MAX_LINE];
    unsigned int len;
    int s, new_s;

    /* criaÃ§Ã£o da estrutura de dados de endereÃ§o */
    bzero((char *)&socket_address, sizeof(socket_address));
    
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.a_addr = htonl(INADDR_ANY);
    socker_address.sin_port = htons(LISTEN_PORT);
    
    /* criaÃ§Ã£o de socket passivo */
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("ERROR: Unable to create socket\n");
        exit(1)
    }

    /* Associar socket ao descritor */
    bind (s, (struct sockaddr *)&socket_address, sizeof(socket_address));

    /* Criar escuta do socket para aceitar conexÃµes */
    if (listen(s, (struct sockaddr *)&socket_address, MAX_PENDING)) {
        printf("ERROR: Unable to invoke listen method \n");
        exit(1);
    }

    /* aguardar/aceita conexÃ£o, receber e imprimir texto na tela, enviar eco */
    if ((new_s = accept(s, (struct sockaddr *)NULL, NULL)) == -1) {
        printf("ERROR: Unable to get client socket\n");
        exit(1);
    }

    memset(buf, '\0', sizeof(buf));
    recv(new_s, buf, sizeof(buf), 0);
    printf("%s\n", buf);
    
    sprintf(buf, "TESTE_ser\n"); 
    send(new_s, buf, strlen(buf));

    return 0;
}
