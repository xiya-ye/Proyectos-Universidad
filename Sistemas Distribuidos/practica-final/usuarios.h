#ifndef USUARIOS_H
#define USUARIOS_H

struct MensajePendiente {
    unsigned int id;
    char remitente[256];
    char mensaje[256];
    char fichero[256];
};

/**
 * @brief Inicializa el directorio ALMACEN.
 * @return int El servicio devuelve 0 si se insertó con éxito y -1 en caso de error.
 * @retval 0 en caso de éxito.
 * @retval -1 en caso de error.
 */
int init_almacen(void);

/**
 * @brief Registra un usuario nuevo en el sistema (REGISTER)
 * @param nombre Nombre del usuario.
 * @return int El servicio devuelve 0 si se insertó con éxito, 1 si ya existe y 2 si hay error
 * @retval 0 en caso de éxito.
 * @retval 1 si el usuario ya existe.
 * @retval 2 si hay un error.
 */
int registrar_usuario(char *nombre);

/**
 * @brief Da de baja a un usuario del sistema y borra sus mensajes (UNREGISTER)
 * @param nombre Nombre del usuario.
 * @return int El servicio devuelve 0 si se insertó con éxito, 1 si no existe y 2 si hay error
 * @retval 0 en caso de éxito.
 * @retval 1 si el usuario no existe.
 * @retval 2 si hay un error.
 */
int baja_usuario(char *nombre);

/**
 * @brief Conecta a un usuario en el sistema, guardand su IP y puerto (CONNECT)
 * @param nombre Nombre del usuario.
 * @param ip IP del usuario.
 * @param puerto Puerto del usuario.
 * @return int El servicio devuelve 0 si se insertó con éxito, 1 si el usuario no existe, 2 si el usuario ya está conectado y 3 si hay error
 * @retval 0 en caso de éxito.
 * @retval 1 si el usuario no existe.
 * @retval 2 si el usuario ya está conectado.
 * @retval 3 si hay un error.
 */
int conectar_usuario(char *nombre, char *ip, char *puerto);

/**
 * @brief Obtiene la lista de mensajes pendientes de un usuario (para CONNECT)
 * @param usuario Nombre del usuario.
 * @param num_mensajes por referencia, devuelve cuántos mensajes pendientes tiene el usuario
 * @param mensajes por referencia, devuelve un array dinamico con los mensajes pendientes
 * @return int El servicio devuelve 0 si se obtuvo la lista con éxito y -1 si hay error
 * @retval 0 en caso de éxito.
 * @retval -1 si hay un error.
 */
int obtener_mensajes_pendientes(char *usuario, int *num_mensajes, struct MensajePendiente **mensajes);

/**
 * @brief Desconecta a un usuario del sistema (DISCONNECT).
 * @param nombre Nombre del usuario.
 * @return int El servicio devuelve 0 si se insertó con éxito, 1 si no existe, 2 si no está conectado y 3 si hay error
 * @retval 0 en caso de éxito.
 * @retval 1 si el usuario no existe.
 * @retval 2 si el usuario no está conectado.
 * @retval 3 si hay un error.
 */
int desconectar_usuario(char *nombre);

/**
 * @brief Genera el siguiente ID de mensaje para un remitente (SEND)
 * @param remitente Nombre del usuario [256].
 * @return int El servicio devuelve el ID si se generó con éxito y -1 si hay error
 * @retval ID del mensaje en caso de éxito.
 * @retval -1 si hay un error.
 */
int obtener_siguiente_id_mensaje(char *remitente);

/**
 * @brief Guarda un mensaje en el archivo del destinatario (SEND)
 * @param destinatario Nombre del usuario destinatario.
 * @param remitente Nombre del usuario remitente.
 * @param id ID del mensaje.
 * @param mensaje Contenido del mensaje.
 * @param fichero Nombre del fichero adjunto.
 * @return int El servicio devuelve 0 si se guardó correctamente, 1 si el destinatario no existe y 2 si hay error
 * @retval 0 en caso de éxito.
 * @retval 1 si el destinatario no existe.
 * @retval 2 si hay un error.
 */
int guardar_mensaje_pendiente(char *destinatario, char *remitente, unsigned int id, char *mensaje, char *fichero);

/**
 * @brief Obtiene los datos de conexión de un usuario (SEND_MESSAGE)
 * @param nombre Nombre del usuario.
 * @param estado Estado de conexión del usuario (0 = desconectado, 1 = conectado).
 * @param ip IP del usuario.
 * @param puerto Puerto del usuario.
 * @return int El servicio devuelve 0 si se obtuvieron los datos con éxito y -1 si hay error
 * @retval 0 en caso de éxito.
 * @retval -1 si hay un error.
 */
int obtener_datos_conexion_usuario(char *nombre, int *estado, char *ip, char *puerto);

/**
 * @brief Marca un mensaje como entregado eliminándolo de los pendientes (SEND_MESS_ACK)
 * @param nombre Nombre del usuario.
 * @param id ID del mensaje.
 * @return int El servicio devuelve 0 si se eliminó con éxito y -1 si hay error
 * @retval 0 en caso de éxito.
 * @retval -1 si hay un error.
 */
int eliminar_mensaje_pendiente(char *nombre, unsigned int id);

/**
 * @brief Obtiene la lista de todos los usuarios conectados (USERS)
 * @param usuario_pet Nombre del usuario que solicita la lista.
 * @param num_conectados por referencia, devuelve cuántos usuarios están conectados.
 * @param conectados Matriz donde se guardan los nombres.
 * @return int El servicio devuelve 0 si se obtuvo la lista con éxito, 1 si el usuario solicitante no está conectado y 2 si hay error
 * @retval 0 en caso de éxito.
 * @retval 1 si el usuario no está conectado.
 * @retval 2 si hay un error.
 */
int obtener_usuarios_conectados(char *usuario_pet, int *num_conectados, char conectados[][256]);

#endif