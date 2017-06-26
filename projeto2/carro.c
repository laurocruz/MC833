#include <stdio.h>
#include <time.h>
#include <omp.h>

#include "carro.h"
#include "client.h"

void security_manager(char * msg, Car * car) {
    int ts = time(NULL);
    car->pos.y += (ts - car->ts)*car->speed;
    car->ts = ts;
    car->speed++;

    printf("%s\n", msg);
}

void entertainment_manager(char * msg, Car * car) {
    printf("%s\n", msg);
}

void confort_manager(char * msg, Car * car) {
    printf("%s\n", msg);
}

int carro(Car * car, char * hostname, int sec_port, int entcon_port, int sec_tcp, int ent_tcp, int con_tcp) {
    int tid;

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
