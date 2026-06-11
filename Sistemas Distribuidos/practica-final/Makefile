CC = gcc
CFLAGS = -Wall -Wextra -g -fPIC -I/usr/include/tirpc
LDFLAGS = -L. -Wl,-rpath=.
LDLIBS = -lpthread -ldl -ltirpc

# Ejecutables
all: server rpc_server

# Compilar servidor de mensajería
server: servidor.o lines.o usuarios.o rpc_client.o registro_clnt.o registro_xdr.o
	$(CC) -o server $^ $(LDLIBS)

# Compilar servidor RPC - usa registro_svc.c (tiene el main), no registro_server.c
rpc_server: registro_svc.o registro_server.o registro_xdr.o
	$(CC) -o rpc_server registro_svc.o registro_server.o registro_xdr.o $(LDLIBS)

# Reglas de compilación
servidor.o: servidor.c lines.h usuarios.h rpc_client.h
	$(CC) $(CFLAGS) -c servidor.c

lines.o: lines.c lines.h
	$(CC) $(CFLAGS) -c lines.c

usuarios.o: usuarios.c usuarios.h
	$(CC) $(CFLAGS) -c usuarios.c

rpc_client.o: rpc_client.c rpc_client.h registro.h
	$(CC) $(CFLAGS) -c rpc_client.c

registro_clnt.o: registro_clnt.c registro.h
	$(CC) $(CFLAGS) -c registro_clnt.c

registro_svc.o: registro_svc.c registro.h
	$(CC) $(CFLAGS) -c registro_svc.c

registro_server.o: registro_server.c registro.h
	$(CC) $(CFLAGS) -c registro_server.c

registro_xdr.o: registro_xdr.c registro.h
	$(CC) $(CFLAGS) -c registro_xdr.c

# Limpiar
clean:
	rm -f *.o server rpc_server
	rm -rf ./ALMACEN

# Limpiar todo
clean-all: clean
	rm -f registro_clnt.c registro_svc.c registro_server.c registro_xdr.c registro.h

.PHONY: all clean clean-all