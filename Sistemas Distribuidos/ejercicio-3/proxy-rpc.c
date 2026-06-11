#include "clavesRPC.h"
#include "claves.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int destroy (void)
{	
	CLIENT *clnt;
	char *ip_tuplas;
	enum clnt_stat retval_1;
	int result_1;

    // Obtenemos la IP del servidor
	ip_tuplas = getenv("IP_TUPLAS");
	if (NULL == ip_tuplas) {
		return -1;
	}

    // Creamos la conexion RPC
	clnt = clnt_create(ip_tuplas, CLAVES, CLAVES_VERSION, "tcp");
	if (NULL == clnt) { 
        clnt_pcreateerror(ip_tuplas);
        return -1;
    }
	
    // Realizamos la llamada RPC
	retval_1 = rpc_destroy_1(&result_1, clnt);
	if (retval_1 != RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
        clnt_destroy(clnt);
		return -1;
	}

	clnt_destroy(clnt);
	return result_1;
}

int set_value (char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3)
{
    CLIENT *clnt;
	char *ip_tuplas;
	int result_1;
    enum clnt_stat retval_1;
    struct set_value_arg arg;

    // Obtenemos la IP del servidor
    ip_tuplas = getenv("IP_TUPLAS");
    if (NULL == ip_tuplas) {
        return -1;
    }

    //Creamos la conexion RPC
    clnt = clnt_create(ip_tuplas, CLAVES, CLAVES_VERSION, "tcp");
	if (NULL == clnt) { 
        clnt_pcreateerror(ip_tuplas);
        return -1;
    }
    
    // Preparamos los datos para el envio
    strncpy(arg.key, key, 256);
    strncpy(arg.value1, value1, 256);
    arg.N_value2 = N_value2;
    for (int i = 0; i < N_value2; i++) {
        arg.V_value2[i] = V_value2[i];
    }
    arg.value3.x = value3.x;
    arg.value3.y = value3.y;
    arg.value3.z = value3.z;

    // Realizamos la llamada RPC
    retval_1 = rpc_set_value_1(arg, &result_1, clnt);
    if (retval_1 != RPC_SUCCESS) {
        clnt_perror (clnt, "call failed");
        clnt_destroy(clnt);
        return -1;
    }

    clnt_destroy(clnt);
    return result_1;
}

int get_value (char *key, char *value1, int *N_value2, float *V_value2, struct Paquete *value3)
{
    CLIENT *clnt;
    char *ip_tuplas;
    struct key_arg arg;
    struct get_value_ret result;
    enum clnt_stat retval_1;

    // Obtenemos la IP del servidor
    ip_tuplas = getenv("IP_TUPLAS");
    if (NULL == ip_tuplas) {
        return -1;
    }

    // Creamos la conexion RPC
    clnt = clnt_create(ip_tuplas, CLAVES, CLAVES_VERSION, "tcp");
	if (NULL == clnt) { 
        clnt_pcreateerror(ip_tuplas);
        return -1;
    }

    strncpy(arg.key, key, 256);

    // Realizamos la llamada RPC
    retval_1 = rpc_get_value_1(arg, &result, clnt);
    if (retval_1 != RPC_SUCCESS) {
        clnt_perror (clnt, "call failed");
        clnt_destroy(clnt);
        return -1;
    }

    // Si el servidor encontro la clave copiamos los datos fuera 
    if (0 == result.status) {
        strncpy(value1, result.value1, 256);
        *N_value2 = result.N_value2;
        for (int i = 0; i < result.N_value2; i++) {
            V_value2[i] = result.V_value2[i];
        }
        value3->x = result.value3.x;
        value3->y = result.value3.y;
        value3->z = result.value3.z;
    }

    clnt_destroy(clnt);
    return result.status;

}

int modify_value (char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3)
{
    CLIENT *clnt;
	char *ip_tuplas;
	int result_1;
    enum clnt_stat retval_1;
    struct set_value_arg arg;

    // Obtenemos la IP del servidor
    ip_tuplas = getenv("IP_TUPLAS");
    if (NULL == ip_tuplas) {
        return -1;
    }

    // Creamos la conexion RPC
    clnt = clnt_create(ip_tuplas, CLAVES, CLAVES_VERSION, "tcp");
	if (NULL == clnt) { 
        clnt_pcreateerror(ip_tuplas);
        return -1;
    }
    
    // Preparamos los datos para el envio
    strncpy(arg.key, key, 256);
    strncpy(arg.value1, value1, 256);
    arg.N_value2 = N_value2;
    for (int i = 0; i < N_value2; i++) {
        arg.V_value2[i] = V_value2[i];
    }
    arg.value3.x = value3.x;
    arg.value3.y = value3.y;
    arg.value3.z = value3.z;

    // Realizamos la llamada RPC
    retval_1 = rpc_modify_value_1(arg, &result_1, clnt);
    if (retval_1 != RPC_SUCCESS) {
        clnt_perror (clnt, "call failed");
        clnt_destroy(clnt);
        return -1;
    }

    clnt_destroy(clnt);
    return result_1;
}

int delete_key (char *key)
{
    CLIENT *clnt;
    char *ip_tuplas;
    int result_1;
    enum clnt_stat retval_1;
    struct key_arg arg;

    // Obtenemos la IP del servidor
    ip_tuplas = getenv("IP_TUPLAS");
    if (NULL == ip_tuplas) {
        return -1;
    }

    // Creamos la conexion RPC
    clnt = clnt_create(ip_tuplas, CLAVES, CLAVES_VERSION, "tcp");
	if (NULL == clnt) { 
        clnt_pcreateerror(ip_tuplas);
        return -1;
    }

    strncpy(arg.key, key, 256);

    // Realizamos la llamada RPC
    retval_1 = rpc_delete_key_1(arg, &result_1, clnt);
    if (retval_1 != RPC_SUCCESS) {
        clnt_perror (clnt, "call failed");
        clnt_destroy(clnt);
        return -1;
    }

    clnt_destroy(clnt);
    return result_1;
}

int exist (char *key)
{
    CLIENT *clnt;
    char *ip_tuplas;
    int result_1;
    enum clnt_stat retval_1;
    struct key_arg arg;

    // Obtenemos la IP del servidor
    ip_tuplas = getenv("IP_TUPLAS");
    if (NULL == ip_tuplas) {
        return -1;
    }

    // Creamos la conexion RPC
    clnt = clnt_create(ip_tuplas, CLAVES, CLAVES_VERSION, "tcp");
    if (NULL == clnt) { 
        clnt_pcreateerror(ip_tuplas);
        return -1;
    }

    strncpy(arg.key, key, 256);

    // Realizamos la llamada RPC
    retval_1 = rpc_exist_1(arg, &result_1, clnt);
    if (retval_1 != RPC_SUCCESS) {
        clnt_perror (clnt, "call failed");
        clnt_destroy(clnt);
        return -1;
    }

    clnt_destroy(clnt);
    return result_1;

}