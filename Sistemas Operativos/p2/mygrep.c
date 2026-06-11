#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>    
#include <unistd.h>


#define BUFFER_SIZE 1024

int main(int argc, char ** argv) {
    /*Comprobar que recibe los argumentos correctos*/
	if (argc != 3) {
    		printf("Usage: %s <ruta_fichero> <cadena_busqueda>\n", argv[0]);
    	return -1;
	}

	/*Abrimos el archivo para leerlo */
	int fd;
	fd = open(argv[1], O_RDONLY, 0644);
	if(fd == -1){ 
        /*En caso de error al abrir el archivo*/
    		perror("Error al abrir el archivo");
    	exit(-1);

	}
	/*Definición de variables para lectura del archivo*/
	char buffer[BUFFER_SIZE];
	int bytes_rd;
	int encontrada = 0;
	int buffer_size = 1024;
	char *linea = malloc(buffer_size);

	if (linea == NULL) { /* error */
		perror("malloc: ");
		close(fd);
	}
	
	int pos = 0;/*Posición dentro de la linea*/


	/*Leer el archivo*/
	while ((bytes_rd = read(fd, buffer, sizeof(buffer))) >0){
        /*Recorrer los bytes leídos carácter por carácter hasta fin de la línea*/
    	for(int i = 0; i < bytes_rd; i++){
        	if(buffer[i] == '\n'){
            	/*Añadir \0 al final de la línea*/
            	linea[pos] = '\0';
            	/*Comprobar si esta la cadena*/
            	if (strstr(linea, argv[2]) != NULL){
                	/*Escribir la línea de salida si es encontrada*/
                	write(STDOUT_FILENO, linea, strlen(linea));
              	write(STDOUT_FILENO, "\n", 1);
                /*Marcar como encontrada*/
                	encontrada = 1;
            	}
            	/*Reiniciar pos para la siguiente línea*/
            	pos = 0;

        	} else{
        		/* expandimos buffer cuando esta lleno */
        		if (pos >= buffer_size-1)
        		{
        			buffer_size = buffer_size * 2;
        			char * linea_temporal = realloc(linea, buffer_size);
        			if (linea_temporal == NULL) 
        			{
        				perror("realloc:");
        				free(linea);
        				exit(-1);
        			}
        			linea = linea_temporal;
        		}
        	
            	/*Almacenar cada carácter en la linea y aumentar una posición*/
                	linea[pos++] = buffer[i];
        	}
     	}
    	}
	/*Comprobar si se ha encontrado*/
	if(encontrada == 0){
    	printf("%s not found.\n ", argv[2]);
	}

    /*Terminar el programa*/
	close(fd);

	return 0;

} 
