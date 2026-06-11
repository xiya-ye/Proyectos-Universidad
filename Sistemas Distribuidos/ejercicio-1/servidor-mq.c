#include "claves.h"
#include "mensaje.h"
#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *th_function(void *arg) {
    mqd_t q_cliente;  // cola de mensajes del cliente


    // Pasamos la petición a la estructura peticion
    struct peticion pet = *(struct peticion *)arg;
    free(arg);

    struct respuesta res;

    // Bloqueamos 
    pthread_mutex_lock(&mutex);

    // Ejecutamos dependiendo de la operación solicitada
    switch (pet.op) {
        case OP_DESTROY:
            res.resultado = destroy();
            break;
        case OP_SET_VALUE:
            res.resultado = set_value(pet.key, pet.value1, pet.N_value2, pet.V_value2, pet.value3);
            break;
        case OP_GET_VALUE:
            res.resultado = get_value(pet.key, res.value1, &res.N_value2, res.V_value2, &res.value3);
            break;
        case OP_MODIFY_VALUE:
            res.resultado = modify_value(pet.key, pet.value1, pet.N_value2, pet.V_value2, pet.value3);
            break;
        case OP_DELETE_KEY:
            res.resultado = delete_key(pet.key);
            break;
        case OP_EXIST:
            res.resultado = exist(pet.key);
            break;
        default:    // Operación no reconocida
            res.resultado = -1;
            break;
    }

    // Desbloqueamos
    pthread_mutex_unlock(&mutex);

    // Enviamos la respuesta al cliente, abriendo previamente su cola
    q_cliente = mq_open(pet.nombre_cola, O_WRONLY);
    if (-1 == q_cliente) {
        perror("mq_open: ");
        pthread_exit(NULL);
    }
    mq_send(q_cliente, (const char *)&res, sizeof(struct respuesta), 0);
    mq_close(q_cliente);
    pthread_exit(NULL);
}

int main ( int argc, char *argv[] )
{
    mqd_t q_servidor; /* cola de mensajes del servidor */

    struct mq_attr attr;
    int ret;

    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(struct peticion);

    // Borramos la cola del servidor en el caso de que se quedó colgada en la anterior ejecución
    mq_unlink(COLA_SERVIDOR);

    q_servidor = mq_open(COLA_SERVIDOR, O_CREAT|O_RDONLY, 0700, &attr);
    if (-1 == q_servidor) {
        perror("mq_open: ");
        exit(-1);
    }

    while(1) {  // bucle infinito para atender peticiones
        // Reservamos memoria para la petición
        struct peticion *pet = malloc(sizeof(struct peticion));
        if (NULL == pet) {
            perror("malloc: ");
            continue; // En caso de error, continuamos esperando nuevas peticiones
        }
        
        // El servidor espera a recibir una petición del cliente
        ret = mq_receive(q_servidor, (char *)pet, sizeof(struct peticion), NULL);
        if (-1 == ret) {
            perror("mq_receive: ");
            free(pet);
            continue; // En caso de error, continuamos esperando nuevas peticiones
        }

        // Procesamos la petición recibida con hilos
        pthread_t th;
        pthread_attr_t attr_hilo;
        pthread_attr_init(&attr_hilo);
        pthread_attr_setdetachstate(&attr_hilo, PTHREAD_CREATE_DETACHED);

        ret = pthread_create(&th, &attr_hilo, th_function, (void *)pet);
        if (0 != ret) {
            perror("pthread_create: ");
            free(pet);
            continue; // En caso de error, continuamos esperando nuevas peticiones
        }

    }

    return 0;
}