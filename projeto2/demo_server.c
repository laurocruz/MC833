#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "server.h"

#define MAX_CARS 20

void echo(char * bufout, char * bufin) {
    memcpy(bufout, bufin, MAX_LINE);
}

int in_array(Car * cars, int n, int id) {
    for (int i = 0; i < n; i++) {
        if (cars[i].id == id)
            return i;
    }
    return -1;
}

void security_manager(char * bufout, char * bufin) {
    // 10 ms DELAY - CANT CHANGE
    usleep(10000);

    static int n = 0;
    static Car cars[MAX_CARS];
    static int crashed[MAX_CARS] = {0};

    char * tok = strtok(bufin, " ");
    int pos = in_array(cars, n, atoi(tok));

    if (pos == -1) pos = n++;

    cars[pos].id = atoi(tok);
    int ts = strtoul(strtok(NULL," "), NULL, 10);

    if (pos == n-1 || cars[pos].ts < ts) {
        cars[pos].ts = ts;
        cars[pos].speed = atoi(strtok(NULL," "));
        cars[pos].dir = atoi(strtok(NULL," "));
        cars[pos].size = atoi(strtok(NULL," "));
        cars[pos].pos.x = atoi(strtok(NULL," "));
        cars[pos].pos.y = atoi(strtok(NULL," "));
    }

    if (crashed[pos] == 1) {
        bufout[0] = '2'; bufout[1] = '\0';
        // Remove crashed car
        cars[pos] = cars[n-1];
        crashed[pos] = 0;
        n--;
        return;
    }

    for (int i = 0; i < n; i++) {
        if (i != pos) {
            int ci = cars[i].dir, cpos = cars[pos].dir;
            if (ci != cpos) {
                if (ci != (cpos + 2) % 4) {
                    int ts;
                    int ii, posi;

                    switch (cpos) {
                        case 0:
                            posi = -cars[pos].pos.y;
                            break;
                        case 1:
                            posi = -cars[pos].pos.x;
                            break;
                        case 2:
                            posi = cars[pos].pos.y;
                            break;
                        case 3:
                            posi = cars[pos].pos.x;
                            break;
                    }

                    switch (ci) {
                        case 0:
                            ii = -cars[i].pos.y;
                            break;
                        case 1:
                            ii = -cars[i].pos.x;
                            break;
                        case 2:
                            ii = cars[i].pos.y;
                            break;
                        case 3:
                            ii = cars[i].pos.x;
                            break;
                    }

                    if (cars[i].ts > cars[pos].ts) {
                        ts = cars[i].ts;
                        posi -= (ts - cars[pos].ts)*cars[pos].speed;

                    } else {
                        ts = cars[pos].ts;
                        ii -= (ts - cars[i].ts)*cars[i].speed;
                    }

                    int tsii, tsio, tsposi, tsposo;

                    // BATEU
                    if ((posi <= 0) && (posi + cars[pos].size >= 0) && (ii <= 0) && (ii + cars[i].size >= 0)) {
                        cars[pos] = cars[n-1];
                        crashed[n-1] = 0;

                        if (i != n-1)
                            crashed[i] = 1;
                        else crashed[pos] = 1;
                        n--;

                        bufout[0] = '2'; bufout[1] = '\0';
                        return;
                    }


                    if ((posi + cars[pos].size >= 0) && (ii + cars[i].size >= 0)) {
                        tsposi = ts + posi/cars[pos].speed;
                        tsposo = ts + (posi+cars[pos].size)/cars[pos].speed;

                        tsii = ts + ii/cars[i].speed;
                        tsio = ts + (ii+cars[i].size)/cars[i].speed;
                    }

                    // VAI BATER PORRA
                    if ((tsposi >= tsii && tsposi <= tsio) || (tsii >= tsposi && tsii <= tsposo)) {
                        // manda carro pos freiar
                        bufout[0] = '0'; bufout[1] = '\0';
                        return;
                    }

                } /* DEVERIA FAZER ALGO COM ISSO, MAS NAO SEI O QUE
                else {
                    // Um de frente para o outro (vai bater em algum momento)
                    if ((ci == 0 || ci == 2) && (cars[i].pos.x == cars[pos].pos.x)) {

                    } else if (ci == 1 || ci == 3) && (cars[i].pos.y == cars[pos].pos.y) {

                    }
                }
                */
            }
        }
    }

    // acelera
    bufout[0] = '1'; bufout[1] = '\0';

    /**********************************************/
    /************** freie - 0 *********************/
    /************** acelere - 1 *******************/
    /************** ambulancia - 2 ****************/
    /**********************************************/

}

void entcon_manager(char * bufout, char * bufin) {
    // 100 ms DELAY - CANT CHANGE
    usleep(100000);

    memcpy(bufout, bufin, MAX_LINE);
}

int main(int argc, char ** argv) {

    if (argc != 5) {
        printf("Usage: %s safety_tcp_port safety_udp_port ent_conf_tcp_port ent_conf_udp_port\n", argv[0]);
	exit(-1);
    }

    server(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),security_manager,entcon_manager);

    return 0;
}
