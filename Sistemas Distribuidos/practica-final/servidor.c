#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <getopt.h>
#include "lines.h"
#include "usuarios.h"
#include "rpc_client.h"


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//variable global para el host RPC
static char *rpc_host = NULL;


void *tratar_cliente (void *arg) {
    int socket_cliente = *(int *) arg;
    free(arg);

    char operacion[256];
    char respuesta;

    while (readLine(socket_cliente, operacion, 256) > 0) {
        if (0 == strcmp(operacion, "REGISTER")) {
            char nombre_usuario[256];
            int res;
            readLine(socket_cliente, nombre_usuario, 256);       
            
            // Llamamos a la función
            pthread_mutex_lock(&mutex);
            res = registrar_usuario(nombre_usuario);
            pthread_mutex_unlock(&mutex);

            if(rpc_host != NULL) {
                registrar_operacion(rpc_host, nombre_usuario, "REGISTER", "");
            }
            
            // Enviamos el byte de respuesta al cliente
            respuesta = (char) res;
            write(socket_cliente, &respuesta, 1);

            if (0 == res) {
                printf("s> REGISTER %s OK\n", nombre_usuario);
            }
            else {
                printf("s> REGISTER %s FAIL\n", nombre_usuario);
            }
        }

        else if (0 == strcmp(operacion, "UNREGISTER")) {
            char nombre_usuario[256];
            int res;
            readLine(socket_cliente, nombre_usuario, 256);       
            
            // Llamamos a la función
            pthread_mutex_lock(&mutex);
            res = baja_usuario(nombre_usuario);
            pthread_mutex_unlock(&mutex);

            if(rpc_host != NULL) {
                registrar_operacion(rpc_host, nombre_usuario, "UNREGISTER", "");
            }
            
            // Enviamos el byte de respuesta al cliente
            respuesta = (char) res;
            write(socket_cliente, &respuesta, 1);

            if (0 == res) {
                printf("s> UNREGISTER %s OK\n", nombre_usuario);
            }
            else {
                printf("s> UNREGISTER %s FAIL\n", nombre_usuario);
            }
        }

        else if(0 == strcmp(operacion, "CONNECT")) {
            char nombre_usuario[256];
            char puerto[256];
            int res, num_mensajes = 0;
            struct MensajePendiente *mensajes = NULL;

            // Leemos los datos enviados por el cliente
            readLine(socket_cliente, nombre_usuario, 256);
            readLine(socket_cliente, puerto, 256);

            // Obtenemos la IP del cliente a partir del socket
            struct sockaddr_in addr;
            socklen_t addr_size = sizeof(struct sockaddr_in);
            getpeername(socket_cliente, (struct sockaddr *)&addr, &addr_size);
            char *ip_cliente = inet_ntoa(addr.sin_addr);

            // Llamamos a la función
            pthread_mutex_lock(&mutex);
            res = conectar_usuario(nombre_usuario, ip_cliente, puerto);

            // Extraemos mensajes pendientes
            if (0 == res) {
                obtener_mensajes_pendientes(nombre_usuario, &num_mensajes, &mensajes);
            }
            pthread_mutex_unlock(&mutex);

            // Enviamos el byte de respuesta al cliente
            respuesta = (char) res;
            write(socket_cliente, &respuesta, 1);

            if (0 == res) {
                printf("s> CONNECT %s OK\n", nombre_usuario);

                if (rpc_host != NULL) {
                    registrar_operacion(rpc_host, nombre_usuario, "CONNECT", "");
                }

                // Enviamos mensajes pendientes
                if (num_mensajes > 0 && NULL != mensajes) {
                    for (int i = 0; i < num_mensajes; i++) {
                        int puerto_dest = atoi(puerto);
                        int sock_dest = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

                        if (sock_dest >= 0) {
                            struct sockaddr_in addr_destino;
                            bzero(&addr_destino, sizeof(addr_destino));
                            addr_destino.sin_family = AF_INET;
                            addr_destino.sin_port = htons(puerto_dest);
                            inet_pton(AF_INET, ip_cliente, &addr_destino.sin_addr);

                            // Conectamos al cliente para enviar el mensaje pendiente
                            if (0 == connect(sock_dest, (struct sockaddr *)&addr_destino, sizeof(addr_destino))) {
                                char id_str[32];
                                snprintf(id_str, sizeof(id_str), "%u", mensajes[i].id);

                                write(sock_dest, "SEND_MESSAGE", strlen("SEND_MESSAGE") + 1);
                                write(sock_dest, mensajes[i].remitente, strlen(mensajes[i].remitente) + 1);
                                write(sock_dest, id_str, strlen(id_str) + 1);
                                write(sock_dest, mensajes[i].mensaje, strlen(mensajes[i].mensaje) + 1);

                                // Comprobamos si hay fichero adjunto o no
                                if (0 == strcmp(mensajes[i].fichero, "*NO_FILE*")) {    // sin fichero
                                    write(sock_dest, "SEND_MESSAGE", strlen("SEND_MESSAGE") + 1);
                                    write(sock_dest, mensajes[i].remitente, strlen(mensajes[i].remitente) + 1);
                                    write(sock_dest, id_str, strlen(id_str) + 1);
                                    write(sock_dest, mensajes[i].mensaje, strlen(mensajes[i].mensaje) + 1);
                                } else {        // con fichero adjunto
                                    write(sock_dest, "SEND_MESSAGE_ATTACH", strlen("SEND_MESSAGE_ATTACH") + 1);
                                    write(sock_dest, mensajes[i].remitente, strlen(mensajes[i].remitente) + 1);
                                    write(sock_dest, id_str, strlen(id_str) + 1);
                                    write(sock_dest, mensajes[i].mensaje, strlen(mensajes[i].mensaje) + 1);
                                    write(sock_dest, mensajes[i].fichero, strlen(mensajes[i].fichero) + 1);
                                }

                                printf("s> SEND MESSAGE %u FROM %s TO %s\n", mensajes[i].id, mensajes[i].remitente, nombre_usuario);

                                // Notificar de mensajes pendientes (SEND_MESS_ACK)
                                pthread_mutex_lock(&mutex);
                                int estado_remitente;
                                char ip_remitente[256], puerto_remitente[256];

                                if (0 == obtener_datos_conexion_usuario(mensajes[i].remitente, &estado_remitente, ip_remitente, puerto_remitente) && 1 == estado_remitente) {
                                    int p_remitente = atoi(puerto_remitente);
                                    if (p_remitente > 0) {
                                        int sock_ack = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                                        if (sock_ack >= 0) {
                                            struct sockaddr_in addr_remitente;
                                            bzero(&addr_remitente, sizeof(addr_remitente));
                                            addr_remitente.sin_family = AF_INET;
                                            addr_remitente.sin_port = htons(p_remitente);
                                            inet_pton(AF_INET, ip_remitente, &addr_remitente.sin_addr);

                                            if (0 == connect(sock_ack, (struct sockaddr *)&addr_remitente, sizeof(addr_remitente))) {
                                                if (0 == strcmp(mensajes[i].fichero, "*NO_FILE*")) {    // sin fichero
                                                    write(sock_ack, "SEND_MESS_ACK", strlen("SEND_MESS_ACK") + 1);
                                                    write(sock_ack, id_str, strlen(id_str) + 1);
                                                } else {        // con fichero adjunto
                                                     write(sock_ack, "SEND_MESS_ATTACH_ACK", strlen("SEND_MESS_ATTACH_ACK") + 1);
                                                     write(sock_ack, id_str, strlen(id_str) + 1);
                                                     write(sock_ack, mensajes[i].fichero, strlen(mensajes[i].fichero) + 1);

                                                }
                                            }
                                            close(sock_ack);
                                        }
                                    }
                                }
                                eliminar_mensaje_pendiente(nombre_usuario, mensajes[i].id);
                                pthread_mutex_unlock(&mutex);
                            }
                            close(sock_dest);
                        }
                    }
                    free(mensajes);
                }
            }
            else {
                printf("s> CONNECT %s FAIL \n", nombre_usuario);
            }
            
        }

        else if (0 == strcmp(operacion, "DISCONNECT")) {
            char nombre_usuario[256];
            int res;

            // Leemos los datos enviados por el cliente
            readLine(socket_cliente, nombre_usuario, 256);

            // Llamamos a la función
            pthread_mutex_lock(&mutex);
            res = desconectar_usuario(nombre_usuario);
            pthread_mutex_unlock(&mutex);

            // Enviamos el byte de respuesta al cliente
            respuesta = (char) res;
            write(socket_cliente, &respuesta, 1);

            if (0 == res) {
                printf("s> DISCONNECT %s OK\n", nombre_usuario);

                if (rpc_host != NULL) {
                    registrar_operacion(rpc_host, nombre_usuario, "DISCONNECT", "");
                }
            }

            else {
                printf("s> DISCONNECT %s FAIL\n", nombre_usuario);
            }
            
        }

        else if(0 == strcmp(operacion, "SEND")) {
            char remitente[256];
            char destinatario[256];
            char mensaje[256];

            // leemos los datos enviados por el cliente
            readLine(socket_cliente, remitente, 256);
            readLine(socket_cliente, destinatario, 256);
            readLine(socket_cliente, mensaje, 256);

            pthread_mutex_lock(&mutex);
            int id_mensaje = obtener_siguiente_id_mensaje(remitente);

            int res;
            if (-1 != id_mensaje) {
                res = guardar_mensaje_pendiente(destinatario, remitente, (unsigned int) id_mensaje, mensaje, "*NO_FILE*");
            } else {
                res = 2; // Error al obtener ID
            }
            pthread_mutex_unlock(&mutex);

            // Respuesta al cliente
            if (-1 == id_mensaje || 1 == res) {     // usuario no existe
                respuesta = 1;
                write(socket_cliente, &respuesta, 1);
            } else if (2 == res) {   // error al obtener ID
                respuesta = 2;
                write(socket_cliente, &respuesta, 1);
            } else {                // éxito
                respuesta = 0;
                write(socket_cliente, &respuesta, 1);

                if (rpc_host != NULL) {
                    registrar_operacion(rpc_host, remitente, "SEND", "");
                }

                char id_str[32];
                snprintf(id_str, sizeof(id_str), "%d", id_mensaje);
                write(socket_cliente, id_str, strlen(id_str) + 1);

                // Intentamos entregar el mensaje si el destinatario está conectado
                int estado_destinatario;
                char ip_destinatario[256], puerto_destinatario[256];
                pthread_mutex_lock(&mutex);
                int ret = obtener_datos_conexion_usuario(destinatario, &estado_destinatario, ip_destinatario, puerto_destinatario);
                pthread_mutex_unlock(&mutex);
                if (0 == ret && 1 == estado_destinatario) {
                    int p_destinatario = atoi(puerto_destinatario);
                    int entregado = 0;

                    if (p_destinatario > 0) {
                        int sock_dest = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                        if (sock_dest >= 0) {
                            struct sockaddr_in addr_destino;
                            bzero(&addr_destino, sizeof(addr_destino));
                            addr_destino.sin_family = AF_INET;
                            addr_destino.sin_port = htons(p_destinatario);
                            inet_pton(AF_INET, ip_destinatario, &addr_destino.sin_addr);

                            if (0 == connect(sock_dest, (struct sockaddr *)&addr_destino, sizeof(addr_destino))) {
                                write(sock_dest, "SEND_MESSAGE", strlen("SEND_MESSAGE") + 1);
                                write(sock_dest, remitente, strlen(remitente) + 1);
                                write(sock_dest, id_str, strlen(id_str) + 1);
                                write(sock_dest, mensaje, strlen(mensaje) + 1);

                                entregado = 1;

                                printf("s> SEND MESSAGE %u FROM %s TO %s\n", id_mensaje, remitente, destinatario);

                                // Borramos el mensaje pendiente
                                pthread_mutex_lock(&mutex);
                                eliminar_mensaje_pendiente(destinatario, (unsigned int) id_mensaje);
                                pthread_mutex_unlock(&mutex);

                                // Notificar de mensajes pendientes (SEND_MESS_ACK)
                                int estado_remitente;
                                char ip_remitente[256], puerto_remitente[256];
                                pthread_mutex_lock(&mutex);
                                ret = obtener_datos_conexion_usuario(remitente, &estado_remitente, ip_remitente, puerto_remitente);
                                pthread_mutex_unlock(&mutex);
                                if (0 == ret && 1 == estado_remitente) {
                                    int p_remitente = atoi(puerto_remitente);
                                    if (p_remitente > 0) {
                                        int sock_ack = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                                        if (sock_ack >= 0) {
                                            struct sockaddr_in addr_remitente;
                                            bzero(&addr_remitente, sizeof(addr_remitente));
                                            addr_remitente.sin_family = AF_INET;
                                            addr_remitente.sin_port = htons(p_remitente);
                                            inet_pton(AF_INET, ip_remitente, &addr_remitente.sin_addr);

                                            if (0 == connect(sock_ack, (struct sockaddr *)&addr_remitente, sizeof(addr_remitente))) {
                                                write(sock_ack, "SEND_MESS_ACK", strlen("SEND_MESS_ACK") + 1);
                                                write(sock_ack, id_str, strlen(id_str) + 1);
                                            }
                                            close(sock_ack);
                                        }
                                    }
                                }
                            }
                            close(sock_dest);
                        }
                    }
                    if (0 == entregado) {
                        // Si connect falla --> desconectamos al ususario
                        pthread_mutex_lock(&mutex);
                        desconectar_usuario(destinatario);
                        pthread_mutex_unlock(&mutex);

                        // Almacenamos el mensaje
                        printf("s> MESSAGE %d FROM %s TO %s STORED\n", id_mensaje, remitente, destinatario);
                    }
                } else {
                    // Destinatario no conectado --> almacenamos mensaje
                    printf("s> MESSAGE %d FROM %s TO %s STORED\n", id_mensaje, remitente, destinatario);
                }
            }
        }
        else if (0 == strcmp(operacion, "SENDATTACH")) {
            char remitente[256];
            char destinatario[256];
            char mensaje[256];
            char fichero[256];

            // Leemos los datos enviados por el cliente
            readLine(socket_cliente, remitente, 256);
            readLine(socket_cliente, destinatario, 256);
            readLine(socket_cliente, mensaje, 256);
            readLine(socket_cliente, fichero, 256);

            pthread_mutex_lock(&mutex);
            int id_mensaje = obtener_siguiente_id_mensaje(remitente);

            int res;
            if (-1 != id_mensaje) {
                res = guardar_mensaje_pendiente(destinatario, remitente, (unsigned int) id_mensaje, mensaje, fichero);
            } else {
                res = 2; // Error al obtener ID
            }
            pthread_mutex_unlock(&mutex);

            // Respuesta al cliente
            if (-1 == id_mensaje || 1 == res) {     // usuario no existe
                respuesta = 1;
                write(socket_cliente, &respuesta, 1);
            } else if (2 == res) {   // error al obtener ID
                respuesta = 2;
                write(socket_cliente, &respuesta, 1);
            } else {                // éxito
                respuesta = 0;
                write(socket_cliente, &respuesta, 1);

                if (rpc_host != NULL) {
                    registrar_operacion(rpc_host, remitente, "SENDATTACH", fichero);
                }

                char id_str[32];
                snprintf(id_str, sizeof(id_str), "%d", id_mensaje);
                write(socket_cliente, id_str, strlen(id_str) + 1);

                // Intentamos entregar el mensaje si el destinatario está conectado
                int estado_destinatario;
                char ip_destinatario[256], puerto_destinatario[256];
                pthread_mutex_lock(&mutex);
                int ret = obtener_datos_conexion_usuario(destinatario, &estado_destinatario, ip_destinatario, puerto_destinatario);
                pthread_mutex_unlock(&mutex);

                if (0 == ret && 1 == estado_destinatario) {
                    int p_destinatario = atoi(puerto_destinatario);
                    int entregado = 0;

                    if (p_destinatario > 0) {
                        int sock_dest = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                        if (sock_dest >= 0) {
                            struct sockaddr_in addr_destino;
                            bzero(&addr_destino, sizeof(addr_destino));
                            addr_destino.sin_family = AF_INET;
                            addr_destino.sin_port = htons(p_destinatario);
                            inet_pton(AF_INET, ip_destinatario, &addr_destino.sin_addr);

                            if (0 == connect(sock_dest, (struct sockaddr *)&addr_destino, sizeof(addr_destino))) {
                                // Se envía la cadena "SEND_MESSAGE_ATTACH" indicando la operación
                                write(sock_dest, "SEND_MESSAGE_ATTACH", strlen("SEND_MESSAGE_ATTACH") + 1);
                                write(sock_dest, remitente, strlen(remitente) + 1);
                                write(sock_dest, id_str, strlen(id_str) + 1);
                                write(sock_dest, mensaje, strlen(mensaje) + 1);
                                write(sock_dest, fichero, strlen(fichero) + 1);

                                entregado = 1;

                                printf("s> SEND MESSAGE %u FROM %s TO %s\n", id_mensaje, remitente, destinatario);

                                // Borramos el mensaje pendiente
                                pthread_mutex_lock(&mutex);
                                eliminar_mensaje_pendiente(destinatario, (unsigned int) id_mensaje);
                                pthread_mutex_unlock(&mutex);

                                // Notificar de mensajes pendientes (SEND_MESS_ATTACH_ACK)
                                int estado_remitente;
                                char ip_remitente[256], puerto_remitente[256];

                                pthread_mutex_lock(&mutex);
                                ret = obtener_datos_conexion_usuario(remitente, &estado_remitente, ip_remitente, puerto_remitente);
                                pthread_mutex_unlock(&mutex);

                                if (0 == ret && 1 == estado_remitente) {
                                    int p_remitente = atoi(puerto_remitente);
                                    if (p_remitente > 0) {
                                        int sock_ack = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                                        if (sock_ack >= 0) {
                                            struct sockaddr_in addr_remitente;
                                            bzero(&addr_remitente, sizeof(addr_remitente));
                                            addr_remitente.sin_family = AF_INET;
                                            addr_remitente.sin_port = htons(p_remitente);
                                            inet_pton(AF_INET, ip_remitente, &addr_remitente.sin_addr);

                                            if (0 == connect(sock_ack, (struct sockaddr *)&addr_remitente, sizeof(addr_remitente))) {
                                                write(sock_ack, "SEND_MESS_ATTACH_ACK", strlen("SEND_MESS_ATTACH_ACK") + 1);
                                                write(sock_ack, id_str, strlen(id_str) + 1);
                                            }
                                            close(sock_ack);
                                        }
                                    }
                                }
                            }
                            close(sock_dest);
                        }
                    }
                    if (0 == entregado) {
                        // Si connect falla --> desconectamos al ususario
                        pthread_mutex_lock(&mutex);
                        desconectar_usuario(destinatario);
                        pthread_mutex_unlock(&mutex);

                        // Almacenamos el mensaje
                        printf("s> MESSAGE %d FROM %s TO %s STORED\n", id_mensaje, remitente, destinatario);
                    } else {
                        // Destinatario no conectado --> almacenamos mensaje
                        printf("s> MESSAGE %d FROM %s TO %s STORED\n", id_mensaje, remitente, destinatario);
                    }
                }
            }
        }
        else if (0 == strcmp(operacion, "USERS")) {
            char nombre_usuario[256];
            readLine(socket_cliente, nombre_usuario, 256);

            pthread_mutex_lock(&mutex);

            // Obtenemos la lista de usuarios conectados
            int num_conectados;
            char usuarios_conectados[256][256];
            int res = obtener_usuarios_conectados(nombre_usuario, &num_conectados, usuarios_conectados);
            
            pthread_mutex_unlock(&mutex);

            if (res == 0) {
                if (rpc_host != NULL) {
                    registrar_operacion(rpc_host, nombre_usuario, "USERS", "");
                }

                respuesta = 0;
                write(socket_cliente, &respuesta, 1);

                // Enviamos el número de usuarios conectados
                char num_str[32];
                snprintf(num_str, sizeof(num_str), "%d", num_conectados);
                write(socket_cliente, num_str, strlen(num_str) + 1);

                // Enviamos cada nombre de usuario conectado con formato --> usuario :: IP :: puerto
                for (int i = 0; i < num_conectados; i++) {
                    int estado;
                    char ip[256], puerto[256];
                    char cadena[1024];

                    // Obtenemos la IP y el puerto
                    pthread_mutex_lock(&mutex);
                    obtener_datos_conexion_usuario(usuarios_conectados[i], &estado, ip, puerto);
                    pthread_mutex_unlock(&mutex);

                    snprintf(cadena, sizeof(cadena), "%s :: %s :: %s", usuarios_conectados[i], ip, puerto);
                    write(socket_cliente, cadena, strlen(cadena) + 1);
                }

                printf("s> CONNECTEDUSERS OK\n");
            }
            else if (res == 1) {
                // Usuario no conectado
                respuesta = 1;
                write(socket_cliente, &respuesta, 1);
                printf("s> CONNECTEDUSERS FAIL\n");
            }
            else {
                // Error
                respuesta = 2;
                write(socket_cliente, &respuesta, 1);
                printf("s> CONNECTEDUSERS FAIL\n");
            }
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
    if (argc != 3) {
        printf("Usage: %s <puerto>\n", argv[0]);
        exit(-1);
    }

    // Obtenemos el puerto
    int puerto = -1;
    int opt;
    while (-1 != (opt = getopt(argc, argv, "p:"))) {
        if (opt == 'p') {
            puerto = atoi(optarg);
        }
    }

    if (puerto < 1024 || puerto > 65535) {
        printf("Usage: %s <puerto>\n", argv[0]);
        exit(-1);
    }

    // Obtenemos la IP
    char ip_local[256] = "127.0.0.1";
    int sock_ip = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_ip >= 0) {
        struct sockaddr_in addr_ip;
        memset(&addr_ip, 0, sizeof(addr_ip));
        addr_ip.sin_family = AF_INET;
        addr_ip.sin_addr.s_addr = inet_addr("8.8.8.8");
        addr_ip.sin_port = htons(53);

        if (0 == connect(sock_ip, (struct sockaddr *)&addr_ip, sizeof(addr_ip))) {
            struct sockaddr_in local_addr;
            socklen_t local_addr_len = sizeof(local_addr);
            if (0 == getsockname(sock_ip, (struct sockaddr *)&local_addr, &local_addr_len)) {
                inet_ntop(AF_INET, &local_addr.sin_addr, ip_local, sizeof(ip_local));
            }
        }
        close(sock_ip);
    }

    printf("s> init server %s:%d\n", ip_local, puerto);

    rpc_host = getenv("LOG_RPC_IP");
    if (rpc_host == NULL) {
        printf("s> Warning: LOG_RPC_IP not set. RPC logging disabled\n");
    } else {
        printf("s> RPC logging enabled\n");
    }

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

    // Inicializamos el directorio ALMACEN
    pthread_mutex_lock(&mutex);
    ret = init_almacen();
    pthread_mutex_unlock(&mutex);
    if (ret < 0) {
        printf("Error al inicializar el directorio ALMACEN\n");
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
