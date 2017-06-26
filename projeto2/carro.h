#ifndef _CARRO_H
#define _CARRO_H

typedef struct pos {
    int x, y;
} Pos;

typedef struct car {
    long unsigned ts;
    int speed, dir, size;
    Pos pos;
} Car;

int carro(Car * car, char * hostname, int sec_port, int entcon_port, int sec_tcp, int ent_tcp, int con_tcp);

#endif /* _CARRO_H */
