#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"

void echo(char * bufout, char * bufin) {
    memcpy(bufout, bufin, MAX_LINE);
}

int main(int argc, char ** argv) {
    server(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),echo,echo);

    return 0;
}
