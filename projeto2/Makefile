CC := gcc
CCFLAGS := -fopenmp -lm -g -pedantic -Wall
LDFLAGS :=
SERVER_OBJ := server.o demo_server.o
CLIENT_OBJ := client.o carro.o demo_carro.o
ARGS=$(filter-out $@,$(MAKECMDGOALS))

all: demo_carro demo_server

demo_carro: $(CLIENT_OBJ)
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $@ $(CLIENT_OBJ)

demo_server: $(SERVER_OBJ)
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $@ $(SERVER_OBJ)

client.o: client.c
	$(CC) $(CCFLAGS) $(LDFLAGS) -c $<

carro.o: carro.c
	$(CC) $(CCFLAGS) $(LDFLAGS) -c $<

demo_carro.o: demo_carro.c
	$(CC) $(CCFLAGS) $(LDFLAGS) -c $<

server.o: server.c
	$(CC) $(CCFLAGS) $(LDFLAGS) -c $<

demo_server.o: demo_server.c
	$(CC) $(CCFLAGS) $(LDFLAGS) -c $<

clean: 
	rm $(SERVER_OBJ) $(CLIENT_OBJ) demo_carro demo_server
