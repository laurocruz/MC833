#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

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

    socket_address->sin_family = AF_INET;
    socket_address->sin_addr = *(struct in_addr *)host_address->h_addr_list[0];
    socket_address->sin_port = htons(port);


    if (type == SOCK_STREAM) {
        /* estabelecimento da conexão */
        if (connect(*s, (struct sockaddr *) socket_address, sizeof(*socket_address)) == -1) {
            perror("ERROR: Unable to connect to server");
            exit(errno);
        }
    }

    return 0;
}

static inline void make_msg(char * buf, char app, Car * car) {
    if (app != SECURITY)
	sprintf(buf, "%c", app);
    else
	sprintf(buf, "%d %lu %d %d %d %d", car->id, car->ts, car->speed, car->dir, car->size, car->pos);
}

int client_tcp(char * hostname, int port, Car * car, char app, int (*app_fun)(char *, Car *)) {
    struct sockaddr_in socket_address;
    //struct sockaddr_in local_address;
    char buf[MAX_LINE];
    int s;
    int roundtrip_count = 0;
    long long latency_sum = 0;
    
    get_socket(&socket_address, &s, hostname, port, SOCK_STREAM);

    /* ler e enviar linhas de texto, receber eco */
    memset(buf, '\0', MAX_LINE);

    while (1) {
        make_msg(buf, app, car);

	struct timespec now, start;
	clock_gettime(CLOCK_MONOTONIC, &start);
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
	clock_gettime(CLOCK_MONOTONIC, &now);
	
	latency_sum += (now.tv_nsec - start.tv_nsec)
	    + (now.tv_sec - start.tv_sec)*1000000000;
	roundtrip_count++;
	
        /******************************************/
        /**************** DO STUFF ****************/
        if (app_fun(buf, car) != 0)
            break;
        /******************************************/

        memset(buf, '\0', MAX_LINE);
    }

    if (roundtrip_count)
      printf ("avg. TCP latency = %lld\n", latency_sum/(1000000 * roundtrip_count));

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

    get_socket(&socket_address, &s, hostname, port, SOCK_DGRAM);

    /* ler e enviar linhas de texto, receber eco */
    memset(buf, '\0', MAX_LINE);

    int roundtrip_count = 0;
    long long latency_sum = 0;


    while (1) {
        make_msg(buf, app, car);

	
	struct timespec now, start;
	clock_gettime(CLOCK_MONOTONIC, &start);


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

	clock_gettime(CLOCK_MONOTONIC, &now);
	
	latency_sum += (now.tv_nsec - start.tv_nsec)
	    + (now.tv_sec - start.tv_sec)*1000000000;
	roundtrip_count++;


        /******************************************/
        /**************** DO STUFF ****************/
        if (app_fun(buf, car) != 0)
            break;
        /******************************************/

        memset(buf, '\0', MAX_LINE);
    }

    if (roundtrip_count)
    printf ("avg. UDP latency = %lld\n", latency_sum/(1000000 * roundtrip_count));

    /* Fecha o socket do cliente */
    if (close(s) == -1) {
        perror("ERROR: unable to close socket");
        exit(errno);
    }

    return 0;
}
