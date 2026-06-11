#include "claves.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int destroy(void) {
   
   int ret;

   // Eliminamos el directorio ALMACEN y su contenido
   ret = system("rm -fr ./ALMACEN");
   if (ret < 0) {
      perror("system: ");
      return -1;  
   }

   // Creamos el directorio ALMACEN
   ret = mkdir("./ALMACEN", 0777);
   if (ret < 0) {
      perror("mkdir: ");
      return -1;  
   }

   return 0;

}

int set_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {

   FILE *fd;
   char fname[1024];
   int ret;

   // Comprueba la longitud de key
   if (strlen(key) > 255) {
      printf("Error: key is too long\n");
      return -1;
   }

   sprintf(fname, "%s/%s", "./ALMACEN", key); //Concateno

   // Comprueba si fname existe, en ese caso devolver error
   ret = access(fname, F_OK);
   if (0 == ret) {
      printf("ERROR: file %s exists\n", fname);
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

   // Abre el archivo para escritura y escribe los datos
   fd = fopen(fname, "w");
   if (fd == NULL) {
         perror("fopen: ");
         return -1;
   }
   fprintf(fd, "%s\n", value1);
   fprintf(fd, "%d\n", N_value2);
   for (int i = 0; i < N_value2; i++) {
      fprintf(fd, "%f\n", V_value2[i]);
   }

   fprintf(fd, "%d\n", value3.x);
   fprintf(fd, "%d\n", value3.y);
   fprintf(fd, "%d\n", value3.z);

   fclose(fd);
   return 0;
}

int get_value(char *key, char *value1, int *N_value2, float *V_value2, struct Paquete *value3) {
   
   char fname[1024];
   FILE *fd;
   int i;

   // Comprueba la longitud de key
   if (strlen(key) > 255) {
      printf("Error: key is too long\n");
      return -1;
   }

   sprintf(fname, "%s/%s", "./ALMACEN", key); //Concateno

   // Abre el archivo para la lectura
   fd = fopen(fname, "r");
   if (fd == NULL) {
         perror("fopen: ");
         return -1;
   
   }

   // Lee value1
   fgets(value1, 256, fd);

   // Lee N_value2 y sus valores
   fscanf(fd, "%d\n", N_value2);

   for (i = 0; i < *N_value2; i++) {
      fscanf(fd, "%f\n", &V_value2[i]);
   }

   // Lee los valores de value3
   fscanf(fd, "%d\n", &value3 -> x);
   fscanf(fd, "%d\n", &value3 -> y);
   fscanf(fd, "%d\n", &value3-> z);

   fclose(fd);
   return 0;
}

int modify_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {
   FILE *fd;
   char fname[1024];
   int ret;

   // Comprueba la longitud de key
   if (strlen(key) > 255) {
      printf("Error: key is too long\n");
      return -1;
   }

   sprintf(fname, "%s/%s", "./ALMACEN", key); //Concateno

   // Comprueba si fname no existe, en ese caso devolver error
   ret = access(fname, F_OK);
   if (ret != 0) {
      printf("ERROR: file %s does not exist\n", fname);
      return -1;
   }

   // Comprueba la longitud de value1
   if (strlen(value1) > 255) {
      printf("Error: value1 is too long\n");
      return -1;
   }

   // Comprueba el rango de N_value2
   if (N_value2 < 1) {
      printf("Error: N_value2 < 1\n");
      return -1;
   }
   if (N_value2 > 32) {
      printf("Error: N_value2 > 32\n");
      return -1;
   }

   // Abre el archivo para escritura y sobreescribe los nuevos datos
   fd = fopen(fname, "w");
   if (fd == NULL) {
      perror("fopen: ");
      return -1;
   }
   fprintf(fd, "%s\n", value1);
   fprintf(fd, "%d\n", N_value2);
   for (int i = 0; i < N_value2; i++) {
      fprintf(fd, "%f\n", V_value2[i]);
   }

   fprintf(fd, "%d\n", value3.x);
   fprintf(fd, "%d\n", value3.y);
   fprintf(fd, "%d\n", value3.z);

   fclose(fd);
   
   return 0;
}

int delete_key(char *key){
   char fname[1024];
   int ret;

   // Comprueba la longitud de key
   if (strlen(key) > 255) {
      printf("Error: key is too long\n");
      return -1;
   }

   sprintf(fname, "%s/%s", "./ALMACEN", key); //Concateno

   // Comprueba si fname no existe, en ese caso devolver error
   ret = access(fname, F_OK);
   if (ret != 0) {
      printf("ERROR: file %s does not exist\n", fname);
      return -1;
   }
   
   // Elimina el archivo
   ret = remove(fname);
   if (ret != 0) {
      perror("remove: ");
      return -1;
   }

   return 0;
}

int exist(char *key){
   char fname[1024];
   int ret;

   // Comprueba la longitud de key
   if (strlen(key) > 255) {
      printf("Error: key is too long\n");
      return -1;
   }

   sprintf(fname, "%s/%s", "./ALMACEN", key);

   // Comprobramos si fname no existe, en ese caso devuelve error
   ret = access(fname, F_OK);
   if (ret != 0) {
      printf("ERROR: file %s does not exist\n", fname);
      return 0;
   }

   return 1;
}



