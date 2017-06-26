#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "carro.h"

int main(int argc, char ** argv) {
    Car car;

    car.ts = time(NULL);
    car.speed = 0;
    car.dir = 1;
    car.size = 2;
    car.pos.x = 0;
    car.pos.y = 0;

    carro(&car, argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));

    return 0;

}
