"""
cerrada.py
Implementación de la Lista Cerrada para el control de nodos visitados en algoritmos de búsqueda.
"""

class Cerrada:
    """
    Gestiona el conjunto de nodos que ya han sido expandidos por el algoritmo.
    """

    def __init__(self):
        """
        Inicializa la lista cerrada vacía.
        """
        self.visitados = set()

    def insert(self, nodo_id):
        """
        Marca un nodo como visitado añadiéndolo a la colección.
        """
        self.visitados.add(nodo_id)

    def membership(self, nodo_id):
        """
        Verifica si un nodo ya ha sido visitado previamente.
        """
        return nodo_id in self.visitados
