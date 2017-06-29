#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "server.h"
#include "carro.h"

#define MAX_CARS 100

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


// retorna em qual valor da coordenada fixa do carro uma colisão é possível
// ex: carro movendo pra cima tem x fixo em 0
int possible_collision(Direction dir)
{
    switch (dir) {
    case UP:
    case LEFT:
	return 0;
    case RIGHT:
    case DOWN:
	return -1;
    }
}

// checa se há possibilidade de colisão
int check_collision(Direction dir, int pos, int size, int possible)
{
    switch (dir) {
    case UP:
    case RIGHT:
	return !(pos < possible || pos-size > possible);
    case DOWN:
    case LEFT:
	return !(pos > possible || pos+size < possible);
    }
}


// calcula tempos de entrada e saída do cruzamento
void enter_exit_time(double * enter_time, double * exit_time, Direction dir, int pos, int size, double speed)
{
    switch (dir) {
    case UP:
    case RIGHT:
	*enter_time = -(1 + pos) / speed;
	*exit_time = -(pos - size) / speed;
	break;
    case DOWN:
    case LEFT:
	*enter_time = pos / speed;
	*exit_time = (pos + size + 1) / speed;
	break;
    }
    //printf ("dir: %s pos: %d speed: %lf enter_time: %.3lf exit_time: %.3lf\n", directions[dir], pos, speed, *enter_time, *exit_time);

}


#define NADA '0'
#define FREIE '1'
#define ACELERE '2'
#define AMBULANCIA '3'

void security_manager(char * bufout, char * bufin) {
    // 10 ms DELAY - CANT CHANGE
    usleep(10000);

    bufout[1] = '\0';
    static int n = 0;
    static Car cars[MAX_CARS];
    static int crashed[MAX_CARS] = {0};

    char * tok = strtok(bufin, " ");
    int pos = in_array(cars, n, atoi(tok));

    if (pos == -1) {
	if (n == MAX_CARS) {
	    bufout[0] = AMBULANCIA;
	    return;
	} else {
	    pos = n++;
	    crashed[pos] = 0;
	}
    }

    if (crashed[pos] == 1) {
        bufout[0] = AMBULANCIA;
        // Remove crashed car
        cars[pos] = cars[--n];
        crashed[pos] = crashed[n];
        return;
    }

    cars[pos].id = atoi(tok);
    int ts = strtoul(strtok(NULL," "), NULL, 10);

    if (pos == n-1 || cars[pos].ts < ts) {
	cars[pos].ts = ts;
	cars[pos].speed = atoi(strtok(NULL," "));
	cars[pos].dir = atoi(strtok(NULL," "));
	cars[pos].size = atoi(strtok(NULL," "));
	cars[pos].pos = atoi(strtok(NULL," "));
    }


    double pos_enter_time, pos_exit_time;
    enter_exit_time(
	&pos_enter_time,
	&pos_exit_time,
	cars[pos].dir,
	cars[pos].pos,
	cars[pos].size,
	cars[pos].speed);


    if (pos_exit_time <= 0) {
	// já passamos do cruzamento, acelerar e remover
	bufout[0] = ACELERE;
	cars[pos] = cars[--n];
	crashed[pos] = crashed[n];
	return;
    }

    // padrão é nenhum comando / manter
    bufout[0] = NADA;

    for (int i = 0; i < n; i++) {

	// desconsiderar já batidos
	if (crashed[i])
	    continue;

	int di = cars[i].dir, dpos = cars[pos].dir;
	// sem risco de colisão
	if ((di == UP || di == DOWN) && (dpos == UP || dpos == DOWN))
	    continue;
	else if ((di == RIGHT || di == LEFT) && (dpos == RIGHT || dpos == LEFT))
	    continue;

	// ajusta posição do outro carro pro tempo atual deste carro;
	int posi = cars[i].pos;
	/* switch (di) { */
	/* case UP: */
	/* case RIGHT: */
	/*     posi = cars[i].pos + (cars[pos].ts - cars[i].ts)*cars[i].speed; */
	/*     break; */
	/* case DOWN: */
	/* case LEFT: */
	/*     posi = cars[i].pos - (cars[pos].ts - cars[i].ts)*cars[i].speed; */
	/*     break; */
	/* } */

	double i_enter_time, i_exit_time;

	enter_exit_time(
	    &i_enter_time,
	    &i_exit_time,
	    cars[i].dir,
	    posi,
	    cars[i].size,
	    cars[i].speed);

	// outro carro já passou do cruzamento, desconsiderar e remover
	if (i_exit_time <= 0) {
	    cars[i] = cars[--n];
	    crashed[i] = crashed[n];
	    continue;
	}

	// não há intersecção entre os tempos de passagem do cruzamento
	if (i_enter_time >= pos_exit_time || pos_enter_time >= i_exit_time)
	    continue;

	// ainda não chegamos no cruzamento
	if (pos_enter_time >= 0) {

	    // estamos mais longe do cruzamento, freiar
	    if (pos_enter_time >= i_enter_time) {
		printf ("evitar colisão: ID: %d - %s vel: %d pos: %d size: %d - %s vel: %d pos: %d size: %d\n",
			cars[pos].id, directions[dpos], cars[pos].speed, cars[pos].pos, cars[pos].size,
			directions[di], cars[i].speed, posi, cars[i].size);
		bufout[0] = FREIE;
		return;
	    }  else {
		// preparar pra acelerar, continuar escaneando outros carros
		bufout[0] = ACELERE;
		continue;
	    }
	}


	// já chegamos ao cruzamento, checar se houve de fato colisão

	if (!check_collision(dpos, cars[pos].pos, cars[pos].size, possible_collision(di)))
	    continue;

	if (!check_collision(di, posi, cars[i].size, possible_collision(dpos)))
	    continue;

	// bateu :(

	printf ("colisão: ID: %d - %s vel: %d pos: %d size: %d - %s vel: %d pos: %d size: %d\n",
		cars[pos].id, directions[dpos], cars[pos].speed, cars[pos].pos, cars[pos].size,
		directions[di], cars[i].speed, posi, cars[i].size);

	cars[pos] = cars[n-1];
	crashed[n-1] = 0;

	if (i != n-1)
	    crashed[i] = 1;
	else crashed[pos] = 1;
	n--;

	bufout[0] = AMBULANCIA;
	return;    // manter
    }

    /**********************************************/
    /************** nenhum comando - 0 ************/
    /************** freie - 1 *********************/
    /************** acelere - 2 *******************/
    /************** ambulancia - 3 ****************/
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
