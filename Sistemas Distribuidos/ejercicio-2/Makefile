CC=gcc
CFLAGS=-Wall -Wextra -fPIC

all: libclaves.so libproxyclaves.so servidor-sock app-cliente-sock app-cliente-sock-concurrente

claves.o: claves.c claves.h
	$(CC) $(CFLAGS) -c claves.c

libclaves.so: claves.o
	$(CC) -shared -o libclaves.so claves.o

lines.o: lines.c lines.h
	$(CC) $(CFLAGS) -c lines.c

proxy-sock.o: proxy-sock.c claves.h lines.h
	$(CC) $(CFLAGS) -c proxy-sock.c

libproxyclaves.so: proxy-sock.o lines.o
	$(CC) -shared -o libproxyclaves.so proxy-sock.o lines.o

servidor-sock: servidor-sock.c libclaves.so lines.o lines.h
	$(CC) -Wall servidor-sock.c lines.o -o servidor-sock -L. -lclaves -lpthread

app-cliente-sock: app-cliente-1.c libproxyclaves.so
	$(CC) -Wall app-cliente-1.c -o app-cliente-sock -L. -lproxyclaves

app-cliente-sock-concurrente: app-cliente-2.c libproxyclaves.so
	$(CC) -Wall app-cliente-2.c -o app-cliente-sock-concurrente -L. -lproxyclaves -lpthread

# MAKE CLEAN
clean:
	rm -f *.o *.so app-cliente servidor-sock app-cliente-sock app-cliente-sock-concurrente
	rm -rf ./ALMACEN