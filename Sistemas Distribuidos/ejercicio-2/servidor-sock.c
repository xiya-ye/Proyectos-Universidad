#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include "claves.h"
#include "lines.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *tratar_cliente (void *arg) {
    int socket_cliente = *(int *) arg;
    free(arg);

    char op_code;
    char buffer[256];

    while (read(socket_cliente, &op_code, sizeof(op_code)) > 0) {
        if (op_code == '0') { // Destroy
            printf("Recibida operación DESTROY\n");
            
            pthread_mutex_lock(&mutex);
            int res = destroy();
            pthread_mutex_unlock(&mutex);
            
            write_int(socket_cliente, res);
        }

        else if (op_code == '1') { //Set_value
            char key[256], value1[256];
            int N_value2;
            float V_value2[32];
            struct Paquete value3;

            printf("Recibida operación SET\n");

            // Leemos los datos de la operación SET
            readLine(socket_cliente, key, 256);
            readLine(socket_cliente, value1, 256);
            read_int(socket_cliente, &N_value2);
            for (int i = 0; i < N_value2; i++) {
                readLine(socket_cliente, buffer, 256);
                V_value2[i] = atof(buffer);
            }
            read_int(socket_cliente, &value3.x);
            read_int(socket_cliente, &value3.y);
            read_int(socket_cliente, &value3.z);

            // Llamamos a la función
            pthread_mutex_lock(&mutex);
            int res = set_value(key, value1, N_value2, V_value2, value3);
            pthread_mutex_unlock(&mutex);

            write_int(socket_cliente, res);
        }

        else if (op_code == '2') { //Get_value
            char key[256], value1[256];
            int N_value2;
            float V_value2[32];
            struct Paquete value3;

            printf("Recibida operación GET\n");
            
            // Leemos los datos de la operación GET
            readLine(socket_cliente, key, 256);
            
            // Llamamos a la función
            pthread_mutex_lock(&mutex);
            int res = get_value(key, value1, &N_value2, V_value2, &value3);
            pthread_mutex_unlock(&mutex);
            
            write_int(socket_cliente, res);

            // Enviamos los datos al cliente si la operación se realizó correctamente
            if (res == 0) {
                value1[strcspn(value1, "\r")] = 0;
                value1[strcspn(value1, "\n")] = 0;

                writeLine(socket_cliente, value1);
                write_int(socket_cliente, N_value2);
                for (int i = 0; i < N_value2; i++) {
                    sprintf(buffer, "%f", V_value2[i]);
                    writeLine(socket_cliente, buffer);
                }
                write_int(socket_cliente, value3.x);
                write_int(socket_cliente, value3.y);
                write_int(socket_cliente, value3.z);
            }
                
        }

        else if (op_code == '3') { //Modify_value
            char key[256], value1[256];
            int N_value2;
            float V_value2[32];
            struct Paquete value3;
            
            printf("Recibida operación MODIFY\n");

            // Leemos los datos de la operación MODIFY
            readLine(socket_cliente, key, 256);
            readLine(socket_cliente, value1, 256);
            read_int(socket_cliente, &N_value2);
            for (int i = 0; i < N_value2; i++) {
                readLine(socket_cliente, buffer, 256);
                V_value2[i] = atof(buffer);
            }
            read_int(socket_cliente, &value3.x);
            read_int(socket_cliente, &value3.y);
            read_int(socket_cliente, &value3.z);

            // Llamamos a la función
            pthread_mutex_lock(&mutex);
            int res = modify_value(key, value1, N_value2, V_value2, value3);
            pthread_mutex_unlock(&mutex);

            write_int(socket_cliente, res);
        }

        else if (op_code == '4') { //Delete_value
            char key[256];

            printf("Recibida operación DELETE\n");

            // Leemos los datos de la operación DELETE
            readLine(socket_cliente, key, 256);

            // Llamamos a la función
            pthread_mutex_lock(&mutex);
            int res = delete_key(key);
            pthread_mutex_unlock(&mutex);

            write_int(socket_cliente, res);
        }

        else if (op_code == '5') { //Exist
            char key[256];

            printf("Recibida operación EXIST\n");

            // Leemos los datos de la operación EXIST
            readLine(socket_cliente, key, 256);

            // Llamamos a la función
            pthread_mutex_lock(&mutex);
            int res = exist(key);
            pthread_mutex_unlock(&mutex);

            write_int(socket_cliente, res);
        }

        else {
            printf("Operación no reconocida: %c\n", op_code);
        }
    }
    
    close(socket_cliente);
    pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
    int sd, newsd, ret, val;
    socklen_t size;
    struct sockaddr_in server_addr, client_addr;

    // Comprobamos que nos pasa el número correcto de argumentos
    if (argc != 2) {
        printf("Usage: %s <puerto>\n", argv[0]);
        exit(-1);
    }

    int puerto = atoi(argv[1]) ;

    // Creamos el socket
    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); ;
    if (sd < 0) {
        perror("Error en socket: ");
        exit(-1);
    }

    val = 1;
    ret = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *) &val, sizeof(int));
    if (ret < 0) {
        perror("Error en setsockopt: ");
        exit(-1);
    }

    // Obtenemos la dirección
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(puerto);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Asignamos dirección a un socket (Bind)
    ret = bind(sd,(struct sockaddr *) &server_addr, sizeof(server_addr)) ;
    if (ret < 0) {
        perror("Error en bind: ") ;
        exit(-1);
    } 

    // Preparamos para aceptar conexiones (Listen)
    ret = listen(sd, SOMAXCONN);
    if (ret < 0) {
        perror("Error en listen: ") ;
        exit(-1);
    }

    // Servir clientes...
    while (1) {
        size = sizeof(struct sockaddr_in) ;
        newsd = accept (sd, (struct sockaddr *) &client_addr, &size);
        if (newsd < 0) {
            perror("Error en accept");
            continue;
        }

        // Establecemos memoria dinámica para el socket
        int *socket_thread = malloc(sizeof(int));
        *socket_thread = newsd;

        // Creamos un hilo para atender al cliente
        pthread_t thread_id;
        ret = pthread_create(&thread_id, NULL, tratar_cliente, (void*) socket_thread);
        if (ret != 0) {
            perror("Error en pthread_create: ");
            free(socket_thread);
            close(newsd);
        }
        else {
            pthread_detach(thread_id);
        }
    }

    close (sd);
    return 0;

}
