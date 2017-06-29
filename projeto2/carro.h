#ifndef _CARRO_H
#define _CARRO_H

typedef enum {UP, RIGHT, DOWN, LEFT} Direction;


#define SPEED_DOWN 3
#define SPEED_UP 2

#define MAX_SPEED 10

#define LIMIT 20

static char up[] = "sobe";
static char right[] = "direita";	
static char down[] = "desce";
static char left[] = "esquerda";

static char* directions[] = {up, right, down, left};


typedef struct car {
    int id;
    long unsigned ts;
    Direction dir;
    int speed, size;
    int pos;
} Car;

int carro(Car * car, char * hostname, int sec_port, int entcon_port, int sec_tcp, int ent_tcp, int con_tcp);

#endif /* _CARRO_H */
