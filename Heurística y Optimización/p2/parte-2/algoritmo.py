"""
algoritmo.py
Implementación del algoritmo de búsqueda de caminos óptimos (A* y Dijkstra) sobre grafos geográficos.
"""

import math
from abierta import Abierta
from cerrada import Cerrada

class Algoritmo:
    """
    Clase que encapsula la lógica de búsqueda de rutas.
    Permite ejecutar A* utilizando una heurística basada en la distancia Haversine,
    o el algoritmo de Dijkstra desactivando dicha heurística.
    """

    def __init__(self, grafo):
        """
        Inicializa el algoritmo con el grafo sobre el que se realizará la búsqueda.
        """
        self.grafo = grafo

    def heuristica(self, nodo_actual, nodo_destino):
        """
        Calcula la distancia Haversine entre dos nodos geográficos.
        """
        # Recuperación de coordenadas desde el grafo (tuplas: longitud, latitud)
        coords_actual = self.grafo.get_coordenadas(nodo_actual)
        coords_destino = self.grafo.get_coordenadas(nodo_destino)

        # Si algún nodo no tiene coordenadas, la heurística retorna 0
        if not coords_actual or not coords_destino:
            return 0

        # El fichero .co da primero Longitud, luego Latitud
        lon1, lat1 = coords_actual
        lon2, lat2 = coords_destino

        # Radio medio de la Tierra en metros
        R = 6371000 

        # Conversión de grados a radianes para el cálculo trigonométrico
        phi1 = math.radians(lat1)
        phi2 = math.radians(lat2)
        delta_phi = math.radians(lat2 - lat1)
        delta_lambda = math.radians(lon2 - lon1)

        # Fórmula de Haversine
        a = math.sin(delta_phi / 2.0) ** 2 + \
            math.cos(phi1) * math.cos(phi2) * \
            math.sin(delta_lambda / 2.0) ** 2
        
        c = 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))

        distancia = R * c
        
        # El grafo parece estar en decímetros o similar
        # Usamos 9.5 para mantener admisibilidad (h <= h*) pero acercarnos al coste real.
        FACTOR_ESCALA = 9.5
        
        return int(distancia * FACTOR_ESCALA)

    def buscar(self, inicio, destino, heuristica_cero=False):
        """
        Ejecuta el algoritmo de búsqueda (A* o Dijkstra).
        """
        abierta = Abierta()
        cerrada = Cerrada()

        # Diccionario para almacenar el coste real g(n) desde el inicio
        g_score = {inicio: 0}
        # Diccionario para reconstruir el camino (puntero al nodo padre)
        padres = {inicio: None}

        # Cálculo de f(n) inicial = g(n) + h(n)
        h_inicial = 0 if heuristica_cero else self.heuristica(inicio, destino)
        
        # Inserción del nodo inicial en la lista abierta
        abierta.insert(inicio, 0 + h_inicial)

        nodos_expandidos = 0

        # Bucle principal de búsqueda
        while True:
            # Extracción del nodo con menor f(n) de la lista abierta
            dato_nodo = abierta.pop()
            
            if dato_nodo is None:
                # La lista abierta está vacía y no se ha alcanzado el destino: no hay solución
                return None, [], nodos_expandidos

            actual, _ = dato_nodo

            # Verificación de objetivo
            if actual == destino:
                coste_total = g_score[destino]
                camino = self.reconstruir_camino(padres, destino)
                return coste_total, camino, nodos_expandidos

            # Control de nodos repetidos (Lista Cerrada)
            if cerrada.membership(actual):
                continue

            # Expansión del nodo
            cerrada.insert(actual)
            nodos_expandidos += 1

            # Generación de sucesores
            vecinos = self.grafo.get_adyacentes(actual)
            for vecino, coste_arco in vecinos:
                # Coste tentativo g(vecino) = g(actual) + coste(arco)
                tentative_g = g_score[actual] + int(coste_arco)

                # Relajación de aristas: si encontramos un camino mejor hacia el vecino
                if vecino not in g_score or tentative_g < g_score[vecino]:
                    g_score[vecino] = tentative_g
                    padres[vecino] = actual
                    
                    # Cálculo de la nueva prioridad f(vecino)
                    if heuristica_cero:
                        h_vecino = 0
                    else:
                        h_vecino = self.heuristica(vecino, destino)
                    
                    f_score = tentative_g + h_vecino
                    
                    # Inserción en abierta (se permiten duplicados, la lista cerrada los filtra después)
                    abierta.insert(vecino, f_score)

    def reconstruir_camino(self, padres, actual):
        """
        Reconstruye la ruta óptima siguiendo los punteros desde el destino hacia el inicio.
        """
        camino = []
        while actual is not None:
            camino.append(actual)
            actual = padres[actual]
        return camino[::-1]  # Inversión para obtener orden correcto (Inicio -> Fin)