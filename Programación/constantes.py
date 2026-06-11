"""
Created by Marta Vindel in dic 2023 in version 3.10
"""
import random
import constantes
class Cangrejo:


    def __init__(self):

        self.direccion = random.randint(1, 2)

        if (self.direccion == 1):
            self.x = 30
            self.y = 20
        else:
            self.x = 160
            self.y = 20
        self.velocidadx = 1
        self.velocidady = 0
        self.numeroaleatorio = 2
        self.enemigo_rebelde = False
        self.enemigo_enfadado = False
        self.contador = 0
        self.contador_volteo = 0
        self.empezar_contador_volteo = False
        self.numero_golpes_plataforma = 0

    @property
    def x(self) -> int:
        return self.__x
        # Este método se utiliza para dar valor a los atributos

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

#Aquí creamos el atributo sprite como propiedad
    @property
    def sprite(self):
        if self.enemigo_enfadado == False:
            if self.enemigo_rebelde == False:
                if self.direccion == 1:
                    return constantes.CANGREJO_DCH
                else:
                    return constantes.CANGREJO_IZQ
            else:
                return constantes.CANGREJO_REBELDE
        else:
            if self.direccion == 1:
                return constantes.CANGREJO_ENFADAD0_DCH
            else:
                return constantes.CANGREJO_ENFADADO_IZQ

    def mover(self):
        """Con este método hacemos que el cangrejo se mueva.
        Cuando llegan a los extremos aparecen por el otro lado"""
        if self.direccion == 1:

            if self.x < 200:
                self.x += self.velocidadx
            else:
                self.x = -10
                if self.y == 141:
                    self.x = 30
                    self.y = 20

        if self.direccion == 2:

            if self.x > -10:
                self.x -= self.velocidadx
            else:
                self.x = 200
                if self.y == 141:
                    self.x = 160
                    self.y = 20

# Este método hace que se caiga si no está sobre el suelo
    def caer(self):
        self.y = self.y + self.velocidady
        if self.y < constantes.SUELO:
                self.velocidady += constantes.GRAVEDAD
        else:
            self.velocidady=0
            self.y= constantes.SUELO

# Creamos este método para llevar la cuenta del tiempo que
# deben estar volteados los enemigos
    def cuenta_tiempo_volteo(self):

        if self.contador == 0:
            self.contador_volteo = self.contador_volteo + 1
            self.contador += 1
        else:
            self.contador += 1
            if self.contador > 20:
                self.contador = 0
