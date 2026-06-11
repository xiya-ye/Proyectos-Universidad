#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <arpa/inet.h>
#include "usuarios.h"

int init_almacen(void) {
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

int registrar_usuario (char *nombre) {
    FILE *fd;
    char fname[1024];
    int ret;

    // Comprueba la longitud de nombre
    if (strlen(nombre) > 255) {
        printf("Error: nombre is too long\n");
        return 2;
    }

    sprintf(fname, "%s/%s.txt", "./ALMACEN", nombre); //Concateno

    // Comprueba si fname existe, en ese caso devolver 1 (ya existe)
    ret = access(fname, F_OK);
    if (0 == ret) {
        printf("ERROR: file %s exists\n", fname);
        return 1;
    }

   // Abre el archivo para escritura y escribe los datos
    fd = fopen(fname, "w");
    if (NULL == fd) {
        perror("fopen: ");
        return 2;
    }

    fprintf(fd, "0\n");         // Estado de conexión: 0 = desconectado
    fprintf(fd, "0.0.0.0\n");   // IP vacía
    fprintf(fd, "0\n");         // Puerto vacío
    fprintf(fd, "0\n");         // Último mensaje ID
    fprintf(fd, "0\n");         // Número de mensajes pendientes


    fclose(fd);
    return 0;
}

int baja_usuario (char *nombre) {
    char fname[1024];
    int ret;

    // Comprueba la longitud de nombre
    if (strlen(nombre) > 255) {
        printf("Error: nombre is too long\n");
        return 2;
    }

    sprintf(fname, "%s/%s.txt", "./ALMACEN", nombre); //Concateno

    // Comprueba si fname existe, en ese caso devolver 1 (ya existe)
    ret = access(fname, F_OK);
    if (0 != ret) {
        printf("ERROR: file %s does not exist\n", fname);
        return 1;
    }

    // Elimina el archivo del usuario
    ret = remove(fname);
    if (0 != ret) {
        perror("remove: ");
        return 2;
    }

    return 0;
}

int conectar_usuario (char *nombre, char *ip, char *puerto) {
    FILE *fd;
    char fname[1024];
    int ret;

    // Comprueba la longitud de nombre
    if (strlen(nombre) > 255) {
        printf("Error: nombre is too long\n");
        return 3;
    }

    sprintf(fname, "%s/%s.txt", "./ALMACEN", nombre); //Concateno

    // Comprueba si fname no existe
    ret = access(fname, F_OK);
    if (0 != ret) {
        printf("ERROR: file %s does not exist\n", fname);
        return 1;
    }

   // Abre el archivo para lectura y escritura
    fd = fopen(fname, "r");
    if (NULL == fd) {
        perror("fopen: ");
        return 3;
    }

    // Guardamos los datos actuales antes
    int estado;
    char ip_anterior[256];
    char puerto_anterior[256];
    unsigned int ultimo_id_mensaje;
    int num_mensajes;

    fscanf(fd, "%d\n", &estado);
    fgets(ip_anterior, 256, fd);
    ip_anterior[strcspn(ip_anterior, "\n")] = 0;
    fgets(puerto_anterior, 256, fd);
    puerto_anterior[strcspn(puerto_anterior, "\n")] = 0;
    fscanf(fd, "%u\n", &ultimo_id_mensaje);
    fscanf(fd, "%d\n", &num_mensajes);

    // Si ya está conectado
    if (estado == 1) {
        fclose(fd);
        return 2;
    }

    // Guardamos los mensajes pendientes
    struct MensajePendiente *mensajes_pendientes = NULL;
    if (num_mensajes > 0) {
        mensajes_pendientes = malloc (num_mensajes * sizeof(struct MensajePendiente));
        for (int i = 0; i < num_mensajes; i++) {
            fscanf(fd, "%u\n", &mensajes_pendientes[i].id);
            fgets(mensajes_pendientes[i].remitente, 256, fd);
            fgets(mensajes_pendientes[i].mensaje, 256, fd);
            fgets(mensajes_pendientes[i].fichero, 256, fd);
            mensajes_pendientes[i].remitente[strcspn(mensajes_pendientes[i].remitente, "\n")] = 0;
            mensajes_pendientes[i].mensaje[strcspn(mensajes_pendientes[i].mensaje, "\n")] = 0;
            mensajes_pendientes[i].fichero[strcspn(mensajes_pendientes[i].fichero, "\n")] = 0;
        }
    }

    fclose(fd);

    // Abrimos el archivo para escribir todos los datos actualizados
    fd = fopen(fname, "w");
    if (NULL == fd) {
        perror("fopen: ");
        free(mensajes_pendientes);
        return 3;
    }

    fprintf(fd, "1\n");                     // Estado de conexión: 1 = conectado
    fprintf(fd, "%s\n", ip);                // IP del usuario
    fprintf(fd, "%s\n", puerto);            // Puerto del usuario
    fprintf(fd, "%u\n", ultimo_id_mensaje); // Último mensaje ID
    fprintf(fd, "%d\n", num_mensajes);      // Número de mensajes pendientes

    // Escribimos los mensajes pendientes si los hay
    for (int i = 0; i < num_mensajes; i++) {
        fprintf(fd, "%u\n%s\n%s\n%s\n", mensajes_pendientes[i].id, mensajes_pendientes[i].remitente, mensajes_pendientes[i].mensaje, mensajes_pendientes[i].fichero);
    }

    fclose(fd);
    free(mensajes_pendientes);
    return 0;
}

int obtener_mensajes_pendientes (char *nombre, int *num_mensajes, struct MensajePendiente **mensajes) {
    FILE *fd;
    char fname[1024];
    int ret;

    sprintf(fname, "%s/%s.txt", "./ALMACEN", nombre); //Concateno
    // Comprueba si fname no existe
    ret = access(fname, F_OK);
    if (0 != ret) {
        printf("ERROR: file %s does not exist\n", fname);
        return -1;
    }

    fd = fopen(fname, "r");
    if (NULL == fd) {
        perror("fopen: ");
        return -1;
    }

    int estado;
    char ip[256], puerto[256];
    unsigned int ultimo_id_mensaje;

    // Saltamos las primeras 4 líneas
    fscanf(fd, "%d\n", &estado);
    fgets(ip, 256, fd);
    fgets(puerto, 256, fd);
    fscanf(fd, "%u\n", &ultimo_id_mensaje);

    // Leemos el número de mensajes pendientes
    fscanf(fd, "%d\n", num_mensajes);

    if (*num_mensajes > 0) {
        *mensajes = malloc (*num_mensajes * sizeof(struct MensajePendiente));
        for (int i = 0; i < *num_mensajes; i++) {
            fscanf(fd, "%u\n", &(*mensajes)[i].id);
            fgets((*mensajes)[i].remitente, 256, fd);
            fgets((*mensajes)[i].mensaje, 256, fd);
            fgets((*mensajes)[i].fichero, 256, fd);
            (*mensajes)[i].remitente[strcspn((*mensajes)[i].remitente, "\n")] = 0;
            (*mensajes)[i].mensaje[strcspn((*mensajes)[i].mensaje, "\n")] = 0;
            (*mensajes)[i].fichero[strcspn((*mensajes)[i].fichero, "\n")] = 0;
        }
    } else {
        *mensajes = NULL;
    }

    fclose(fd);
    return 0;
}

int desconectar_usuario (char *nombre) {
    FILE *fd;
    char fname[1024];
    int ret;

    // Comprueba la longitud de nombre
    if (strlen(nombre) > 255) {
        printf("Error: nombre is too long\n");
        return 3;
    }

    sprintf(fname, "%s/%s.txt", "./ALMACEN", nombre); //Concateno

    // Comprueba si fname no existe
    ret = access(fname, F_OK);
    if (0 != ret) {
        printf("ERROR: file %s does not exist\n", fname);
        return 1;
    }

   // Abre el archivo para lectura y escritura
    fd = fopen(fname, "r");
    if (NULL == fd) {
        perror("fopen: ");
        return 3;
    }

    // Guardamos los datos actuales antes
    int estado;
    char ip_anterior[256];
    char puerto_anterior[256];
    unsigned int ultimo_id_mensaje;
    int num_mensajes;

    fscanf(fd, "%d\n", &estado);
    fgets(ip_anterior, 256, fd);
    fgets(puerto_anterior, 256, fd);
    fscanf(fd, "%u\n", &ultimo_id_mensaje);
    fscanf(fd, "%d\n", &num_mensajes);

    // Si ya está desconectado
    if (estado == 0) {
        fclose(fd);
        return 2;
    }

    // Guardamos los mensajes pendientes
    struct MensajePendiente *mensajes_pendientes = NULL;
    if (num_mensajes > 0) {
        mensajes_pendientes = malloc (num_mensajes * sizeof(struct MensajePendiente));
        for (int i = 0; i < num_mensajes; i++) {
            fscanf(fd, "%u\n", &mensajes_pendientes[i].id);
            fgets(mensajes_pendientes[i].remitente, 256, fd);
            fgets(mensajes_pendientes[i].mensaje, 256, fd);
            fgets(mensajes_pendientes[i].fichero, 256, fd);
            mensajes_pendientes[i].remitente[strcspn(mensajes_pendientes[i].remitente, "\n")] = 0;
            mensajes_pendientes[i].mensaje[strcspn(mensajes_pendientes[i].mensaje, "\n")] = 0;
            mensajes_pendientes[i].fichero[strcspn(mensajes_pendientes[i].fichero, "\n")] = 0;
        }
    }

    fclose(fd);

    // Abrimos el archivo para escribir todos los datos actualizados
    fd = fopen(fname, "w");
    if (NULL == fd) {
        perror("fopen: ");
        free(mensajes_pendientes);
        return 3;
    }

    fprintf(fd, "0\n");                     // Estado de conexión: 0 = desconectado
    fprintf(fd, "0.0.0.0\n");               // IP del usuario
    fprintf(fd, "\n");                      // Puerto del usuario
    fprintf(fd, "%u\n", ultimo_id_mensaje); // Último mensaje ID
    fprintf(fd, "%d\n", num_mensajes);      // Número de mensajes pendientes

    // Escribimos los mensajes pendientes si los hay
    for (int i = 0; i < num_mensajes; i++) {
        fprintf(fd, "%u\n%s\n%s\n%s\n", mensajes_pendientes[i].id, mensajes_pendientes[i].remitente, mensajes_pendientes[i].mensaje, mensajes_pendientes[i].fichero);
    }

    fclose(fd);
    free(mensajes_pendientes);
    return 0;
}

int obtener_siguiente_id_mensaje (char *remitente) {
    FILE *fd;
    char fname[1024];
    int ret;

    // Comprueba la longitud de nombre
    if (strlen(remitente) > 255) {
        printf("Error: remitente is too long\n");
        return -1;
    }

    sprintf(fname, "%s/%s.txt", "./ALMACEN", remitente); //Concateno

    // Comprueba si fname no existe
    ret = access(fname, F_OK);
    if (0 != ret) {
        printf("ERROR: file %s does not exist\n", fname);
        return -1;
    }

    // Abre el archivo para lectura
    fd = fopen(fname, "r");
    if (NULL == fd) {
        perror("fopen: ");
        return -1;
    }

    // Guardamos los datos actuales antes
    int estado;
    char ip_anterior[256];
    char puerto_anterior[256];
    unsigned int ultimo_id_mensaje;
    int num_mensajes;

    fscanf(fd, "%d\n", &estado);
    fgets(ip_anterior, 256, fd);
    ip_anterior[strcspn(ip_anterior, "\n")] = 0;
    fgets(puerto_anterior, 256, fd);
    puerto_anterior[strcspn(puerto_anterior, "\n")] = 0;
    fscanf(fd, "%u\n", &ultimo_id_mensaje);
    fscanf(fd, "%d\n", &num_mensajes);
    
    // Sumamos 1 al ID del último mensaje
    ultimo_id_mensaje++;

    // Guardamos los mensajes pendientes
    struct MensajePendiente *mensajes_pendientes = NULL;
    if (num_mensajes > 0) {
        mensajes_pendientes = malloc (num_mensajes * sizeof(struct MensajePendiente));
        for (int i = 0; i < num_mensajes; i++) {
            fscanf(fd, "%u\n", &mensajes_pendientes[i].id);
            fgets(mensajes_pendientes[i].remitente, 256, fd);
            fgets(mensajes_pendientes[i].mensaje, 256, fd);
            fgets(mensajes_pendientes[i].fichero, 256, fd);
            mensajes_pendientes[i].remitente[strcspn(mensajes_pendientes[i].remitente, "\n")] = 0;
            mensajes_pendientes[i].mensaje[strcspn(mensajes_pendientes[i].mensaje, "\n")] = 0;
            mensajes_pendientes[i].fichero[strcspn(mensajes_pendientes[i].fichero, "\n")] = 0;
        }
    }
    fclose(fd);

    // Abrimos el archivo para escribir todos los datos actualizados
    fd = fopen(fname, "w");
    if (NULL == fd) {
        perror("fopen: ");
        return -1;
    }
    fprintf(fd, "%d\n%s\n%s\n%u\n%d\n", estado, ip_anterior, puerto_anterior, ultimo_id_mensaje, num_mensajes);
    // Escribimos los mensajes pendientes si los hay
    for (int i = 0; i < num_mensajes; i++) {
        fprintf(fd, "%u\n%s\n%s\n%s\n", mensajes_pendientes[i].id, mensajes_pendientes[i].remitente, mensajes_pendientes[i].mensaje, mensajes_pendientes[i].fichero);
    }

    fclose(fd);
    free(mensajes_pendientes);
    return ultimo_id_mensaje;
}

int guardar_mensaje_pendiente (char *destinatario, char *remitente, unsigned int id, char *mensaje, char *fichero) {
    FILE *fd;
    char fname[1024];
    int ret;

    // Comprueba la longitud de destinatario y remitente
    if (strlen(destinatario) > 255 || strlen(remitente) > 255) {
        printf("Error: destinatario or remitente is too long\n");
        return 2;
    }

    sprintf(fname, "%s/%s.txt", "./ALMACEN", destinatario); //Concateno

    // Comprueba si fname no existe
    ret = access(fname, F_OK);
    if (0 != ret) {
        printf("ERROR: file %s does not exist\n", fname);
        return 1;
    }

   // Abre el archivo para lectura
    fd = fopen(fname, "r");
    if (NULL == fd) {
        perror("fopen: ");
        return 2;
    }

    // Guardamos los datos actuales antes
    int estado;
    char ip_anterior[256];
    char puerto_anterior[256];
    unsigned int ultimo_id_mensaje;
    int num_mensajes;

    fscanf(fd, "%d\n", &estado);
    fgets(ip_anterior, 256, fd);
    ip_anterior[strcspn(ip_anterior, "\n")] = 0;
    fgets(puerto_anterior, 256, fd);
    puerto_anterior[strcspn(puerto_anterior, "\n")] = 0;
    fscanf(fd, "%u\n", &ultimo_id_mensaje);
    fscanf(fd, "%d\n", &num_mensajes);

    // Guardamos los mensajes pendientes
    struct MensajePendiente *mensajes_pendientes = NULL;
    if (num_mensajes > 0) {
        mensajes_pendientes = malloc (num_mensajes * sizeof(struct MensajePendiente));
        for (int i = 0; i < num_mensajes; i++) {
            fscanf(fd, "%u\n", &mensajes_pendientes[i].id);
            fgets(mensajes_pendientes[i].remitente, 256, fd);
            fgets(mensajes_pendientes[i].mensaje, 256, fd);
            fgets(mensajes_pendientes[i].fichero, 256, fd);
            mensajes_pendientes[i].remitente[strcspn(mensajes_pendientes[i].remitente, "\n")] = 0;
            mensajes_pendientes[i].mensaje[strcspn(mensajes_pendientes[i].mensaje, "\n")] = 0;
            mensajes_pendientes[i].fichero[strcspn(mensajes_pendientes[i].fichero, "\n")] = 0;
        }
    }
    fclose(fd);

    // Añadimos un mensaje
    num_mensajes++;

    fd = fopen(fname, "w");
    if (NULL == fd) {
        perror("fopen: ");
        return 2;
    }
    fprintf(fd, "%d\n%s\n%s\n%u\n%d\n", estado, ip_anterior, puerto_anterior, ultimo_id_mensaje, num_mensajes);
    // Escribimos los mensajes pendientes si los hay
    for (int i = 0; i < num_mensajes - 1; i++) {
        fprintf(fd, "%u\n%s\n%s\n%s\n", mensajes_pendientes[i].id, mensajes_pendientes[i].remitente, mensajes_pendientes[i].mensaje, mensajes_pendientes[i].fichero);
    }

    // Escribimos el nuevo
    fprintf(fd, "%u\n%s\n%s\n%s\n", id, remitente, mensaje, fichero);

    fclose(fd);
    free(mensajes_pendientes);
    return 0;
}

int obtener_datos_conexion_usuario (char *nombre, int *estado, char *ip, char *puerto) {
    FILE *fd;
    char fname[1024];
    int ret;

    // Comprueba la longitud de nombre
    if (strlen(nombre) > 255) {
        printf("Error: nombre is too long\n");
        return -1;
    }

    sprintf(fname, "%s/%s.txt", "./ALMACEN", nombre); //Concateno

    // Comprueba si fname no existe
    ret = access(fname, F_OK);
    if (0 != ret) {
        printf("ERROR: file %s does not exist\n", fname);
        return -1;
    }

   // Abre el archivo para lectura
    fd = fopen(fname, "r");
    if (NULL == fd) {
        perror("fopen: ");
        return -1;
    }

    fscanf(fd, "%d\n", estado);
    fgets(ip, 256, fd);
    fgets(puerto, 256, fd);
    ip[strcspn(ip, "\n")] = 0;
    puerto[strcspn(puerto, "\n")] = 0;

    fclose(fd);
    return 0;
}

int eliminar_mensaje_pendiente (char *usuario, unsigned int id) {
    FILE *fd;
    char fname[1024];
    int ret;

    // Comprueba la longitud de destinatario y remitente
    if (strlen(usuario) > 255) {
        printf("Error: usuario is too long\n");
        return -1;
    }

    sprintf(fname, "%s/%s.txt", "./ALMACEN", usuario); //Concateno

    // Comprueba si fname no existe
    ret = access(fname, F_OK);
    if (0 != ret) {
        printf("ERROR: file %s does not exist\n", fname);
        return -1;
    }

   // Abre el archivo para lectura
    fd = fopen(fname, "r");
    if (NULL == fd) {
        perror("fopen: ");
        return -1;
    }

    // Guardamos los datos actuales antes
    int estado;
    char ip_anterior[256];
    char puerto_anterior[256];
    unsigned int ultimo_id_mensaje;
    int num_mensajes;

    fscanf(fd, "%d\n", &estado);
    fgets(ip_anterior, 256, fd);
    ip_anterior[strcspn(ip_anterior, "\n")] = 0;
    fgets(puerto_anterior, 256, fd);
    puerto_anterior[strcspn(puerto_anterior, "\n")] = 0;
    fscanf(fd, "%u\n", &ultimo_id_mensaje);
    fscanf(fd, "%d\n", &num_mensajes);

    // Guardamos los mensajes pendientes
    struct MensajePendiente *mensajes_pendientes = NULL;
    if (num_mensajes > 0) {
        mensajes_pendientes = malloc (num_mensajes * sizeof(struct MensajePendiente));
        for (int i = 0; i < num_mensajes; i++) {
            fscanf(fd, "%u\n", &mensajes_pendientes[i].id);
            fgets(mensajes_pendientes[i].remitente, 256, fd);
            fgets(mensajes_pendientes[i].mensaje, 256, fd);
            fgets(mensajes_pendientes[i].fichero, 256, fd);
            mensajes_pendientes[i].remitente[strcspn(mensajes_pendientes[i].remitente, "\n")] = 0;
            mensajes_pendientes[i].mensaje[strcspn(mensajes_pendientes[i].mensaje, "\n")] = 0;
            mensajes_pendientes[i].fichero[strcspn(mensajes_pendientes[i].fichero, "\n")] = 0;
        }
    }
    fclose(fd);


    fd = fopen(fname, "w");
    if (NULL == fd) {
        perror("fopen: ");
        return -1;
    }

    // ELiminamos un mensaje --> restar 1 al total
    fprintf(fd, "%d\n%s\n%s\n%u\n%d\n", estado, ip_anterior, puerto_anterior, ultimo_id_mensaje, num_mensajes - 1);
    // Escribimos todos los mensajes, menos el que queremos eliminar
    for (int i = 0; i < num_mensajes; i++) {
        if (mensajes_pendientes[i].id != id) {
            fprintf(fd, "%u\n%s\n%s\n%s\n", mensajes_pendientes[i].id, mensajes_pendientes[i].remitente, mensajes_pendientes[i].mensaje, mensajes_pendientes[i].fichero);
        }
    }

    fclose(fd);
    free(mensajes_pendientes);
    return 0;   
}

int obtener_usuarios_conectados(char *usuario_pet, int *num_conectados, char usuarios_conectados[][256]) {
    *num_conectados = 0;
    int ret;
    DIR *d;
    struct dirent *dir;
    
    // Verificamos si el usuario que solicita la lista está conectado
    int estado;
    char ip[256], puerto[256];
    ret = obtener_datos_conexion_usuario(usuario_pet, &estado, ip, puerto);
    
    if (0 != ret) return 2;
    if (estado == 0) return 1;

    d = opendir("./ALMACEN");
    if (d) {
        while (NULL != (dir = readdir(d))) {
            // Buscamos solo los archivos .txt
            if (NULL != strstr(dir->d_name, ".txt")) {
                char nombre_usuario[256];
                strcpy(nombre_usuario, dir->d_name);
                
                // Buscamos '.' y lo sustituimos por '\0'
                char *punto = strrchr(nombre_usuario, '.');
                if (NULL != punto) {
                    *punto = '\0';
                }

                // Leemos su estado, si esta conectado --> añadimos
                ret = obtener_datos_conexion_usuario(nombre_usuario, &estado, ip, puerto);
                if (0 == ret && 1 == estado) {
                    strcpy(usuarios_conectados[*num_conectados], nombre_usuario);
                    (*num_conectados)++;
                }
            }
        }
        closedir(d);
    } else {
        perror("opendir: ");
        return 2;
    }
    return 0;
}