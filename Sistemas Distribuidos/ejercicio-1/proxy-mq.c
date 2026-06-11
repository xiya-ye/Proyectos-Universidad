#include "claves.h"
#include "mensaje.h"
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

// Función auxiliar para la comunicación de las colas
int enviar_recibir (struct peticion *pet, struct respuesta *res) {
    mqd_t q_servidor; // cola de mensajes del proceso servidor
    mqd_t q_cliente;  // cola de mensajes para el proceso cliente
    struct mq_attr attr;
    int ret;
    char queuename[MAX_SIZE];

    attr.mq_maxmsg = 1;
    attr.mq_msgsize = sizeof(struct respuesta);
    sprintf(queuename, "/CLIENTE-%d", getpid()) ; // generar un nombre único por cliente
    strcpy(pet->nombre_cola, queuename);

    // Abrimos la cola del cliente para lectura
    q_cliente  = mq_open(queuename, O_CREAT|O_RDONLY, 0700, &attr);
    if (-1 == q_cliente) {
        perror("mq_open: ");
        return -2;
    }

    // Abrimos la cola del servidor para escritura
    q_servidor = mq_open(COLA_SERVIDOR, O_WRONLY);
    if (-1 == q_servidor) {
        perror("mq_open: ");
        mq_close(q_cliente);
        mq_unlink(queuename);
        return -2;
    }

    // Enviamos la petición al servidor
    ret = mq_send(q_servidor, (const char *)pet, sizeof(struct peticion), 0);
    if (-1 == ret) {
        perror("mq_send: ");
        mq_close(q_cliente);
        mq_unlink(queuename);
        mq_close(q_servidor);
        return -2;
    }

    // Recibimos la respuesta del servidor
    ret = mq_receive(q_cliente, (char *)res, sizeof(struct respuesta), 0);
    if (-1 == ret) {
        perror("mq_receive: ");
        mq_close(q_cliente);
        mq_unlink(queuename);
        mq_close(q_servidor);
        return -2;
    }

    mq_close(q_servidor);
    mq_close(q_cliente);
    mq_unlink(queuename);
    return 0;
}

int destroy(void) {
    struct peticion pet;
    struct respuesta res;

    // Formamos la petición
    pet.op = OP_DESTROY;

    int ret = enviar_recibir(&pet, &res);
    if (-2 == ret) {
        return -2;
    }

    return res.resultado;    
}

int set_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {
    // Validamos los parámetros de entrada
    // Comprueba la longitud de key
    if (strlen(key) > 255) {
        printf("Error: key is too long\n");
        return -1;
    }

    // Comprueba la longitud de value1
    if (strlen(value1) > 255) {
        printf("Error: value1 is too long\n");
        return -1;
    }

    //Comprueba el rango de N_value2
    if (N_value2 < 1) {
        printf("Error: N_value2 < 1\n");
        return -1;
    }

    if (N_value2 > 32) {
        printf("Error: N_value2 > 32\n");
        return -1;
    }
    
    struct peticion pet;
    struct respuesta res;

    // Formamos la petición
    pet.op = OP_SET_VALUE;
    strcpy(pet.key, key);
    strcpy(pet.value1, value1);
    pet.N_value2 = N_value2;
    pet.value3 = value3;
    for (int i = 0; i < N_value2; i++) {
        pet.V_value2[i] = V_value2[i];
    }

    int ret = enviar_recibir(&pet, &res);
    if (-2 == ret) {
        return -2;
    }

    return res.resultado;
}

int get_value(char *key, char *value1, int *N_value2, float *V_value2, struct Paquete *value3) {
    // Validamos los parámetros de entrada
    // Comprueba la longitud de key
    if (strlen(key) > 255) {
        printf("Error: key is too long\n");
        return -1;
    }
    
    struct peticion pet;
    struct respuesta res;

    // Formamos la petición
    pet.op = OP_GET_VALUE;
    strcpy(pet.key, key);

    int ret = enviar_recibir(&pet, &res);
    if (-2 == ret) {
        return -2;
    }

    // Extraemos los datos de la respuesta
    if (0 == res.resultado) {
        strcpy(value1, res.value1);
        *N_value2 = res.N_value2;
        for (int i = 0; i < res.N_value2; i++) {
            V_value2[i] = res.V_value2[i];
        }
        *value3 = res.value3;
    }

    return res.resultado;
}

int modify_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {
    // Validamos los parámetros de entrada
    // Comprueba la longitud de key
    if (strlen(key) > 255) {
        printf("Error: key is too long\n");
        return -1;
    }

    // Comprueba la longitud de value1
    if (strlen(value1) > 255) {
        printf("Error: value1 is too long\n");
        return -1;
    }

    //Comprueba el rango de N_value2
    if (N_value2 < 1) {
        printf("Error: N_value2 < 1\n");
        return -1;
    }

    if (N_value2 > 32) {
        printf("Error: N_value2 > 32\n");
        return -1;
    }
    
    struct peticion pet;
    struct respuesta res;

    // Formamos la petición
    pet.op = OP_MODIFY_VALUE;
    strcpy(pet.key, key);
    strcpy(pet.value1, value1);
    pet.N_value2 = N_value2;
    pet.value3 = value3;
    for (int i = 0; i < N_value2; i++) {
        pet.V_value2[i] = V_value2[i];
    }

    int ret = enviar_recibir(&pet, &res);
    if (-2 == ret) {
        return -2;
    }

    return res.resultado;
}

int delete_key(char *key) {
    // Validamos los parámetros de entrada
    // Comprueba la longitud de key
    if (strlen(key) > 255) {
        printf("Error: key is too long\n");
        return -1;
    }
    
    struct peticion pet;
    struct respuesta res;

    // Formamos la petición
    pet.op = OP_DELETE_KEY;
    strcpy(pet.key, key);

    int ret = enviar_recibir(&pet, &res);
    if (-2 == ret) {
        return -2;
    }

    return res.resultado;
}

int exist(char *key) {
    // Validamos los parámetros de entrada
    // Comprueba la longitud de key
    if (strlen(key) > 255) {
        printf("Error: key is too long\n");
        return -1;
    }

    
    struct peticion pet;
    struct respuesta res;

    // Formamos la petición
    pet.op = OP_EXIST;
    strcpy(pet.key, key);
    
    int ret = enviar_recibir(&pet, &res);
    if (-2 == ret) {
        return -2;
    }

    return res.resultado;
}