#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#define MAX_ALUMNOS 100 /* limite de alumnos que el programa va a leer */


struct alumno{
	char nombre[50];
	int nota;
	int convocatoria;
};


int main(int argc, char *argv[]){
	/* comprobar que la función recibe todos los argumentos nencesarios */
	if (argc != 4) {
	    printf ("Error: número de argumentos incorrecto\n");
	    exit (-1);
	}


	/* definimos los ids de los dos archivos a leer */
	int fd1;	/* fichero 1 */
	int fd2;	/* fichero 2 */
	int fd_sal; 	/* fichero salida */

	/* definir el array donde vamos a guardar los alumnos y el contador de alumnos */
	struct alumno array[MAX_ALUMNOS];

	/* abrir el archivo 1 para leerlo */
	fd1 = open(argv[1], O_RDONLY);
	/* comprobar que se ha abierto el archivo correctamente */
	if(fd1 < 0) {
		perror("open ");
		exit (-1);
	}

	/* leemos los alumnos del fichero 1 y los añadimos al array*/
	int leidos1;
	int total_leidos = 0;
	while ((leidos1 = read(fd1, &array[total_leidos], sizeof(struct alumno))) > 0) {
		/* si supera los 100 alumnos devuelve -1 */
		if (total_leidos >= MAX_ALUMNOS) {
			close (fd1);
			printf ("Error: se ha superado el número max de alumnos\n");
			exit (-1);
			}
		total_leidos++;
	}
	
	
	/* en caso de que haya error al leer el fichero devuelve -1 */
	if (leidos1 < 0) {
		close(fd1);
		perror("read: ");
		exit (-1);
	}
	

	/* abrir el archivo 2 para leerlo */
	fd2 = open(argv[2], O_RDONLY);
	/* comprobar que se ha abierto el fichero correctamente */
	if (fd2 < 0) {
		close (fd1);
		perror("open: ");
		exit (-1);
	}

	/* leemos los alumnos del fichero 2 y los añadimos al array*/
	int leidos2;
	while ((leidos2 = read(fd2, &array[total_leidos], sizeof(struct alumno))) > 0) {
		/* si supera los 100 alumnos devuelve -1 */
		if (total_leidos >= 100) {
			close (fd1);	close (fd2);
			printf ("Error: se ha superado el número max de alumnos\n");
			exit (-1);
			}
			total_leidos++;
		}
	
	/* en caso de que haya error al leer el fichero devuelve -1 */
	if (leidos2 < 0) {
		close (fd1);	close (fd2);
		perror("read: ");
		exit (-1);
	}
	
	
	/* ordernar los alumnos mediante el metodo de la burbuja */
	struct alumno aux;

	/* bucle recorre la lista */
	for (int i = 0; i < total_leidos -1; i++){
		/* segundo bucle compara los dos alumnos consecutivos */
		for (int j = 0; j < total_leidos-i-1; j++){
			if (array[j].nota > array[j+1].nota){
				aux = array[j];
				array[j] = array[j+1];
				array[j+1] = aux;
			}
		}
	}

	
	/* creamos el nuevo fichero donde escribir los datos */
	fd_sal = open(argv[3], O_WRONLY | O_CREAT, 0644);

	/* comprobar que se ha creado corrctamente */
	if (fd_sal < 0) {
		perror("open: ");
		exit(-1);
	}

	/* si esta bien, se escriben los alumnos en el archivo */
	if (write(fd_sal, array, total_leidos*sizeof(struct alumno)) == -1){
		perror("write: ");
		exit(-1);

	}
	
	/* cerramos los ficheros que no vamos a usar */
	close (fd1);	close (fd2);	close(fd_sal);
	

	/* crear fichero estadistica.csv*/
	int fd_estd = open("estadisticas.csv", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_estd < 0) {
		perror ("opne: ");
		exit (-1);
	}

	/* usamos un bucle para ir hacer conteo de las notas de cada tipo */
	int M = 0, S = 0, N = 0, A = 0, F = 0;
	for (int i = 0; i < total_leidos;i++) {
		if (array[i].nota == 10) {M++;}
		else if (array[i].nota == 9) {S++;}
		else if (array[i].nota == 8) {N++;}
		else if (array[i].nota == 7) {N++;}
		else if (array[i].nota == 6) {A++;}
		else if (array[i].nota == 5) {A++;}
		else if (array[i].nota == 4) {F++;}
		else if (array[i].nota == 3) {F++;}
		else if (array[i].nota == 2) {F++;}
		else if (array[i].nota == 1) {F++;}
		else if (array[i].nota == 0) {F++;}
		else {
			printf ("Error: nota inválida\n");
		}
	}
	
	
	/* imprimimos las estadisticas por pantalla */
	printf("M;%d;%.2f%%\n", M, (M * 100.0) / total_leidos);
	printf("S;%d;%.2f%%\n", S, (S * 100.0) / total_leidos);
	printf("N;%d;%.2f%%\n", N, (N * 100.0) / total_leidos);
	printf("A;%d;%.2f%%\n", A, (A * 100.0) / total_leidos);
	printf("F;%d;%.2f%%\n", F, (F * 100.0) / total_leidos);
	
	/* escribimos las estadisticas en el fichero de estadisticas.csv para cada tipo */
	int fd;
	char line[100];
	sprintf(line,"M;%d;%.2f%%\n", M, (M * 100.0) / total_leidos);
	fd = write(fd_estd, line, strlen(line));
	if (fd < 0) {
		perror ("write: ");
		exit (-1);
	}
	
	sprintf(line, "S;%d;%.2f%%\n", S, (S * 100.0) / total_leidos);
	fd = write(fd_estd, line, strlen(line));
	if (fd < 0) {
		perror ("write: ");
		exit (-1);
	}
	
	sprintf(line, "N;%d;%.2f%%\n", N, (N * 100.0) / total_leidos);
	fd = write(fd_estd, line, strlen(line));
	if (fd < 0) {
		perror ("write: ");
		exit (-1);
	}
	
	sprintf(line, "A;%d;%.2f%%\n", A, (A * 100.0) / total_leidos);
	fd = write(fd_estd, line, strlen(line));
	if (fd < 0) {
		perror ("write: ");
		exit (-1);
	}
	
	sprintf(line, "F;%d;%.2f%%\n", F, (F * 100.0) / total_leidos);
	fd = write(fd_estd, line, strlen(line));
	if (fd < 0) {
		perror ("write: ");
		exit (-1);
	}

	/* cerramos fichero de estadisticas.csv */
	close(fd_estd);
	return 0;


}

