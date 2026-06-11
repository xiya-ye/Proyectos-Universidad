import random
import constantes
class Monedas:
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


#Aquí creamos el atributo sprite como propiedad
    @property
    def sprite(self):
        return constantes.MONEDAS

#Creamos el método para el movimiento
    def mover(self):
        if self.direccion == 1:

            if self.x < 200:
                self.x += self.velocidadx
            else:
                self.x = -10

        if self.direccion == 2:

            if self.x > -10:
                self.x -= self.velocidadx
            else:
                self.x = 200

# Creamos el método para la caída
    def caer(self):
        self.y = self.y + self.velocidady
        if self.y < constantes.SUELO:
                self.velocidady+=constantes.GRAVEDAD
        else:
            self.velocidady=0
            self.y=constantes.SUELO