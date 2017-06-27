#ifndef _CLIENT_H
#define _CLIENT_H

#include "carro.h"

#define SECURITY 's'
#define ENTERTAINMENT 'e'
#define CONFORT 'c'

#define MAX_LINE 512

int client_udp(char * hostname, int port, Car * car, char app, int (*app_fun)(char *, Car *));
int client_tcp(char * hostname, int port, Car * car, char app, int (*app_fun)(char *, Car *));

#endif /* _CLIENT_D */
