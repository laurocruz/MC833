#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <omp.h>

#include "carro.h"
#include "client.h"

#define SPEED_DOWN 10
#define SPEED_UP 1

int security_manager(char * msg, Car * car) {
    // 0 - freie
    // 1 - acelere
    // 2 - ambulancia
    int f = atoi(msg);
    int ts = time(NULL);
    int d = car->dir;

    // Cima
    if (d == 0) {
        car->pos.y += (ts - car->ts)*car->speed;
    // Direita
    } else if (d == 1) {
        car->pos.x += (ts - car->ts)*car->speed;
    // Baixo
    } else if (d == 2) {
        car->pos.y -= (ts - car->ts)*car->speed;
    // Esquerda
    } else {
        car->pos.x -= (ts - car->ts)*car->speed;
    }
    car->ts = ts;

    if (f == 0) {
        if (car->speed >= SPEED_DOWN)
            car->speed -= SPEED_DOWN;
        else car->speed = 0;

        printf("FREIE\n");
    } else if (f == 2) {
        car->speed += SPEED_UP;
        printf("ACELERE\n");
    } else {
        car->speed = 0;
        printf("AMBULANCIA\n");

        return 1;
    }

    return 0;
}

int entertainment_manager(char * msg, Car * car) {
    // Time until next request
    sleep(2);

    return 0;
}

int confort_manager(char * msg, Car * car) {
    // Time until next request
    sleep(2);

    return 0;
}

int carro(Car * car, char * hostname, int sec_port, int entcon_port, int sec_tcp, int ent_tcp, int con_tcp) {
    int tid;

    // Create car ID
    srand(time(NULL));
    car->id = rand();

    // Threads to deal with
    #pragma omp parallel private(tid) shared(hostname, car, sec_port, entcon_port, sec_tcp, ent_tcp, con_tcp) num_threads(3)
    {
        tid = omp_get_thread_num();

        if (tid == 0) {
            if (sec_tcp == 1)
                client_tcp(hostname, sec_port, car, SECURITY, security_manager);
            else
                client_udp(hostname, sec_port, car, SECURITY, security_manager);

        } else if (tid == 1) {
            if (ent_tcp == 1)
                client_tcp(hostname, entcon_port, car, ENTERTAINMENT, entertainment_manager);
            else
                client_udp(hostname, entcon_port, car, ENTERTAINMENT, entertainment_manager);

        } else {
            if (con_tcp == 1)
                client_tcp(hostname, entcon_port, car, CONFORT, confort_manager);
            else
                client_udp(hostname, entcon_port, car, CONFORT, confort_manager);
        }
    }
    return 0;
}
