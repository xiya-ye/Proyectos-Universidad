# modulos/firma_digital.py
"""
Módulo para la generación y verificación de firmas digitales usando RSA
Implementa firma digital sobre mensajes para garantizar autenticidad y no repudio
"""

import base64
from Crypto.PublicKey import RSA
from Crypto.Signature import pkcs1_15
from Crypto.Hash import SHA256
from .crypto_log import CryptoLogger

class GestorFirmas:
    """Gestiona la creación y verificación de firmas digitales"""

    @staticmethod
    def firmar_mensaje(mensaje_texto, clave_privada_pem, contrasena=None):
        """
        Firma un mensaje usando RSA-PSS con SHA-256

        Args:
            mensaje_texto: Texto del mensaje a firmar
            clave_privada_pem: Clave privada en formato PEM (bytes o string)
            contrasena: Contraseña para descifrar la clave privada (si está protegida)

        Returns:
            Firma digital codificada en base64 (string)
        """
        try:
            # Convertir string a bytes si es necesario
            if isinstance(clave_privada_pem, str):
                clave_privada_pem = clave_privada_pem.encode('utf-8')

            # Importar clave privada
            if contrasena:
                if isinstance(contrasena, str):
                    contrasena = contrasena.encode('utf-8')
                clave_privada = RSA.import_key(clave_privada_pem, passphrase=contrasena)
            else:
                clave_privada = RSA.import_key(clave_privada_pem)

            # Calcular hash del mensaje
            mensaje_bytes = mensaje_texto.encode('utf-8')
            hash_obj = SHA256.new(mensaje_bytes)

            # Generar firma con PKCS#1 v1.5
            firma = pkcs1_15.new(clave_privada).sign(hash_obj)

            # Codificar firma en base64 para almacenamiento
            firma_b64 = base64.b64encode(firma).decode('utf-8')

            # LOG: Registrar operación de firma
            CryptoLogger.log_crypto("GENERACIÓN DE FIRMA DIGITAL", {
                "Algoritmo": "RSA con PKCS#1 v1.5",
                "Hash": "SHA-256",
                "Tamaño clave": f"{clave_privada.size_in_bits()} bits",
                "Tamaño firma": f"{len(firma)} bytes ({len(firma)*8} bits)",
                "Tamaño mensaje": f"{len(mensaje_bytes)} bytes",
                "Tamaño hash": "256 bits (32 bytes)",
                "Propiedades": "Autenticidad + Integridad + No repudio"
            })

            return firma_b64

        except ValueError as e:
            CryptoLogger.log_crypto("ERROR FIRMA DIGITAL", {
                "Error": "Contraseña incorrecta o clave privada inválida",
                "Detalle": str(e)
            })
            return None
        except Exception as e:
            CryptoLogger.log_crypto("ERROR FIRMA DIGITAL", {
                "Error": str(e),
                "Tipo": type(e).__name__
            })
            return None

    @staticmethod
    def verificar_firma(mensaje_texto, firma_b64, clave_publica_pem):
        """
        Verifica una firma digital RSA

        Args:
            mensaje_texto: Texto original del mensaje
            firma_b64: Firma digital en base64
            clave_publica_pem: Clave pública en formato PEM (bytes o string)

        Returns:
            True si la firma es válida, False en caso contrario
        """
        try:
            # Convertir string a bytes si es necesario
            if isinstance(clave_publica_pem, str):
                clave_publica_pem = clave_publica_pem.encode('utf-8')

            # Importar clave pública
            clave_publica = RSA.import_key(clave_publica_pem)

            # Decodificar firma
            firma = base64.b64decode(firma_b64)

            # Calcular hash del mensaje
            mensaje_bytes = mensaje_texto.encode('utf-8')
            hash_obj = SHA256.new(mensaje_bytes)

            # Verificar firma
            try:
                pkcs1_15.new(clave_publica).verify(hash_obj, firma)

                # LOG: Verificación exitosa
                CryptoLogger.log_crypto("VERIFICACIÓN DE FIRMA DIGITAL", {
                    "Algoritmo": "RSA con PKCS#1 v1.5",
                    "Hash": "SHA-256",
                    "Resultado": "FIRMA VÁLIDA",
                    "Estado": "Mensaje auténtico e íntegro",
                    "Tamaño clave": f"{clave_publica.size_in_bits()} bits",
                    "Garantías": "Autenticidad + Integridad + No repudio"
                })
                return True

            except (ValueError, TypeError):
                # LOG: Verificación fallida
                CryptoLogger.log_crypto("VERIFICACIÓN DE FIRMA DIGITAL", {
                    "Algoritmo": "RSA con PKCS#1 v1.5",
                    "Hash": "SHA-256",
                    "Resultado": "FIRMA INVÁLIDA",
                    "Estado": "Mensaje comprometido o firma incorrecta",
                    "Acción": "Rechazar mensaje por seguridad"
                })
                return False

        except Exception as e:
            CryptoLogger.log_crypto("ERROR VERIFICACIÓN FIRMA", {
                "Error": str(e),
                "Tipo": type(e).__name__
            })
            return False
