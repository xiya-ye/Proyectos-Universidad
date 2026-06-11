"""
grafo.py
Definición de la estructura de datos para representar y manipular el grafo de carreteras.
"""

class Grafo:
    """
    Representa el mapa de carreteras como un grafo dirigido ponderado.
    """

    def __init__(self, fichero_co, fichero_gr):
        """
        Inicializa el grafo y carga los datos desde los ficheros proporcionados.
        """
        self.adyacencia = {}
        self.coordenadas = {}
        self.num_vertices = 0
        self.num_arcos = 0

        # Carga secuencial de datos
        self._cargar_coordenadas(fichero_co)
        self._cargar_arcos(fichero_gr)

    def _cargar_coordenadas(self, fichero):
        """
        Parsea el fichero de coordenadas (.co).
        Formato esperado por línea: 'v <id> <longitud> <latitud>'
        """
        try:
            with open(fichero, 'r', encoding='utf-8') as f:
                for linea in f:
                    # Filtrar líneas de metadatos o comentarios
                    if not linea.startswith('v '):
                        continue

                    partes = linea.split()
                    nodo_id = int(partes[1])
                    
                    # Extracción de coordenadas
                    long_int = int(partes[2]) 
                    lat_int = int(partes[3])

                    # Normalización a float
                    longitud = long_int / 1000000.0
                    latitud = lat_int / 1000000.0

                    self.coordenadas[nodo_id] = (longitud, latitud)
                    self.num_vertices += 1

        except FileNotFoundError:
            print(f"Error crítico: No se encuentra el fichero de coordenadas: {fichero}")
            raise

    def _cargar_arcos(self, fichero):
        """
        Parsea el fichero de definición del grafo (.gr).
        Formato esperado por línea: 'a <origen> <destino> <coste>'
        Se construye la lista de adyacencia dirigida.
        """
        try:
            with open(fichero, 'r', encoding='utf-8') as f:
                for linea in f:
                    if not linea.startswith('a '):
                        continue

                    partes = linea.split()
                    
                    # Conversión explícita a enteros para optimizar memoria y cálculos
                    u = int(partes[1])      # Nodo origen
                    v = int(partes[2])      # Nodo destino
                    coste = int(partes[3])  # Peso del arco (distancia en metros)

                    if u not in self.adyacencia:
                        self.adyacencia[u] = []
                    
                    self.adyacencia[u].append((v, coste))
                    self.num_arcos += 1

        except FileNotFoundError:
            print(f"Error crítico: No se encuentra el fichero del grafo: {fichero}")
            raise

    def get_num_vertices(self):
        """
        Retorna el número total de vértices cargados.
        """
        return self.num_vertices

    def get_num_arcos(self):
        """
        Retorna el número total de arcos cargados.
        """
        return self.num_arcos

    def get_adyacentes(self, nodo):
        """
        Obtiene la lista de vecinos de un nodo dado.
        """
        return self.adyacencia.get(nodo, [])

    def get_coordenadas(self, nodo):
        """
        Recupera las coordenadas geográficas de un nodo.
        """
        return self.coordenadas.get(nodo)

    def get_coste_arco(self, u, v):
        """
        Busca el coste de un arco específico entre u y v.
        """
        for vecino, coste in self.get_adyacentes(u):
            if vecino == v:
                return coste
        return 0