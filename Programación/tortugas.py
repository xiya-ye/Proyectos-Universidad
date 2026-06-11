# Importamos las clases que queremos usar dentro de esta
from mario import Mario
from bloque import Bloque
from monedas import Monedas
from tortugas import Tortuga
from cangrejos import Cangrejo
from moscas import Mosca
import constantes
import pyxel
import random

class Tablero:
    """Esta clase contiene la información neceraria para
    representar el tablero"""

    def __init__(self, ancho: int, alto: int):
        """ Estos parámetros son el ancho y el alto del tablero"""
        # Inicializamos el objeto
        self.ancho = ancho
        self.alto = alto
        self.contador = 0
        self.listaenemigos = []
        self.listamonedas = []
        self.direccionaleatoria = random.randint(1, 2)
        self.tiempoinicial_enemigo = 0
        self.tiempoinicial_moneda = 0
        self.tiempo = 0
        self.contador_tiempo_derribar_enemigos = 0
        self.contador_POW = 0
        self.suma = 0



        # Este bloque inicializa pyxel
        # Lo primero que tenemos que hacer es crear la pantalla.
        pyxel.init(self.ancho, self.alto, title="Mario Bros")

        # Cargamos el fichero pyxres que vamos a usar
        pyxel.load("assets/assets.pyxres")

        # Creamos a Mario en la mitad de la pantalla en x e y = 141
        self.mario = Mario(self.ancho / 2 - 8, 141)

# Creamos las plataformas utilizando bucles y listas
        self.__plataforma = []
        #Plataformas arriba
        for x in range(-10,80,16):
            self.__plataforma.append([x, 33, (constantes.SPRITE_PLATAFORMA)])
        for x in range(120,200,16):
            self.__plataforma.append([x, 33, (constantes.SPRITE_PLATAFORMA)])
        #Plataformas abajo
        for x in range(-10, 50, 16):
            self.__plataforma.append([x, 105, (constantes.SPRITE_PLATAFORMA)])
        for x in range(135, 200, 16):
            self.__plataforma.append([x, 105, (constantes.SPRITE_PLATAFORMA)])
        #Plataformas medio
        for x in range(-10, 32, 16):
            self.__plataforma.append([x, 69, (constantes.SPRITE_PLATAFORMA)])
        for x in range(165, 200, 16):
            self.__plataforma.append([x, 69, (constantes.SPRITE_PLATAFORMA)])
        for x in range(60, 140, 8):
            self.__plataforma.append([x, 65, (constantes.SPRITE_PLATAFORMA_2)])

#Creamos las vidas

        self.__aparicionvidas = []
        for x in range(53, 74, 10):
            self.__aparicionvidas.append([x, 15, (constantes.SPRITE_VIDAS)])

#Creamos los bloques y las tuberías superiores
        self.__bloques = []
        for elemento in constantes.BLOQUE_INICIAL:
            self.__bloques.append(Bloque(*elemento))

        self.__tuberias = []
        for elemento in constantes.BLOQUE_TUBERIAS:
            self.__tuberias.append(Bloque(*elemento))

#Ejecutamos el juego
        pyxel.run(self.update, self.draw)

#Definimos los atributos como propiedad y setter:
    #Definen como queremos que se modifiquen nuestras variables
    @property
    def ancho(self)->int:
        return self.__ancho

    @ancho.setter
    def ancho (self, ancho:int):
        self.__ancho = ancho

    @property
    def alto(self) -> int:
        return self.__alto

    @alto.setter
    def alto(self, alto: int):
        self.__alto = alto

    def update(self):
        """Este código se ejecuta cada frame, aquí invocamos
        los métodos que actualizan los diferentes objetos"""
        # Esto permite salir del juego
        if pyxel.btnp(pyxel.KEY_Q):
            pyxel.quit()

        # Esta parte del código se ejecutará siempre que nos queden vidas
        if self.mario.vidas >0:
            self.movimiento_Mario()
            self.saltoplataformas_arriba()
            self.bloqueo_salto()
            # Les damos movimiento a los enemigos que hemos creado con otro método
            # y sus funcionamientos relacionados con las colisiones
            self.crearEnemigo()
            self.derribar_enemigos()
            self.golpe_plataforma()
            for enemigo in self.listaenemigos:
                if enemigo.empezar_contador_volteo == True:
                    enemigo.cuenta_tiempo_volteo()
                enemigo.mover()
                enemigo.caer()
            self.matar_enemigos()
            self.cuenta_tiempo()
            self.mario.update()
            self.saltoplataformas_enemigos()
            self.colision_movedores()
            if self.contador_POW <= 18:
                self.colision_Pow()
            self.choque()
            self.__pintarvidasMario()
            # Update de las monedas
            self.crearMonedas()
            for moneda in self.listamonedas:
                moneda.mover()
                moneda.caer()
            self.saltoplataformas_monedas()
            self.colision_moneda()
            self.desaparicion_moneda()
        self.eliminar_vidas()

    # En esta parte se actualiza el movimiento de mario atendiendo a las
    # teclas pulsadas
    def movimiento_Mario(self):
        if pyxel.btn(pyxel.KEY_RIGHT):
            self.mario.sentido_dibujo_mario = 1
            self.mario.mover('derecha', self.ancho)
        elif pyxel.btn(pyxel.KEY_LEFT):
            self.mario.sentido_dibujo_mario = 2
            self.mario.mover('izquierda', self.ancho)

    # Con esto, bloqueamos momentáneamente el salto de Mario, para
    # que este no esté saltando continuamente
    def bloqueo_salto(self):
        if self.contador == 0:
            if pyxel.btn(pyxel.KEY_UP):
                self.mario.saltar('arriba', self.alto)
                self.contador += 1
        else:
            self.contador += 1
            if self.contador > 20:
                self.contador = 0

# Cuenta el tiempo para crear los enemigos o las monedas
    def cuenta_tiempo(self):
        if self.contador == 0:
            self.tiempo = self.tiempo + 1
            self.contador += 1
        else:
            self.contador += 1
            if self.contador > 20:
                self.contador = 0

# Este método crea los enemigos y los mete en una lista
    def crearEnemigo(self):
        """Este método sirve para crear enemigos cada 20 segundos.
        Saldrá el enemigo correspondiente al valor aleatorio obtenido."""
        if self.tiempo == (self.tiempoinicial_enemigo + 20) or self.tiempo == 0:
            self.tiempoinicial_enemigo = self.tiempo
            self.numeroaleatorio = random.randint(1, 3)
            if self.numeroaleatorio == 1:
                self.listaenemigos.append(Tortuga())
            if self.numeroaleatorio == 2:
                self.listaenemigos.append(Cangrejo())
            if self.numeroaleatorio == 3:
                self.listaenemigos.append(Mosca())

# Este método crea las monedas y los mete en una lista
    def crearMonedas(self):
        """Este método sirve para crear monedas cada 45 segundos."""
        if self.tiempo == (self.tiempoinicial_moneda + 45):
            self.tiempoinicial_moneda = self.tiempo
            self.listamonedas.append(Monedas())

# Creamos un método que es el que rige el salto y el movimiento
# de Mario sobre las plataformas.
    def saltoplataformas_arriba(self):
        for x in range(34):
            if (self.__plataforma[x][0] - 10) < self.mario.x < (
                    self.__plataforma[x][0] + 15):
                if (self.__plataforma[x][1] - 9) < self.mario.y < (
                        self.__plataforma[x][1] + 9):

                    if self.mario.y < self.__plataforma[x][1]:
                        self.mario.velocidady += constantes.GRAVEDAD

                    else:
                        self.mario.velocidady = 0
                        self.mario.y = self.__plataforma[x][1]

# Este método impide a Mario subir a las plataformas por abajo,
# haciendo que se choque con ellas
    def choque(self):
        if (-20 < self.mario.x < 53) or (
                123 < self.mario.x < 200):  # Plataforma abajo
            if 114 < self.mario.y < 127:
                self.mario.y = 127
                self.mario.velocidady += constantes.GRAVEDAD
        if (-20 < self.mario.x < 36.5) or (
                153 < self.mario.x < 200):  # Plataforma medios
            if 78 < self.mario.y < 91:
                self.mario.y = 91
                self.mario.velocidady += constantes.GRAVEDAD
        if (-20 < self.mario.x < 85) or (
                110 < self.mario.x < 200):  # plataforma arriba
            if 38 < self.mario.y < 55:
                self.mario.y = 55
                self.mario.velocidady += constantes.GRAVEDAD
        if (44 < self.mario.x < 140):  # plataforma del medio
            if 76 < self.mario.y < 87:
                self.mario.y = 87
                self.mario.velocidady += constantes.GRAVEDAD

# Este método es el que rige el movimiento de los enemigos por las plataformas
    def saltoplataformas_enemigos(self):
        for x in range(34):
            for enemigo in self.listaenemigos:

                if (self.__plataforma[x][0] - 10) < enemigo.x < (
                        self.__plataforma[x][0] + 15):
                    if (self.__plataforma[x][1] - 9) < enemigo.y < (
                            self.__plataforma[x][1] + 9):

                        if enemigo.y < self.__plataforma[x][1]:
                            enemigo.velocidady += constantes.GRAVEDAD

                        else:
                            enemigo.velocidady = 0
                            enemigo.y = self.__plataforma[x][1]

# Este método es el que rige el movimiento de los monedas por las plataformas,
# igual que los enemigos pero esta vez recorriendo la lista monedas
    def saltoplataformas_monedas(self):
        for x in range(34):
            for moneda in self.listamonedas:

                if (self.__plataforma[x][0] - 10) < moneda.x < (
                        self.__plataforma[x][0] + 15):
                    if (self.__plataforma[x][1] - 9) < moneda.y < (
                            self.__plataforma[x][1] + 9):

                        if moneda.y < self.__plataforma[x][1]:
                            moneda.velocidady += constantes.GRAVEDAD

                        else:
                            moneda.velocidady = 0
                            moneda.y = self.__plataforma[x][1]

# Este método es el que impide a Mario atraversar el bloque POW, haciendo que
# choque con él. Además, cuando choca voltea a los enemigos que estén
# encima de las plataformas
    def colision_Pow(self):
        if 78 < self.mario.x < 108:
            if 114 < self.mario.y < 131:
                self.mario.y = 131
                self.mario.velocidady += constantes.GRAVEDAD
                self.contador_POW += 1

                for x in range(34):
                    for enemigo in self.listaenemigos:
                        if (self.__plataforma[x][0] - 10) < enemigo.x < (
                                self.__plataforma[x][0] + 15):
                            if (self.__plataforma[x][1] - 9) < enemigo.y < (
                                    self.__plataforma[x][1] + 9):
                                enemigo.enemigo_enfadado = False
                                enemigo.enemigo_rebelde = True
                                enemigo.velocidadx = 0
                                enemigo.empezar_contador_volteo = True

# Este método elimina una vida a mario cada vez que se produce una colisión con
# un enemigo
    def colision_movedores(self):
        for enemigo in self.listaenemigos:
            if enemigo.enemigo_rebelde == False:
                if self.mario.y <= enemigo.y <= (self.mario.y + 16) or (
                        enemigo.y <= self.mario.y <= (enemigo.y + 16)):
                    if self.mario.x <= enemigo.x <= (self.mario.x + 16) or (
                            enemigo.x <= self.mario.x <= (enemigo.x + 16)):
                        self.mario.vidas -= 1

# Este método cuenta los golpes que da Mario por debajo de la plataforma
    def golpe_plataforma(self):
        for x in range(34):
            if self.__plataforma[x][0] - 8 <= self.mario.x <= self.__plataforma[x][0] + 8:
                if self.__plataforma[x][1] + 20 <= self.mario.y <= self.__plataforma[x][1] + 30:
                    for enemigo in self.listaenemigos:
                        if (self.__plataforma[x][0] - 10) < enemigo.x < (
                                self.__plataforma[x][0] + 15):
                            if (self.__plataforma[x][1] - 9) < enemigo.y < (
                                    self.__plataforma[x][1] + 9):
                                enemigo.numero_golpes_plataforma += 1

# Este método derriba a los enemigos cuando Mario golpea la plataforma por debajo
    def derribar_enemigos(self):
        for enemigo in self.listaenemigos:
            if enemigo.numeroaleatorio == 2:
                if enemigo.numero_golpes_plataforma >= 24:
                    enemigo.enemigo_rebelde = True
                    enemigo.velocidadx = 0
                    enemigo.empezar_contador_volteo = True
            else:
                if enemigo.numero_golpes_plataforma == 1:
                    enemigo.enemigo_rebelde = True
                    enemigo.velocidadx = 0
                    enemigo.empezar_contador_volteo = True

# Este método elimina los enemigos que Mario patea mientras están dados la vuelta
    def matar_enemigos(self):
        for enemigo in self.listaenemigos:
            if enemigo.contador_volteo > 8:
                enemigo.enemigo_rebelde = False
                enemigo.enemigo_enfadado = True
                enemigo.velocidadx = 2
            elif enemigo.contador_volteo <= 8:
                if enemigo.enemigo_rebelde == True:
                    if self.mario.y <= enemigo.y <= (self.mario.y + 16) or (
                            enemigo.y <= self.mario.y <= (enemigo.y + 16)):
                        if self.mario.x <= enemigo.x <= (self.mario.x + 16) or (
                                enemigo.x <= self.mario.x <= (enemigo.x + 16)):
                            self.listaenemigos.remove(enemigo)
                            self.suma += 100

# Este método elimina los sprites de las vidas de Mario y
# frena a los enemigos en movimiento si las vidas son 0
    def eliminar_vidas(self):
        if self.mario.vidas == 2 and len(self.__aparicionvidas) > 2:
            del (self.__aparicionvidas[2])
            self.mario.x = self.ancho / 2 - 8
            self.mario.y = 10
        elif self.mario.vidas == 1 and len(self.__aparicionvidas) > 1:
            del (self.__aparicionvidas[1])
            self.mario.x = self.ancho / 2 - 8
            self.mario.y = 10
        elif self.mario.vidas == 0 and len(self.__aparicionvidas) > 0:
            del (self.__aparicionvidas[0])
            for enemigo in self.listaenemigos:
                enemigo.velocidadx = 0
            for monedas in self.listamonedas:
                monedas.velocidadx = 0

# Este método suma 80 a la puntuación a mario cada vez que se produce una colisión con
# una moneda
    def colision_moneda(self):
        for moneda in self.listamonedas:
            if self.mario.y <= moneda.y <= (self.mario.y + 16) or (
                    moneda.y <= self.mario.y <= (moneda.y + 16)):
                if self.mario.x <= moneda.x <= (self.mario.x + 16) or (
                        moneda.x <= self.mario.x <= (moneda.x + 16)):
                    self.listamonedas.remove(moneda)
                    self.suma += 80

# Este método hace que las monedas desaparezcan cuando llegan al final de la
# pantalla si Mario no las ha recogido
    def desaparicion_moneda(self):
        for moneda in self.listamonedas:
            if moneda.y == 141:
                if moneda.x == 200 or moneda.x == -16:
                    self.listamonedas.remove(moneda)


# Estos métodos sirven para dibujar todos los objetos que aparecen en el tablero
    def __pintarBloque(self):
        for elemento in self.__bloques:
            pyxel.blt(elemento.x,elemento.y, *elemento.sprite)

    def __pintartuberías(self):
        for element in self.__tuberias:
            pyxel.blt(element.x,element.y, *element.sprite, colkey=0)

    def __pintarMario(self):
        pyxel.blt(self.mario.x, self.mario.y, *self.mario.sprite, colkey=14)

    def __pintarEnemigo(self):
        for elemento in self.listaenemigos:
            pyxel.blt(elemento.x,elemento.y,*elemento.sprite,colkey=0)

    def __pintarMoneda(self):
        for elemento in self.listamonedas:
            pyxel.blt(elemento.x,elemento.y,*elemento.sprite,colkey=3)

    def __pintarplataforma(self):
        for elemento in self.__plataforma:
            pyxel.blt(elemento[0],elemento[1],*elemento[2],colkey=0)

    def __pintarvidasMario(self):
        for vida in self.__aparicionvidas:
            pyxel.blt(vida[0],vida[1],*vida[2],colkey=0)

    def draw(self):
        """Este código se ejecuta también cada frame, aquí se ejecutan las
        funciones de dibujo de arriba
        """
        pyxel.cls(0)

        self.__pintartuberías()
        if self.contador_POW > 18:
            pyxel.rect(92,115,16,16,col=0)
        self.__pintarMario()
        self.__pintarplataforma()
        self.__pintarBloque()
        self.__pintarMoneda()
        self.__pintarEnemigo()
        self.__pintarvidasMario()
        pyxel.text(85, 5, "TOP:", 8)
        pyxel.text(25, 5, "I:", 5)
        pyxel.text(34, 5, str(self.suma).zfill(6), 7)
        pyxel.text(102, 5, str(self.suma).zfill(6), 7)
        self.__pintarBloque()
        if self.mario.vidas <= 0:
            pyxel.text(75, 35, "Has perdido", 7)
            pyxel.text(75, 35, "Has perdido", 7)