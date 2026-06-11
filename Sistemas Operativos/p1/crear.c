#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int fd;
  
	/* en caso de que el numero de argumentos de entradas no es 3 devuelve -1 */
	if (argc != 3) {
		printf ("Error: número de argumentos incorrecto\n");
		exit (-1);
	}
	
	/* comprobamos que los permisos estan en octal */
	int i = 0;
	while (argv[2][i]) {
		if (strchr("01234567", argv[2][i]) == NULL) {
			printf ("Error: formato de permisos incorrecto, debe ser un número en octal\n");
			exit (-1);
		}
		i++;
	}
	
	
  
	/* guardamos la mascara por defecto del sistema */
	mode_t mask_sys = umask(0);
  
	/* le asignamos a la variable mode los permisos del archivo en octal */
	mode_t mode;
	sscanf (argv[2], "%o", &mode);
 
	/* crea un nuevo fichero con los permisos pedidos */
	fd = creat(argv[1], mode);
  
	/* en caso de error al crear el fichero devuelve -1 */
	if (fd < 0) {
		perror ("creat: ");
    		exit (-1);
  	}
  
  	/* restaurar la mascara por defecto del sistema operativo */
  	umask(mask_sys);
  
  	/* aseguramos que los permisos sean el mismo que el que se nos pide */
  	chmod(argv[1], mode);

  	return 0;
}

