#include "claves.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char * argv[]) {
    char *key = "clave-de-prueba";
    char v1[256] = "valor-1-prueba";
    float v2[256] = {2.3, 0.5, 23.45};
    int n2 = 3;
    struct Paquete v3;

    v3.x = 10;
    v3.y = 5;
    v3.z = 3;

    // ------------- BLOQUE 1: PRUEBAS DE FUNCIONALIDAD -------------

    // Prueba 1: Destruir el almacén antes de realizar cualquier operación
    printf("\nPrueba 1: Destruir el almacén...\n");
    int resultado = destroy();
    if (0 == resultado) {
        printf("Almacen destruido correctamente\n");
        printf("----- Prueba 1 realizada con éxito -----\n");
    }
    
    else {
        printf("Error al destruir el almacen\n");
        printf("----- Prueba 1 fallida -----\n");
        return -1;
    }

    // Prueba 2: Insertar con valores válidos
    printf("\nPrueba 2: Insertar con valores válidos...\n");
    int err = set_value(key, v1, n2, v2, v3);
    if (0 == err) {
        printf("Tupla insertada correctamente\n");
        printf("----- Prueba 2 realizada con éxito -----\n");
    }
    else {
        printf("Error al insertar la tupla\n");
        printf("----- Prueba 2 fallida -----\n");
        return -1;
    }

    // Prueba 3: Obtener la tupla insertada
    printf("\nPrueba 3: Obtener la tupla insertada...\n");
    int valor = get_value(key, v1, &n2, v2, &v3);
    if (0 == valor) {
        printf("Tupla obtenida correctamente\n");
        printf("----- Prueba 3 realizada con éxito -----\n");
    }
    else {
        printf("Error al obtener la tupla\n");
        printf("----- Prueba 3 fallida -----\n");
        return -1;
    }

    // Prueba 4: Comprobar existencia
    printf("\nPrueba 4: Comprobar existencia...\n");
    int existe = exist(key);
    if (existe == 1) {
        printf("La clave existe\n");
        printf("----- Prueba 4 realizada con éxito -----\n");
    }
    else {
        printf("Error: la clave no existe\n");
        printf("----- Prueba 4 fallida -----\n");
        return -1;
    }

    // Prueba 5: Modificar la tupla insertada
    printf("\nPrueba 5: Modificar la tupla insertada...\n");
    char nuevo_v1[256] = "valor-1-actualizado";
    float nuevo_v2[256] = {
        1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 
        9.9, 10.1, 11.11, 12.12, 13.13, 14.14, 15.15, 16.16,
        17.17, 18.18, 19.19, 20.2, 21.21, 22.22, 23.23, 24.24,
        25.25, 26.26, 27.27, 28.28, 29.29, 30.3, 31.31, 32.32
    };
    int nuevo_n2 = 32;
    struct Paquete nuevo_v3;
    nuevo_v3.x = 100;
    nuevo_v3.y = 200;
    nuevo_v3.z = 300;

    int modificado = modify_value(key, nuevo_v1, nuevo_n2, nuevo_v2, nuevo_v3);

    if (0 == modificado) {
        printf("Modificación realizada correctamente\n");
        // Comprobamos leyendo de nuevo
        get_value(key, v1, &n2, v2, &v3);
        printf("----- Prueba 5 realizada con éxito -----\n");
    }
    else {
        printf("Error al modificar la tupla\n");
        printf("----- Prueba 5 fallida -----\n");
        return -1;
    }


    // Prueba 6: Eliminar la tupla
    printf("\nPrueba 6: Eliminar la tupla...\n");
    int eliminado = delete_key(key);
    if (0 == eliminado) {
        printf("Clave eliminada correctamente\n");
        printf("----- Prueba 6 realizada con éxito -----\n");
    }
    else {
        printf("Error al eliminar la clave\n");
        printf("----- Prueba 6 fallida -----\n");
        return -1;
    }


    // ------------- BLOQUE 2: CONTROL DE ERRORES Y LÍMITES -------------
    char *key_err = "clave-control-errores";
    set_value(key_err, v1, n2, v2, v3); // Insertamos una tupla para realizar pruebas de errores
    
    // Prueba 7: Insertar con clave ya existente
    printf("\nPrueba 7: Insertar con clave ya existente...\n");
    int err_clave_existente = set_value(key_err, v1, n2, v2, v3);
    if (-1 == err_clave_existente) {
        printf("El programa bloqueó la inserción duplicada\n");
        printf("----- Prueba 7 realizada con éxito -----\n");
    }
    else {
        printf("Error: el programa no bloqueó la inserción duplicada\n");
        printf("----- Prueba 7 fallida -----\n");
        return -1;
    }

    // Prueba 8: Obtener una clave inexistente
    printf("\nPrueba 8: Obtener una clave inexistente...\n");
    char *clave_inexistente = "clave-inexistente";
    int err_clave_inexistente = get_value(clave_inexistente, v1, &n2, v2, &v3);
    if (-1 == err_clave_inexistente) {
        printf("El programa bloqueó la obtención de una clave inexistente\n");
        printf("----- Prueba 8 realizada con éxito -----\n");
    }
    else {
        printf("Error: el programa no bloqueó la obtención de una clave inexistente\n");
        printf("----- Prueba 8 fallida -----\n");
        return -1;
    }

    // Prueba 9: Comprobar existencia de una clave inexistente
    printf("\nPrueba 9: Comprobar existencia de una clave inexistente...\n");
    int err_existencia_inexistente = exist(clave_inexistente);
    if (0 == err_existencia_inexistente) {
        printf("El programa indicó que la clave no existe\n");
        printf("----- Prueba 9 realizada con éxito -----\n");
    }
    else {        
        printf("Error: el programa no indicó que la clave no existe\n");
        printf("----- Prueba 9 fallida -----\n");
        return -1;
    }

    // Prueba 10: Modificar una clave inexistente
    printf("\nPrueba 10: Modificar una clave inexistente...\n");
    int err_modificar_inexistente = modify_value(clave_inexistente, v1, n2, v2, v3);
    if (-1 == err_modificar_inexistente) {
        printf("El programa bloqueó la modificación de una clave inexistente\n");
        printf("----- Prueba 10 realizada con éxito -----\n");
    }
    else {
        printf("Error: el programa no bloqueó la modificación de una clave inexistente\n");
        printf("----- Prueba 10 fallida -----\n");
        return -1;
    }

    // Prueba 11: Eliminar una clave inexistente
    printf("\nPrueba 11: Eliminar una clave inexistente...\n");
    int err_eliminar_inexistente = delete_key(clave_inexistente);
    if (-1 == err_eliminar_inexistente) {
        printf("El programa bloqueó la eliminación de una clave inexistente\n");
        printf("----- Prueba 11 realizada con éxito -----\n");
    }
    else {
        printf("Error: el programa no bloqueó la eliminación de una clave inexistente\n");
        printf("----- Prueba 11 fallida -----\n");
        return -1;
    }

    // Prueba 12: Límite de longitud de clave
    printf("\nPrueba 12: Límite de longitud de clave (>255)...\n");
    char clave_larga[300];
    // Rellenamos la cadena con a's y lo finalizamos con \0
    memset(clave_larga, 'a', 299);
    clave_larga[299] = '\0';
    int err_clave_larga = set_value(clave_larga, v1 , n2, v2, v3);
    if (-1 == err_clave_larga) {
        printf("El programa bloqueó la inserción de una clave con longitud mayor a 255 caracteres\n");
        printf("----- Prueba 12 realizada con éxito -----\n");
    }
    else {
        printf("Error: el programa no bloqueó la inserción de una clave con longitud mayor a 255 caracteres\n");
        printf("----- Prueba 12 fallida -----\n");
        return -1;
    }

    // Prueba 13: Límite de longitud de value1
    printf("\nPrueba 13: Límite de longitud de value1 (>255)...\n");
    char value1_largo[300];
    // Rellenamos la cadena con b's y lo finalizamos con \0
    memset(value1_largo, 'b', 299);
    value1_largo[299] = '\0';
    int err_value1_largo = set_value(key, value1_largo, n2, v2, v3);
    if (-1 == err_value1_largo) {
        printf("El programa bloqueó la inserción de un value1 con longitud mayor a 255 caracteres\n");
        printf("----- Prueba 13 realizada con éxito -----\n");
    }
    else {
        printf("Error: el programa no bloqueó la inserción de un value1 con longitud mayor a 255 caracteres\n");
        printf("----- Prueba 13 fallida -----\n");
        return -1;
    }

    // Prueba 14: Límite inferior de N_value2
    printf("\nPrueba 14: Límite inferior de N_value2 (<1)...\n");
    int err_value2_bajo = set_value(key, v1, 0, v2, v3);
    if (-1 == err_value2_bajo) {
        printf("El programa bloqueó la inserción de un N_value2 con longitud menor a 1\n");
        printf("----- Prueba 14 realizada con éxito -----\n");
    }
    else {
        printf("Error: el programa no bloqueó la inserción de un N_value2 con longitud menor a 1\n");
        printf("----- Prueba 14 fallida -----\n");
        return -1;
    }

    // Prueba 15: Límite superior de N_value2
    printf("\nPrueba 15: Límite superior de N_value2 (>32)...\n");
    int err_value2_alto = set_value(key, v1, 33, v2, v3);
    if (-1 == err_value2_alto) {
        printf("El programa bloqueó la inserción de un N_value2 con longitud mayor a 32\n");
        printf("----- Prueba 15 realizada con éxito -----\n");
    }
    else {
        printf("Error: el programa no bloqueó la inserción de un N_value2 con longitud mayor a 32\n");
        printf("----- Prueba 15 fallida -----\n");
        return -1;
    }

    // Destruimos el almacén al finalizar las pruebas
    printf("\nDestruyendo el almacén al finalizar las pruebas...\n");
    int resultado_destruccion = destroy();
    if (0 == resultado_destruccion) {
        printf("Almacen destruido correctamente\n");
        printf("----- Destrucción realizada con éxito -----\n");
    }
    else {
        printf("Error al destruir el almacen\n");
        printf("----- Destrucción fallida -----\n");
        return -1;
    }

    printf("Todas las pruebas se han ejecutado correctamente\n");
    return 0;
        
}