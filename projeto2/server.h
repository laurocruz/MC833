#ifndef _SERVER_H
#define _SERVER_H

#include "carro.h"

#define MAX_PENDING 5
#define MAX_LINE 512

int server(int sec_tcp_p, int sec_udp_p, int ec_tcp_p, int ec_udp_p, void (*sec)(char *, char *), void (*ent_con)(char *, char *));

#endif /* _SERVER_H */
