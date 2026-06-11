#include "registro.h"
#include <string.h>
#include <stdlib.h>

int registrar_operacion(char *host, char *nombre, char *operacion, char *fichero)
{
    CLIENT *clnt;
    int resultado;
    struct operacion_usuario args;
    
    clnt = clnt_create(host, LOGSRPC_PROG, LOGSRPC_VERS, "tcp");
    if (clnt == NULL) {
        return -1;  // No falla el servidor principal
    }
    
    args.nombre = nombre;
    args.operacion = operacion;
    args.nombrefichero = fichero;
    
    if (imprimir_log_1(args, &resultado, clnt) != TRUE) {
        clnt_destroy(clnt);
        return -1;
    }
    
    clnt_destroy(clnt);
    return resultado;
}
