#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "carro.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char ** argv) {
    if (argc != 7) {
	printf("Usage: %s hostname safety_port entcon_port safety_use_tcp entertainment_use_tcp comfort_use_tcp\n", argv[0]);
	exit(-1);
    }
    setbuf(stdout, NULL);
    
    Car car;

    int urandom = open("/dev/urandom", O_RDONLY);
    unsigned int seed;
    read(urandom, &seed, sizeof(seed));
    close(urandom);
    srand(seed);

    int range;

    scanf ("%d %d %d %d %d %d %d", &car.speed, &car.dir, &car.size, &car.accel_up, &car.accel_down, &car.max_speed, &range);
    car.ts = time(NULL);

    switch (car.dir) {
    case UP:
    case RIGHT:
	car.pos = -range;
	break;
    case LEFT:
    case DOWN:
	car.pos = range-1;
    }

    carro(&car, argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));

    return 0;

}
