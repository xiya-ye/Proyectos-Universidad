#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "claves.h"

#define NUM_HILOS 6
#define NUM_OPERACIONES 50

void *tratar_hilo(void *arg) {
    int id_hilo = *(int *) arg;
    free(arg);

    // Usamos el ID del hilo para la clave de cada cliente
    char key[256];
    sprintf(key, "clave-concurrente-%d", id_hilo);

    char v1[256] = "valor1-concurrente";
    float v2[3] = {1.1, 2.2, 3.3};
    struct Paquete v3 = {1, 2, 3};

    printf("[Hilo %d] Iniciando prueba de concurrencia...\n", id_hilo);

    // Probamos a hacer todas las operaciones en bucle
    for (int i = 0; i < NUM_OPERACIONES; i++) {
        set_value(key, v1, 3, v2, v3);
        exist(key);
        modify_value(key, "valor-modificado", 3, v2, v3);

        int n2;
        float res_v2[32];
        struct Paquete res_v3;
        get_value(key, v1, &n2, res_v2, &res_v3);
        
        usleep(10000); 
    }

    delete_key(key);
    printf("[Hilo %d] Prueba finalizada con éxito.\n", id_hilo);
    pthread_exit(NULL);

}


int main(void) {
    int ret;
    pthread_t hilos[NUM_HILOS];

    printf("Iniciando prueba de concurrencia con %d hilos...\n", NUM_HILOS);

    // Creamos los hilos
    for (int i = 0; i < NUM_HILOS; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        ret = pthread_create(&hilos[i], NULL, tratar_hilo, (void *) id);
        if (ret != 0) {
            perror("pthread_create: ");
            exit(-1);
        }
    }

    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    printf("Todas las pruebas de concurrencia se han ejecutado correctamente\n");
    return 0;
}