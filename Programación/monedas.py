"""
Created by Marta Vindel in nov 2023 in version 3.10
"""
import constantes
import pyxel

class Mario:
    """Esta clase almacena la información necesaria para Mario."""

    def __init__(self, x: int, y: int):
        """ Este método crea el objeto Mario
        @param x -> la posición x inicial de Mario
        @param y -> la posición y inicial de Mario
        """
        self.x = x
        self.y = y
        self.velocidadx = 1
        self.velocidady = 0
        self.contador = 0
        self.sprite = (0,32,16,16,16)
        # Establecemos que tiene tres vidas al principio del juego
        self.vidas = 3

#Definimos las propiedades y los setters
    # Propiedad x
    @property
    def x(self) -> int:
        return self.__x
        #Este método se utiliza para dar valor a los atributos
    @x.setter
    def x(self, x: int):
        self.__x = x

    # Propiedad y
    @property
    def y(self) -> int:
        return self.__y
    # Este método se utiliza para dar valor a los atributos
    @y.setter
    def y(self, y: int):
        self.__y = y

    # Propiedad vidas
    @property
    def vidas(self) -> int:
        return self.__vidas
    # Este método se utiliza para dar valor a los atributos
    @vidas.setter
    def vidas(self, vidas: int):
        self.__vidas = vidas



# Con este método le cambiamos el sprite a mario dependiendo de
# hacia que lado vaya
    def cambia_sentido(self, direccion):
        if direccion == "derecha":
            self.sprite = (0,32,16,16,16)
        else:
            self.sprite = (0,32,16,-16,16)


    def mover(self, direccion: str, tamaño: int):
        """Esto es un método para mover a Mario horizontalmente.
        Cuando llega a los extremos aparece por el otro lado."""
        # Calculamos el ancho de Mario para poder hacer las comprobaciones
        # necesarias parar a Mario antes de alcanzar el borde derecho
        self.cambia_sentido(direccion)
        if (direccion.lower() == "derecha"):
            if self.x < 200:
                self.x += self.velocidadx
                self.sentido_dibujo_mario = 1
            else:
                self.x = -10
                self.sentido_dibujo_mario = 1
        elif (direccion.lower() == "izquierda"):
            if self.x > -10:
                self.x -= self.velocidadx
                self.sentido_dibujo_mario = 2
            else:
                self.x = 200
                self.sentido_dibujo_mario = 2

    def update(self):
        self.y=self.y+self.velocidady
        self.caer()

# Este método permite saltar a Mario
    def saltar(self, direccion: str, tamaño: int):
        self.velocidady-=6.5

# Este método hace que mario caiga en caso de no estar sobre el suelo
    def caer(self):
        if self.y < constantes.SUELO:
            self.velocidady+=constantes.GRAVEDAD
        else:
            self.velocidady=0
            self.y=constantes.SUELO




