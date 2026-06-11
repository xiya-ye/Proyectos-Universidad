
struct operacion_usuario
{
   string nombre<256>;
   string operacion<256>;
   string nombrefichero<256>;
} ;

program LOGSRPC_PROG {
 version LOGSRPC_VERS {
    int IMPRIMIR_LOG (struct operacion_usuario argumentos) = 1 ;
    } = 1;
 } = 100522255;