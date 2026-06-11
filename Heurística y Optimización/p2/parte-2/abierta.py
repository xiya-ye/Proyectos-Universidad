"""
abierta.py
Implementación de la Lista Abierta (Priority Queue) utilizando la estructura de datos
"""

class Abierta:
    """
    Gestiona la colección de nodos candidatos a ser expandidos.
    """

    def __init__(self):
        """Inicializa la estructura de datos."""
        self.buckets = {}
        # Puntero para rastrear el coste mínimo actual y evitar búsquedas lineales
        self.min_key = 0
        # Contador de elementos totales en la estructura
        self.count = 0

    def insert(self, nodo_id, prioridad):
        """
        Inserta un nodo en la lista abierta con una prioridad dada.
        """
        prioridad_int = int(prioridad)

        # Si no existe un bucket para esta prioridad, se inicializa
        if prioridad_int not in self.buckets:
            self.buckets[prioridad_int] = []

        # Inserción en tiempo constante al final de la lista
        self.buckets[prioridad_int].append(nodo_id)

        # Mantenimiento del puntero al mínimo:
        # Si es el primer elemento o la nueva prioridad es menor que la actual mínima, actualizamos.
        if self.count == 0:
            self.min_key = prioridad_int
        elif prioridad_int < self.min_key:
            self.min_key = prioridad_int

        self.count += 1

    def pop(self):
        """
        Extrae y devuelve el nodo con la prioridad más baja.
        """
        if self.count == 0:
            return None

        # Avanzamos el puntero min_key hasta encontrar el siguiente bucket no vacío.
        while self.min_key not in self.buckets or not self.buckets[self.min_key]:
            self.min_key += 1

        # Extraemos el último elemento añadido al bucket
        nodo = self.buckets[self.min_key].pop()

        # Eliminamos la entrada del diccionario si el bucket queda vacío
        if not self.buckets[self.min_key]:
            del self.buckets[self.min_key]

        self.count -= 1
        return nodo, self.min_key
