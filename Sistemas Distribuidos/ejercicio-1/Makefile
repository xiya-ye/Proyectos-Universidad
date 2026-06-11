CC=gcc
CFLAGS=-Wall -Wextra -fPIC

all: libclaves.so libproxyclaves.so app-cliente servidor-mq app-cliente-mq

# VERSIÓN A - MONOLÍTICA
claves.o: claves.c claves.h
	$(CC) $(CFLAGS) -c claves.c

libclaves.so: claves.o
	$(CC) -shared -o libclaves.so claves.o

app-cliente: app-cliente.c libclaves.so
	$(CC) -Wall app-cliente.c -o app-cliente -L. -lclaves

# VERSIÓN B - DISTRIBUIDA CON COLAS DE MENSAJES
proxy-mq.o: proxy-mq.c claves.h mensaje.h
	$(CC) $(CFLAGS) -c proxy-mq.c

libproxyclaves.so: proxy-mq.o
	$(CC) -shared -o libproxyclaves.so proxy-mq.o -lrt

servidor-mq: servidor-mq.c libclaves.so mensaje.h
	$(CC) -Wall servidor-mq.c -o servidor_mq -L. -lclaves  -lrt -lpthread

app-cliente-mq: app-cliente.c libproxyclaves.so
	$(CC) -Wall app-cliente.c -o app-cliente-mq -L. -lproxyclaves  -lrt

# MAKE CLEAN
clean:
	rm -f *.o *.so app-cliente servidor-mq app-cliente-mq
	rm -rf ./ALMACEN