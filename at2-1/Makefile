CC=gcc
ARGS=$(filter-out $@,$(MAKECMDGOALS))

all: server client

run_client: client
	./client ${ARGS}

run_server: server
	./server ${ARGS}

client: client.c
	${CC} client.c -o client

server: server.c
	${CC} server.c -o server

clean: 
	rm client server
