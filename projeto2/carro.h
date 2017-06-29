#ifndef _CARRO_H
#define _CARRO_H

typedef enum {UP, RIGHT, DOWN, LEFT} Direction;


static char up[] = "sobe";
static char right[] = "direita";	
static char down[] = "desce";
static char left[] = "esquerda";

static char* directions[] = {up, right, down, left};


typedef struct car {
    int id;
    long unsigned ts;
    Direction dir;
    int speed, size, accel_up, accel_down, max_speed, pos;
} Car;

int carro(Car * car, char * hostname, int sec_port, int entcon_port, int sec_tcp, int ent_tcp, int con_tcp);

#endif /* _CARRO_H */
