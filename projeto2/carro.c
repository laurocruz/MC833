#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <omp.h>

#include "carro.h"
#include "client.h"

int security_manager(char * msg, Car * car) {
    // 0 - freie
    // 1 - acelere
    // 2 - ambulancia
    int f = atoi(msg);
    int ts = time(NULL);
    int d = car->dir;

    switch (d) {
    case UP:
    case RIGHT:
	car->pos += (ts - car->ts)*car->speed;
	break;
    case DOWN:
    case LEFT:
	car->pos -= (ts - car->ts)*car->speed;
	break;
    }

    car->ts = ts;

    if (f == 0) {
        if (car->speed >= SPEED_DOWN)
            car->speed -= SPEED_DOWN;
        else car->speed = 0;
	for (int i = 0; i < car->speed; i++)
	    putchar('-');

    } else if (f == 1) {
	int speed_up = (rand() % SPEED_UP) + 1;
	if (car->speed + speed_up < MAX_SPEED)
	    car->speed += speed_up;
	else
	    car->speed = MAX_SPEED;
	//      printf("ACELERE\n");
	for (int i = 0; i < car->speed; i++)
	    putchar('.');
    } else {
        car->speed = 0;
	putchar('#');
	car->pos = LIMIT;
        return 1;
    }
    usleep(1500000);

    return car->pos < -LIMIT || car->pos > LIMIT-1;
}

int entertainment_manager(char * msg, Car * car) {
    // Time until next request
    sleep(2);

    return car->pos < -LIMIT || car->pos > LIMIT-1;
}

int confort_manager(char * msg, Car * car) {
    // Time until next request
    sleep(2);

    return car->pos < -LIMIT || car->pos > LIMIT-1;
}

int carro(Car * car, char * hostname, int sec_port, int entcon_port, int sec_tcp, int ent_tcp, int con_tcp) {
    int tid;

    // Create car ID

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
