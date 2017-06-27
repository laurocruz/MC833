CFLAGS = -g -pedantic -Wall -fopenmp
LDLIBS= -lm -fopenmp

BIN= demo_carro demo_server
CLIENTOBJ = client.o carro.o demo_carro.o
SERVEROBJ = server.o demo_server.o

OBJ= $(CLIENTOBJ) $(SERVEROBJ)

all: $(BIN)

demo_carro: $(CLIENTOBJ)

demo_server: $(SERVEROBJ)

.PHONY = clean
clean:
	rm -f $(OBJ) $(BIN)
