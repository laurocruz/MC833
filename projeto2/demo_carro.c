#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "carro.h"

int main(int argc, char ** argv) {
    if (argc != 7) {
	printf("Usage: %s hostname safety_port entcon_port safety_use_tcp, entertainment_use_tcp comfort_use_tcp");
	exit(-1);
    }

    Car car;

    car.ts = time(NULL);
    car.speed = 0;
    car.dir = 1;
    car.size = 3;
    car.pos.x = -100;
    car.pos.y = 1;


    carro(&car, argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));

    return 0;

}
