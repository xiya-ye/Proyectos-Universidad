CC=gcc
CFLAGS=-Wall -Wextra -g -fPIC -I/usr/include/tirpc
LDFLAGS=-L. -Wl,-rpath=.
LDLIBS=-lpthread -ldl -ltirpc

all: libclaves.so libproxyclaves.so servidor-rpc app-cliente-rpc app-cliente-rpc-concurrente


claves.o: claves.c claves.h
	$(CC) $(CFLAGS) -c claves.c

libclaves.so: claves.o
	$(CC) -shared -o libclaves.so claves.o

clavesRPC_xdr.o: clavesRPC_xdr.c clavesRPC.h
	$(CC) $(CFLAGS) -c clavesRPC_xdr.c

clavesRPC_clnt.o: clavesRPC_clnt.c clavesRPC.h
	$(CC) $(CFLAGS) -c clavesRPC_clnt.c

clavesRPC_svc.o: clavesRPC_svc.c clavesRPC.h
	$(CC) $(CFLAGS) -c clavesRPC_svc.c

proxy-rpc.o: proxy-rpc.c claves.h clavesRPC.h
	$(CC) $(CFLAGS) -c proxy-rpc.c

libproxyclaves.so: proxy-rpc.o clavesRPC_clnt.o clavesRPC_xdr.o
	$(CC) -shared -o libproxyclaves.so proxy-rpc.o clavesRPC_clnt.o clavesRPC_xdr.o $(LDLIBS)

servidor-rpc.o: servidor-rpc.c claves.h clavesRPC.h
	$(CC) $(CFLAGS) -c servidor-rpc.c

servidor-rpc: servidor-rpc.o clavesRPC_svc.o clavesRPC_xdr.o libclaves.so
	$(CC) $(CFLAGS) servidor-rpc.o clavesRPC_svc.o clavesRPC_xdr.o -o servidor-rpc $(LDFLAGS) -lclaves $(LDLIBS)

app-cliente-rpc: app-cliente-1.c libproxyclaves.so
	$(CC) $(CFLAGS) app-cliente-1.c -o app-cliente-rpc $(LDFLAGS) -lproxyclaves $(LDLIBS)

app-cliente-rpc-concurrente: app-cliente-2.c libproxyclaves.so
	$(CC) $(CFLAGS) app-cliente-2.c -o app-cliente-rpc-concurrente $(LDFLAGS) -lproxyclaves $(LDLIBS)

# MAKE CLEAN
clean:
	rm -f *.o *.so servidor-rpc app-cliente-rpc app-cliente-rpc-concurrente
	rm -f clavesRPC_client.c clavesRPC_server.c
	rm -rf ./ALMACEN