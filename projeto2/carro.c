#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <omp.h>

#include "carro.h"
#include "client.h"

static int messages = 0, empty = 0, brake = 0, throttle = 0, ambulance = 0;

int security_manager(char * msg, Car * car) {
    // 0 - nenhum comando / manter
    // 1 - freie
    // 2 - acelere
    // 3 - ambulancia
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

    messages++;

    car->ts = ts;

    if (f == 0) {
	empty++;
    } else if (f == 1) {
        if (car->speed >= car->accel_down + 1)
            car->speed -= car->accel_down;
        else
	    car->speed = 1;
	brake++;
    } else if (f == 2) {
	if (car->speed + car->accel_up < car->max_speed)
	    car->speed += car->accel_up;
	else
	    car->speed = car->max_speed;
	//      printf("ACELERE\n");
	throttle++;
    } else {
        car->speed = 0;
	ambulance++;
        return 1;
    }
    sleep(1);

    switch (d) {
    case UP:
    case RIGHT:
	return car->pos - car->size >= 0;
    case DOWN:
    case LEFT:
	return car->pos + car->size < 0;
    }
}

int entertainment_manager(char * msg, Car * car) {
    // Time until next request
    sleep(10);
    return 0
}

int confort_manager(char * msg, Car * car) {
    // Time until next request
    sleep(10);
    return 0;
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
	    printf("%d %d %d %d %d\n", messages, empty, brake, throttle, ambulance);
	    exit(ambulance);
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
