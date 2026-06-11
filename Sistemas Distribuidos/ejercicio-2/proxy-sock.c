#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include "lines.h"
#include "claves.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Creamos una función auxiliar para conectar con el servidor
int conectar_servidor() {
    int sd;
    struct sockaddr_in server_addr;
    struct hostent *hp;
    char *ip_env = getenv("IP_TUPLAS");
    char *port_env = getenv("PORT_TUPLAS");

    // Verificamos las variables de entorno
    if (ip_env == NULL || port_env == NULL) {
        return -1;
    }

    // Creamos el socket con protocolo TCP
    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd < 0) {
        return -1;
    }

    // Obtenemos la direccion del servidor
    pthread_mutex_lock(&mutex);
    hp = gethostbyname(ip_env);
    if (NULL == hp) {
        close(sd);
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    bzero((char *)&server_addr, sizeof(server_addr));
    memcpy(&(server_addr.sin_addr), hp->h_addr, hp->h_length);
    pthread_mutex_unlock(&mutex);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(port_env));

    // Solicitamos la conexión al servidor
    if (connect(sd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        close(sd);
        return -1;
    }

    return sd;
}

int destroy(void) {
    int resultado;
    int sd = conectar_servidor();
    if (sd < 0) {
        return -1;
    }
    
    // enviamos el código de operación
    char op = '0';
    sendMessage(sd, &op, 1);

    // Recibimos la respuesta del servidor
    if (read_int(sd, &resultado) < 0) {
        close(sd);
        return -1;
    }

    close(sd);
    return resultado;
}


int set_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3){
    // Validamos los argumentos
    if (strlen(key) > 255) return -1;
    if (strlen(value1) > 255) return -1;
    if (N_value2 < 1 || N_value2 > 32) return -1;

    char buffer[256];
    int resultado;
    int sd = conectar_servidor();
    if (sd < 0) {
        return -1;
    }

    // Enviamos el código de operación
    char op = '1';
    sendMessage(sd, &op, 1);

    // Enviamos los argumentos de la operación
    writeLine(sd, key);
    writeLine(sd, value1);
    write_int(sd, N_value2);

    for (int i = 0; i < N_value2; i++) {
        sprintf(buffer, "%f", V_value2[i]);
        writeLine(sd, buffer);
    }

    write_int(sd, value3.x);
    write_int(sd, value3.y);
    write_int(sd, value3.z);

    // Recibimos la respuesta del servidor
    if (read_int(sd, &resultado) < 0) {
        close(sd);
        return -1;
    }

    close(sd);
    return resultado;

}

int get_value(char *key, char *value1, int *N_value2, float *V_value2, struct Paquete *value3) {
    // Validamos los argumentos
    if (strlen(key) > 255) return -1;
    
    char buffer[256];
    int resultado;
    int sd = conectar_servidor();
    if (sd < 0) {
        return -1;
    }

    // Enviamos el código de operación
    char op = '2';
    sendMessage(sd, &op, 1);
    writeLine(sd, key);

    // Recibimos la respuesta del servidor
    if (read_int(sd, &resultado) < 0) {
        close(sd);
        return -1;
    }

    if (0 == resultado) {
        // Si la operación fue exitosa --> leemos los datos de la tupla
        readLine(sd, value1, 256);
        read_int(sd, N_value2);

        for (int i = 0; i < *N_value2; i++) {
            readLine(sd, buffer, 256);
            V_value2[i] = atof(buffer);
        }
        read_int(sd, &value3->x);
        read_int(sd, &value3->y);
        read_int(sd, &value3->z);
    }

    close(sd);
    return resultado;
}

int modify_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {
    // Validamos los argumentos
    if (strlen(key) > 255) return -1;
    if (strlen(value1) > 255) return -1;
    if (N_value2 < 1 || N_value2 > 32) return -1;
    
    char buffer[256];
    int resultado;
    int sd = conectar_servidor();
    if (sd < 0) {
        return -1;
    }

    // Enviamos el código de operación
    char op = '3';
    sendMessage(sd, &op, 1);

    // Enviamos los argumentos de la operación
    writeLine(sd, key);
    writeLine(sd, value1);
    write_int(sd, N_value2);

    for (int i = 0; i < N_value2; i++) {
        sprintf(buffer, "%f", V_value2[i]);
        writeLine(sd, buffer);
    }

    write_int(sd, value3.x);
    write_int(sd, value3.y);
    write_int(sd, value3.z);

    // Recibimos la respuesta del servidor
    if (read_int(sd, &resultado) < 0) {
        close(sd);
        return -1;
    }

    close(sd);
    return resultado;
}

int delete_key(char *key){
    // Validamos los argumentos
    if (strlen(key) > 255) return -1;
    
    int resultado;
    int sd = conectar_servidor();
    if (sd < 0) {
        return -1;
    }

    // Enviamos el código de operación
    char op = '4';
    sendMessage(sd, &op, 1);
    writeLine(sd, key);

    // Recibimos la respuesta del servidor
    if (read_int(sd, &resultado) < 0) {
        close(sd);
        return -1;
    }

    close(sd);
    return resultado;
}


int exist(char *key){
    // Validamos los argumentos
    if (strlen(key) > 255) return -1;
    
    int resultado;
    int sd = conectar_servidor();
    if (sd < 0) {
        return -1;
    }

    // Enviamos el código de operación
    char op = '5';
    sendMessage(sd, &op, 1);
    writeLine(sd, key);

    // Recibimos la respuesta del servidor
    if (read_int(sd, &resultado) < 0) {
        close(sd);
        return -1;
    }

    close(sd);
    return resultado;
}

