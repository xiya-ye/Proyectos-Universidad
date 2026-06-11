"""
parte-2.py
Script principal para la resolución del problema de búsqueda de caminos óptimos en mapas.
Carga un grafo desde ficheros DIMACS, ejecuta el algoritmo de búsqueda (A*) y genera
las estadísticas y el fichero de solución requerido.
"""

import sys
import os
import time
from grafo import Grafo
from algoritmo import Algoritmo

def ruta_absoluta(nombre, carpeta_script):
    """
    Resuelve la ruta absoluta de un archivo.
    """
    if os.path.dirname(nombre) == "":
        return os.path.abspath(os.path.join(carpeta_script, nombre))
    return os.path.abspath(nombre)


def escribir_solucion(fichero, camino, grafo):
    """
    Escribe la solución en el fichero de salida.
    """
    with open(fichero, 'w', encoding='utf-8') as f:
        if not camino:
            return

        # Añadir nodo inicial
        piezas = [str(camino[0])]

        # Añadir la secuencia de arcos con sus costes
        for i in range(len(camino) - 1):
            u = camino[i]
            v = camino[i + 1]
            coste = grafo.get_coste_arco(u, v)

            piezas.append(f"({coste})")
            piezas.append(str(v))

        # Unir todo con guiones
        linea_final = " - ".join(piezas)

        f.write(linea_final)

def main():
    # Validación de argumentos de entrada
    if len(sys.argv) != 5:
        print("Uso incorrecto.\nSintaxis: python parte-2.py <vertice_origen> <vertice_destino> <nombre_mapa> <fichero_salida>\n")
        sys.exit(-1)

    try:
        # Parsing de argumentos
        vertice_origen = int(sys.argv[1])
        vertice_destino = int(sys.argv[2])
        nombre_mapa = sys.argv[3]
        fichero_salida = sys.argv[4]

        # Resolución de rutas de archivos (.gr y .co deben estar junto al script)
        carpeta = os.path.abspath(os.path.dirname(__file__))
        fichero_gr = ruta_absoluta(f"{nombre_mapa}.gr", carpeta)
        fichero_co = ruta_absoluta(f"{nombre_mapa}.co", carpeta)
        fd_sal = ruta_absoluta(fichero_salida, carpeta)

        # Verificación de existencia de ficheros de datos
        if not os.path.exists(fichero_gr) or not os.path.exists(fichero_co):
            print(f"Error crítico: No se encuentran los ficheros de datos {fichero_gr} o {fichero_co}")
            sys.exit(-1)

        # Carga del Grafo
        grafo = Grafo(fichero_co, fichero_gr)

        # Ejecución del Algoritmo de Búsqueda (A*)
        tiempo_inicio = time.time()

        solver = Algoritmo(grafo)
        # Se puede activar el modo Dijkstra pasando heuristica_cero=True
        coste_total, camino, nodos_expandidos = solver.buscar(vertice_origen, vertice_destino)

        tiempo_fin = time.time()
        tiempo_total = tiempo_fin - tiempo_inicio

        # Reporte de Estadísticas por pantalla
        print(f"# vertices: {grafo.get_num_vertices()}")
        print(f"# arcos: {grafo.get_num_arcos()}")

        if coste_total is None:
            print("No se encontró solución (destino inalcanzable).")
        else:
            print(f"Solución óptima encontrada con coste {coste_total}")
            print(f"Tiempo de ejecución: {tiempo_total:.4f} segundos")

            if tiempo_total > 0:
                nodos_seg = nodos_expandidos / tiempo_total
                print(f"# expansiones: {nodos_expandidos} ({nodos_seg:.2f} nodes/sec)")
            else:
                print(f"# expansiones: {nodos_expandidos}")

        # Generación del fichero de solución
        escribir_solucion(fd_sal, camino, grafo)
        print(f"Solución guardada en {fichero_salida}")

    except ValueError:
        print("Error: Los identificadores de vértice deben ser números enteros.")
        sys.exit(-1)
    except Exception as e:
        print(f"Error inesperado: {e}")
        sys.exit(-1)

if __name__ == '__main__':
    main()
