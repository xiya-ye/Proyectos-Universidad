#ifndef MENSAJE_H
#define MENSAJE_H

#include "claves.h"

#define MAX_SIZE 256
#define COLA_SERVIDOR "/COLA_SERVIDOR"

// Establecemos los código de operación para cada función
#define OP_DESTROY 0
#define OP_SET_VALUE 1
#define OP_GET_VALUE 2
#define OP_MODIFY_VALUE 3
#define OP_DELETE_KEY 4
#define OP_EXIST 5

// Estructura para el mensaje que se enviará al servidor
struct peticion {
    int op;
    char nombre_cola[MAX_SIZE];

    // Parámetros para la función (para set_value y modify_value)
    char key[MAX_SIZE];
    char value1[MAX_SIZE];
    int N_value2;
    float V_value2[32];
    struct Paquete value3;
};

// Estructura para la respuesta del servidor
struct respuesta {
    int resultado;      // 0 para éxito, -1 para error

    // Datos que devuelve (para get_value)
    char value1[MAX_SIZE];
    int N_value2;
    float V_value2[32];
    struct Paquete value3;
};

#endif // MENSAJE_H