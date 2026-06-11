#ifndef HEADER_FILE
#define HEADER_FILE

#include <pthread.h>

struct element {
  int num_edition;
  int id_belt;
  int last;
};

//Estructura para la cola
struct queue {
  struct element *buffer;
  int tamano;
  int num_elementos;
  int elem_leer;
  int elem_escribir;
  pthread_mutex_t mutex;
  pthread_cond_t not_empty;
  pthread_cond_t not_full;
};

int queue_init (struct queue *q, int size);
int queue_destroy (struct queue *q);
int queue_put (struct queue *q, struct element* elem);
struct element * queue_get(struct queue * q);
int queue_empty (struct queue *q);
int queue_full(struct queue *q);

#endif
