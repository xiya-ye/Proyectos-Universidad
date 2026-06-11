/*
 *
 * process_manager.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>
#include <pthread.h>
#include "queue.h"
#include <semaphore.h>

#define NUM_THREADS 2


/* Paso datos a los hilos */
struct thread_data {
	int id;
	int items_to_process;
	struct queue *q; /* cola compartida */
};

struct params_process {
	int id;
	int belt_size;
	int items_to_produce;
};


/* Funcion para producer */
void * producer(void*arg) {
	struct thread_data * data = (struct thread_data*)arg; /* Para acceder los datos pasados al hilo */
	int id = data->id;
	int items_to_produce = data->items_to_process;
	struct queue *q = data->q;
	struct element e;
	
	for (int i = 0; i < items_to_produce; i++){
		e.num_edition = i;
		e.id_belt = id;
		e.last = (i == items_to_produce - 1);
		
		
		/* Insertar en la cola */
		if (queue_put(q, &e) != 0) {
			fprintf(stderr, "[ERROR][process_manager] There was an error executing process_manager with id %d.\n", id);
		}
		e.num_edition++;	
	}
	
	pthread_exit(NULL);
}

/* Funcion para consumer */
void * consumer(void*arg) {
	struct thread_data * data = (struct thread_data*)arg; /* Para acceder los datos pasados al hilo */
	int id = data->id;
	int items_to_produce = data->items_to_process;
	struct queue *q = data->q;
	
	for (int i = 0; i < items_to_produce; i++){
		struct element *e = queue_get(q);
		
		/* Sacar de la cola */
		if (!e) {
			fprintf(stderr, "[ERROR][process_manager] There was an error executing process_manager with id %d.\n", id);
			pthread_exit(NULL);
		}
		
		free(e); /* Liberamos la memoria */
		
	}
	pthread_exit(NULL);
}

/* Función principal */
void * process_manager (void *params){
	struct params_process *p = (struct params_process *) params;
	int id = p->id;
	int belt_size = p->belt_size;
	int items_to_produce = p->items_to_produce;

	if (id < 0 || belt_size <= 0|| items_to_produce <= 0) {
		fprintf(stderr, "[ERROR][process_manager] Arguments not valid.\n");
		pthread_exit((void *) -1);
	}
	printf("[OK][process_manager] Process_manager with id %d waiting to produce %d elements.\n", id, items_to_produce);
	
	
	struct queue q;
	if (queue_init(&q, belt_size) != 0) {
		fprintf(stderr, "[ERROR][process_manager] There was an error executing process_manager with id %d.\n", id);
		pthread_exit((void *) -1);
	}
	printf("[OK][process_manager] Belt with id %d has been created with a maximum of %d elements.\n", id, belt_size);
	
	
	/* Crear hilos productor y consumidor */
	pthread_t threads[NUM_THREADS];
	struct thread_data* data_producer = malloc(sizeof(struct thread_data));
	struct thread_data* data_consumer = malloc(sizeof(struct thread_data));
	
	if (!data_producer || !data_consumer) {
		fprintf(stderr, "[ERROR][process_manager] There was an error executing process_manager with id %d.\n", id);
		queue_destroy(&q);
		pthread_exit((void *) -1);
	}
	
	/* Para el producer */
	data_producer->id = id;
	data_producer->items_to_process = items_to_produce;
	data_producer->q = &q;
	
	/* Para el consumer */
	data_consumer->id = id;
	data_consumer->items_to_process = items_to_produce;
	data_consumer->q = &q;
	
	//Creación de hilo producer
	if (pthread_create(&threads[0], NULL, producer, data_producer) != 0){
		fprintf(stderr, "[ERROR][process_manager] There was an error executing process_manager with id %d.\n", id);
		free(data_producer);
		free(data_consumer);
		queue_destroy(&q);
		pthread_exit((void *) -1);
	}
	
	/* Creación de hilo consumer */
	if (pthread_create(&threads[1], NULL, consumer, data_consumer) != 0){
		pthread_cancel(threads[0]);	/* Cancela hilo productor si ya fue creado */
		pthread_join(threads[0], NULL);	/* Espera que termine */
		fprintf(stderr, "[ERROR][process_manager] There was an error executing process_manager with id %d.\n", id);
		
		free(data_producer);
		free(data_consumer);
		queue_destroy(&q);
		pthread_exit((void *) -1);
	}
	
	/* Esperamos a que ambos hilos terminen */
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}
	
	/* Liberamos memoria */
	free(data_producer);
	free(data_consumer);
	
	/* Verificación de cola vacía */
	if (!queue_empty(&q)) {
		fprintf(stderr, "[ERROR][process_manager] There was an error executing process_manager with id %d.\n", id);
		queue_destroy(&q);
		pthread_exit((void *) -1);
	}
	
	/* Verificación de destruir cola */
	if (queue_destroy(&q) != 0) {
		fprintf(stderr, "[ERROR][process_manager] There was an error executing process_manager with id %d.\n", id);
		pthread_exit((void *) -1);
	}
	
	printf("[OK][process_manager] Process_manager with id %d has produced %d elements.\n", id, items_to_produce);
   	pthread_exit((void *) 0);
	
}


