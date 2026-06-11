# Required imports
import numpy as np
import networkx as nx
from Boundaries import Boundaries
from Map import EPSILON

# Number of nodes expanded in the heuristic search (stored in a global variable to be updated from the heuristic functions)
NODES_EXPANDED = 0


def h1(current_node, objective_node) -> np.float32:
    """
    Primera heurística: distancia euclídea entre el nodo actual y el objetivo.
    """
    global NODES_EXPANDED
    NODES_EXPANDED += 1
    dist_vert = current_node[0] - objective_node[0]
    dist_hor = current_node[1] - objective_node[1]
    resultado = np.float32((dist_vert**2 + dist_hor**2)**0.5*EPSILON)
    return resultado


def h2(current_node, objective_node) -> np.float32:
    """ Segunda heurística: distancia manhattan entre el nodo actual y el objetivo """
    global NODES_EXPANDED
    NODES_EXPANDED += 1
    dist_vert = abs(current_node[0] - objective_node[0])
    dist_hor = abs(current_node[1] - objective_node[1])
    resultado = np.float32((dist_vert + dist_hor)*EPSILON)
    return resultado


def build_graph(detection_map: np.array, tolerance: np.float32) -> nx.DiGraph:
    """ Builds an adjacency graph (not an adjacency matrix) from the detection map """
    # The only possible connections from a point in space (now a node in the graph) are:
    #   -> Go up
    #   -> Go down
    #   -> Go left
    #   -> Go right
    # Not every point has always 4 possible neighbors
    #   -> The points in the corners have only 2 possible neighbors

    # Crear un grafo dirigido
    G = nx.DiGraph()
    altura, ancho = detection_map.shape

    #Añadir los nodos al grafo
    for i in range(altura):
        for j in range(ancho):
            G.add_node((i, j))

    #Para cada movimiento permitido se calcula la posición del nodo vecino
    for i in range(altura):
        for j in range(ancho):
            # Posibles movimientos
            moves = [(-1, 0), (1, 0), (0, -1), (0, 1)]
            for move in moves:
                vecino_i, vecino_j = i + move[0], j + move[1]

                # Comprobar si el vecino está dentro de los límites del mapa
                if 0 <= vecino_i < altura and 0 <= vecino_j < ancho:
                    coste = detection_map[vecino_i, vecino_j]

                    # Comprobar si el costo del vecino es mayor que la tolerancia
                    if coste <= tolerance:
                        # Añadir la arista al grafo
                        G.add_edge((i, j), (vecino_i, vecino_j), weight=coste)
    print(
        f"Grafo construido con {G.number_of_nodes()} nodos y {G.number_of_edges()} aristas")  # Debug
    print(f"Rango de nodos: (0,0) a ({altura - 1},{ancho - 1})")  # Debug
    return G


def discretize_coords(high_level_plan: np.array, boundaries: Boundaries, map_width: np.int32,
                      map_height: np.int32) -> np.array:
    """ Converts coordiantes from (lat, lon) into (x, y) """
    # Rango para la latitud
    lat = np.linspace(start=boundaries.min_lat, stop=boundaries.max_lat, num=map_height)
    # Rango para la longitud
    lon = np.linspace(start=boundaries.min_lon, stop=boundaries.max_lon, num=map_width)

    plano_xy = []

    # Buscar la posición mas cercana en el array de lat y lon y añadirlo al plano
    for i, j in high_level_plan:
        x = np.argmin(np.abs(lat - i))
        y = np.argmin(np.abs(lon - j))

        x = int(min(max(x, 0), map_height - 1))
        y = int(min(max(y, 0), map_width - 1))
        plano_xy.append((x, y))

    return plano_xy


def path_finding(G: nx.DiGraph,
                 heuristic_function,
                 locations: np.array,
                 initial_location_index: np.int32,
                 boundaries: Boundaries,
                 map_width: np.int32,
                 map_height: np.int32) -> tuple:
    """ Implementation of the main searching / path finding algorithm """
    global NODES_EXPANDED
    NODES_EXPANDED = 0
    solution_plan = []
    coordenadas_discretas = discretize_coords(locations, boundaries, map_width, map_height)
    print("Coordenadas discretas: ", coordenadas_discretas)

    # inicializar nodos a visitar
    to_visit = coordenadas_discretas
    visited = []

    # Discretizar el nodo inicial
    coordenada_ini = locations[initial_location_index]
    ini_discreta_list = discretize_coords([coordenada_ini], boundaries,
                                          map_width, map_height)
    ini_discreta = ini_discreta_list[0]
    current = ini_discreta

    #Buscar el nodo más cercano
    for _ in range(len(coordenadas_discretas)):
        if not to_visit:
            break
        next_node = find_closest_node_graph(G, current, to_visit)
        #En caso de no encontrarlo
        if next_node is None:
            print(f"No se encontro camino desde {current} a los nodos restantes")
            break

        #Buscar el camino óptimo con A*
        try:
            path = nx.astar_path(G, current, next_node, heuristic=heuristic_function,
                                 weight='weight')
            path_str = []
            for p in path:
                path_str.append(str(p))
            solution_plan.append(path_str)
            NODES_EXPANDED += len(path)
        #En caso de no encontrra el camino
        except nx.NetworkXNoPath:
            print("No se encontró camino entre los nodos")
            solution_plan.append([])
            #Caso de error
        except Exception as e:
            print(f"Error inesperado al encontrar el camino: {e}")
            solution_plan.append([])  # Agregar lista vacía en caso de

        visited.append(next_node)
        to_visit.remove(next_node)
        current = next_node

    return solution_plan, NODES_EXPANDED


def compute_path_cost(G, solution_plan):
    """Calcula el coste de un camino sobre el grafo G"""
    total_cost = np.float32(0.0)
    #Recorrer todos los caminos del plan de solución
    for path in solution_plan:
        #Convertir todos de string a tupla en caso de ser str
        for i in range(len(path) - 1):
            node_a = eval(path[i]) if isinstance(path[i], str) else path[i]
            node_b = eval(path[i + 1]) if isinstance(path[i + 1], str) else path[i + 1]
            #Actualizar el coste
            total_cost += np.float32(G[node_a][node_b]['weight'])
    return total_cost

def find_closest_node_graph(G, start_node, location_nodes):
    """Encuentra el nodo más cercano en el grafo usando caminos más cortos."""
    min_cost = float('inf')
    closest_node = None
    #Buscar entre todos los nodos
    for node in location_nodes:
        try:
            cost = nx.shortest_path_length(G, source=start_node, target=node, weight='weight')
            #Si el coste es menor se actualiza el noodo y el valor
            if cost < min_cost:
                min_cost = cost
                closest_node = node
        except nx.NetworkXNoPath:
            continue
    return closest_node
