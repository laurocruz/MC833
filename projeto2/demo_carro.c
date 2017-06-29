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
      printf("Usage: %s hostname safety_port entcon_port safety_use_tcp, entertainment_use_tcp comfort_use_tcp", argv[0]);
	exit(-1);
    }
    setbuf(stdout, NULL);
    
    Car car;

    int urandom = open("/dev/urandom", O_RDONLY);
    unsigned int seed;
    read(urandom, &seed, sizeof(seed));
    close(urandom);
    srand(seed);


    while (1) {
	car.ts = time(NULL);
	car.speed = MAX_SPEED - (rand() % (MAX_SPEED / 2));
	car.dir = rand() % 4;
	car.size = (rand() % 3) + 1;

	switch (car.dir) {
	case UP:
	case RIGHT:
	    car.pos = -LIMIT;
	    break;
	case LEFT:
	case DOWN:
	    car.pos = LIMIT-1;
	}

	char up[] = "cima";
	char right[] = "direita";	
	char down[] = "baixo";
	char left[] = "esquerda";

	char* directions[] = {up, right, down, left};
	

	//printf("\nNovo carro, tamanho: %d, direção: %s\n", car.size, directions[car.dir]);

	carro(&car, argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));
	putchar('\n');
    }

    return 0;

}
