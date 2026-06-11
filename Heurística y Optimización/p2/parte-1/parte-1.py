"""
parte-1.py
Resolución del problema de satisfacción de restricciones (CSP) para el juego Binairo.
Uso: python parte-1.py <fichero_entrada> <fichero_salida>
"""

import sys
import os
import math
import constraint

def ruta_absoluta(nombre, carpeta_script):
    """
    Obtiene la ruta absoluta de un archivo.
    """
    if os.path.dirname(nombre) == "":
        return os.path.abspath(os.path.join(carpeta_script, nombre))
    return os.path.abspath(nombre)

def leer_entrada(fichero_entrada):
    """
    Lee el estado inicial del tablero desde un fichero.
    Devuelve una lista plana con los caracteres y la dimensión n del tablero.
    Lanza error si los datos no se corresponden con los de una matriz cuadrada.
    """
    matriz = []
    with open(fichero_entrada, "r", encoding="utf-8") as f:
        for linea in f:
            fila = linea.strip()
            if not fila:
                continue
            for char in fila:
                matriz.append(char)

    n = int(math.sqrt(len(matriz)))
    if n * n != len(matriz):
        raise ValueError("Error: El número total de elementos no corresponde a una matriz cuadrada (NxN).")
    
    return matriz, n

def escribir_salida(fichero_salida, soluciones, original, n):
    """
    Genera el fichero de salida con el formato especificado.
    Muestra primero la instancia original y a continuación la primera solución encontrada (si existe).
    """
    with open(fichero_salida, "w", encoding="utf-8") as f:
        separador = "+---" * n + "+\n"

        # Escribir instancia original
        f.write("INSTANCIA ORIGINAL\n")
        f.write(separador)
        for i in range(n):
            linea = "|"
            for j in range(n):
                idx = i * n + j
                val = original[idx]
                if val == '.': 
                    val = ' ' 
                linea += f" {val} |"
            f.write(linea + "\n")
            f.write(separador)

        f.write("\n")

        # Escribir solución
        if soluciones:
            f.write("SOLUCIÓN\n")
            f.write(separador)
            primera_solucion = soluciones[0]
            for i in range(n):
                linea = "|"
                for j in range(n):
                    idx = i * n + j
                    # Mapeo inverso: 0 -> 'O' (Blanco), 1 -> 'X' (Negro)
                    val_num = primera_solucion[idx]
                    simbolo = 'X' if val_num == 1 else 'O'
                    linea += f" {simbolo} |"
                f.write(linea + "\n")
                f.write(separador)
        else:
            f.write("No se han encontrado soluciones factibles.")

def mostrar_instancia(original, n):
    """
    Imprime la instancia original por consola
    """
    separador = "+---" * n + "+"
    print(separador)
    for i in range(n):
        linea = "|"
        for j in range(n):
            idx = i * n + j
            val = original[idx]
            if val == '.':
                val = ' '
            linea += f" {val} |"
        print(linea)
        print(separador)
    print()

# --- Funciones de Restricción ---

def mitad_blancos(*valores):
    """
    Restricción: El número de casillas blancas (0) debe ser igual al de negras (1).
    En una fila/columna de longitud L, la suma de valores (cantidad de 1s) debe ser L/2.
    """
    return 2 * sum(valores) == len(valores)

def no_tres_seguidos(a, b, c):
    """
    Restricción: No se permiten tres casillas consecutivas del mismo color.
    """
    return not (a == b == c)

def construir_modelo(matriz, n):
    """
    Construye y configura el problema CSP
    
    Variables:
        - Cada casilla (i*n + j) es una variable.
    Dominios:
        - Casillas preasignadas ('O' o 'X'): Dominio fijo [0] o [1].
        - Casillas vacías ('.'): Dominio binario [0, 1].
    Restricciones:
        1. Balance de colores por fila y columna.
        2. Secuencia máxima de 2 colores iguales consecutivos.
    """
    problema = constraint.Problem()

    # Definición de variables y dominios iniciales
    for i in range(len(matriz)):
        if matriz[i] == 'O':      # Blanco fijo
            problema.addVariable(i, [0])
        elif matriz[i] == 'X':    # Negro fijo
            problema.addVariable(i, [1])
        else:                     # Desconocido
            problema.addVariable(i, [0, 1])
    
    # Restricción de balance (mismo nº de blancos y negros)
    # Aplicar a filas
    for i in range(n):
        indices_fila = [i * n + j for j in range(n)]
        problema.addConstraint(mitad_blancos, indices_fila)

    # Aplicar a columnas
    for j in range(n):
        indices_columna = [i * n + j for i in range(n)]
        problema.addConstraint(mitad_blancos, indices_columna)

    # Restricción de adyacencia (máximo 2 iguales consecutivos)
    # Horizontal (filas)
    for i in range(n):
        for j in range(n - 2):
            # Terna de celdas consecutivas en fila
            variables = [i * n + j, i * n + j + 1, i * n + j + 2]
            problema.addConstraint(no_tres_seguidos, variables)
    
    # Vertical (columnas)
    for i in range(n - 2):
        for j in range(n):
            # Terna de celdas consecutivas en columna
            variables = [i * n + j, (i + 1) * n + j, (i + 2) * n + j]
            problema.addConstraint(no_tres_seguidos, variables)

    return problema

def main():
    # Validación de argumentos
    if len(sys.argv) != 3:
        print("Uso incorrecto.\nSintaxis: python parte-1.py <fichero_entrada> <fichero_salida>")
        sys.exit(-1)

    path_entrada = sys.argv[1]
    path_salida = sys.argv[2]

    # Resolución de rutas
    carpeta_base = os.path.abspath(os.path.dirname(__file__))
    fichero_in = ruta_absoluta(path_entrada, carpeta_base)
    fichero_out = ruta_absoluta(path_salida, carpeta_base)

    try:
        # Carga y modelado del problema
        matriz, n = leer_entrada(fichero_in)

        problema = construir_modelo(matriz, n)

        # Búsqueda de soluciones
        soluciones = problema.getSolutions()

        # Salida por consola
        mostrar_instancia(matriz, n)
        print(f"#{len(soluciones)} soluciones encontradas.")

        # Generación de fichero de salida
        escribir_salida(fichero_out, soluciones, matriz, n)
        print(f"Resultado guardado en: {fichero_out}")

    except Exception as e:
        print(f"Error durante la ejecución: {e}")
        sys.exit(-1)

if __name__ == '__main__':
    main()