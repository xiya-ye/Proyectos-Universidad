"""
Created by Marta Vindel in nov 2023 in version 3.10
"""
import constantes

class Bloque:
    def __init__(self,x:int,y:int,tipo:int):
        self.x = x
        self.y = y
        self.tipo = tipo

        if self.tipo == "BLOQUE11":
            self.sprite = constantes.SPRITE_BLOQUE1_1
        elif self.tipo == "BLOQUE12":
            self.sprite = constantes.SPRITE_BLOQUE1_2
        elif self.tipo == "BLOQUE21":
            self.sprite = constantes.SPRITE_BLOQUE2_1
        elif self.tipo == "BLOQUE22":
            self.sprite = constantes.SPRITE_BLOQUE2_2
        elif self.tipo == "PLATAFORMA":
            self.sprite = constantes.SPRITE_PLATAFORMA
        elif self.tipo == "SUELO":
            self.sprite = constantes.SPRITE_SUELO
        elif self.tipo == "POW":
            self.sprite = constantes.SPRITE_POW
        elif self.tipo == "I":
            self.sprite = constantes.SPRITE_I
        elif self.tipo == "TOP":
            self.sprite = constantes.SPRITE_TOP
        else:
            self.sprite = constantes.SPRITE_BLOQUE1_1

