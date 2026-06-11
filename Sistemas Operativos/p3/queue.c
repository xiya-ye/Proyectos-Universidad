#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <pthread.h>  


#include "queue.h"


/*Crear la cola*/
int queue_init(struct queue *q, int size) {
    /*Si el taamaño es incorrecto*/
    if (size <= 0) return -1;


    /*Reservar memoria dináimica*/
    q -> buffer = malloc(sizeof(struct element) * size);
    /*En caso de fallo devolver error*/
    if (!q->buffer) return -1;


    /*Inicializar las variables*/
    q->tamano = size;
    q->num_elementos = 0;
    q->elem_leer = 0;
    q->elem_escribir = 0;


    /*Inicializar los mecanismos de sincronización*/
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->not_empty, NULL);
    pthread_cond_init(&q->not_full, NULL);


    return 0;
}


/*Insertar elementos en la cola*/
int queue_put(struct queue *q,struct element* x) {
    /*Verficaar que el puntero no sea nulo*/
    if (!x) {
        fprintf(stderr, "[ERROR][queue] There was an error while using queue with id %d\n", x->id_belt);
        return -1;
    }


    /*Bloquear el acceso a la cola*/
    pthread_mutex_lock(&q->mutex);


    /*Esperar si la cola esta llena hasta que haya espacio*/
    while (q->num_elementos == q->tamano) {
        pthread_cond_wait(&q->not_full, &q->mutex);
    }


    /*Escribir el elemnto y actulaizar las variables*/
    q->buffer[q->elem_escribir] = *x;
    printf("[OK][queue] Introduced element with id %d in belt %d.\n", x->num_edition, x->id_belt);
    q->elem_escribir = (q->elem_escribir + 1) % q->tamano;
    q->num_elementos++;
    
    /*Indicar que la cola y ano esta  vacía y desbloquearla*/
    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->mutex);


    return 0;
}


/*Extraer eelembto de la cola*/
struct element* queue_get(struct queue *q) {
    /*Bloquear el acceso a la cola*/
    pthread_mutex_lock(&q->mutex);


    /*Esperar si la cola esta vacía*/
    while (q->num_elementos == 0) {
        pthread_cond_wait(&q->not_empty, &q->mutex);
    }


    /*Liberar memoria para copiar el eleemnto que se va a devolver*/
    struct element* e = malloc(sizeof(struct element));
    /*En caso de error*/
    if (!e) {
        pthread_mutex_unlock(&q->mutex);
        fprintf(stderr, "[ERROR][queue] There was an error while using queue with id %d\n", q->buffer[q->elem_leer].id_belt);
        return NULL;
    }


    /*Copir el elemnto y actualizar las variables*/
    *e = q->buffer[q->elem_leer];
    printf("[OK][queue] Obtained element with id %d in belt %d.\n", e->num_edition, e->id_belt);
    q->elem_leer = (q->elem_leer + 1) % q->tamano;
    q->num_elementos--;

    /*Indicar que hya hueco en la cola y desbloquear el mutex*/
    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->mutex);


    return e;
}


/*Comprobra si la cola esta vacía*/
int queue_empty(struct queue *q) {
    /*Bloquear el acceso a la cola*/
    pthread_mutex_lock(&q->mutex);
    /*DEvolver si esta vacía o no*/
    int vacia = (q->num_elementos == 0);
    pthread_mutex_unlock(&q->mutex);
    return vacia;
}


/*Comporbar si la cola esta llena o no*/
int queue_full(struct queue *q) {
    /*Bloquear la cola*/
    pthread_mutex_lock(&q->mutex);
    /*Devolver si esta llena o no*/
    int llena = (q->num_elementos == q->tamano);
    pthread_mutex_unlock(&q->mutex);
    return llena;
}


/*Destruir la cola*/
int queue_destroy(struct queue *q) {
    /*Liberar la memoria del buffer*/
    if (q->buffer) {
        free(q->buffer);
        q->buffer = NULL;
    }


    /*Reiniciar las variables*/
    q->tamano = 0;
    q->num_elementos = 0;
    q->elem_leer = 0;
    q->elem_escribir = 0;


    /*Liberar recursos de sincronización de la cola*/
    pthread_mutex_destroy(&q->mutex);
    pthread_cond_destroy(&q->not_empty);
    pthread_cond_destroy(&q->not_full);


    return 0;
}
