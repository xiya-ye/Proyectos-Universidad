/*
 *
 * factory_manager.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <pthread.h>

/* importamos la función de process_manager del archivo process_manager.c */
extern void *process_manager(void *params);
void *process_manager(void *params);

/* creamos la estructura del parámetro que se pasa al hilo para process_manager */
struct params_process {
	int id;
	int belt_size;
	int items_to_produce;
	sem_t *semaforo_ini;
};


int main (int argc, const char * argv[] ){

	int fd;
	int leidos;
	int max_cintas, es_primero=1;
	int max_buffer = 2;
	char *chars_leidos = malloc(max_buffer * sizeof(char));
	int *num_leidos = malloc(max_buffer * sizeof(int));
	int pos = 0, i = 0;
	
	
	/* comprueba que el número de argumentos sea correcto */
	if (argc != 2)
	{
		fprintf (stderr, "[ERROR][factory_manager] Process_manager with id 0 has finished with errors.\n");
		exit (-1);
	}
	
	
	/* abrir el fichero de entrada */
	fd = open (argv[1], O_RDONLY, 0644);
	if (fd < 0) 
	{
		fprintf (stderr, "[ERROR][factory_manager] Invalid file.\n");
		exit (-1);
	}
	
	/* leer el fichero caracter a caracter */
	while ((leidos = read (fd, &chars_leidos[i], 1)) > 0)
	{
		if (chars_leidos[i] == ' ')
		{
			/* comprobar que hay chars */
			if (i > 0)
			{
				/* expandimos el buffer de num_leidos y chars_leidos cuando están llenos */
				if (pos >= max_buffer-1)
				{
					max_buffer = max_buffer + 2;
					char *temp_chars = realloc(chars_leidos, max_buffer * sizeof(char));
					if (temp_chars == NULL) 
					{
					    fprintf (stderr, "[ERROR][factory_manager] Process_manager with id 0 has finished with errors.\n");
					    free (chars_leidos);
					    free (num_leidos);
					    close (fd);
					    exit (-1);
					}
					chars_leidos = temp_chars;
					
					int *temp_nums = realloc(num_leidos, max_buffer * sizeof(int));
					if (temp_nums == NULL) 
					{
					    fprintf (stderr, "[ERROR][factory_manager] Process_manager with id 0 has finished with errors.\n");
					    free (chars_leidos);
					    free (num_leidos);
					    close (fd);
					    exit (-1);
					}
					num_leidos = temp_nums;
				}
				
				/* cuando es el primero número, lo guardamos en una variable aparte */
				if (es_primero == 1)
				{
					es_primero = 0;
					chars_leidos[i] = '\0';
					max_cintas = atoi(chars_leidos);
					
					/* en el caso en que el número max de cintas es 0 */
					if (max_cintas <= 0)
					{
						fprintf (stderr, "[ERROR][factory_manager] Invalid file.\n");
						close (fd);
						exit (-1);
					}
					i = 0;
				}
				else
				{
					/* añadir el número al array */
					chars_leidos[i] = '\0';
					num_leidos[pos] = atoi(chars_leidos);
					if (num_leidos[pos] <= 0)
					{
						fprintf (stderr, "[ERROR][factory_manager] Invalid file.\n");
						close (fd);
						exit (-1);
					}
					pos++;
					i = 0;
				}
				
				/* si el número de process que hay es mayor que el número máximo de cintas */
				if (pos >= max_cintas*3)
				{
					fprintf (stderr, "[ERROR][factory_manager] Invalid file.\n");
					close (fd);
					exit (-1);
				}	
			}
		}
		else {i++;}
	}
	
	/* en el caso en que después del último número no haya un espacio en blanco */
	if (i > 0)
	{
		if (es_primero == 1)
		{
			es_primero = 0;
			chars_leidos[i] = '\0';
			max_cintas = atoi(chars_leidos);
			
			/* en el caso en que el número max de cintas es 0 */
			if (max_cintas <= 0)
			{
				fprintf (stderr, "[ERROR][factory_manager] Invalid file.\n");
				close (fd);
				exit (-1);
			}
		}
		else
		{
			chars_leidos[i] = '\0';
			num_leidos[pos] = atoi(chars_leidos);
		}
	}
	
	/* manejo de error en la lectura */
	if (leidos < 0)
	{
		fprintf (stderr, "[ERROR][factory_manager] Process_manager with id 0 has finished with errors.\n");
		close (fd);
		exit (-1);
	}
	
	close (fd);
	
	/* FICHERO LEIDO CORRECTAMENTE */
	
	/* crear estructuras de sincronización con semáforos */
	int j = 0;
	sem_t *sem_ini = malloc(max_cintas * sizeof(sem_t));
	
	if (sem_ini == NULL)
	{
		printf ("[ERROR][factory_manager] Process_manager with id 0 has finished with errors.\n");
		exit (-1);
	}
	
	while (j < max_cintas && (j*3+2) < (pos+1))
	{
		sem_init(&sem_ini[j], 0, 0);
		j++;
	}
	
	/* crear threads */
	pthread_t tid[max_cintas];
	struct params_process params[max_cintas];
	
	int hilos_creados = 0;
	while (hilos_creados < max_cintas && (hilos_creados*3+2) < (pos+1))
	//for (int i=0; i<max_cintas; i++)
	{
		params[hilos_creados].id = num_leidos[hilos_creados*3];
		params[hilos_creados].belt_size = num_leidos[hilos_creados*3 + 1];
		params[hilos_creados].items_to_produce = num_leidos[hilos_creados*3 + 2];
		params[hilos_creados].semaforo_ini = &sem_ini[hilos_creados];
		
		int ret = pthread_create (&(tid[hilos_creados]), NULL, process_manager, &params[hilos_creados]);
		if (ret < 0)
		{
			fprintf (stderr, "[ERROR][factory_manager] Process_manager with id %d not created.\n", params[hilos_creados].id);
			exit (-1);
		}
		printf("[OK][factory_manager] Process_manager with id %d has been created.\n", params[hilos_creados].id);
		hilos_creados++;
	}
	
	/* controlar orden de los threads creados */
	for (int i=0; i<hilos_creados; i++)
	{
		sem_post (&sem_ini[i]);
		void * process_result;
		pthread_join (tid[i], &process_result);
		if ((intptr_t)process_result == -1)
		{
			printf ("[ERROR][factory_manager] Process_manager with id %d has finished with errors.\n", params[i].id);
			exit (-1);
		}
		printf ("[OK][factory_manager] Process_manager with id %d has finished.\n", params[i].id);
	}
	
	/* liberar recursos */
	int n=0;
	while (n < max_cintas && (n*3+2) < (pos+1))
	{
		sem_destroy (&sem_ini[n]);
		n++;
	}
	
	free (sem_ini);
	free(chars_leidos);
	free(num_leidos);
	
	printf("[OK][factory_manager] Finishing.\n");
	
	return 0;
}


