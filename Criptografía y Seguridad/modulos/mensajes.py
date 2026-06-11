"""mensajes.py"""
import getpass
import base64
import os
from Crypto.Cipher import AES, PKCS1_OAEP
from Crypto.PublicKey import RSA
from Crypto.Random import get_random_bytes
from .crypto_log import CryptoLogger
from .helpers import JsonStore


class GestorMensajes:
    """Gestiona el envío, recepción y cifrado de mensajes"""
    def __init__(self, archivo_mensajes, gestor_contactos, gestor_claves, gestor_firmas):
        self.archivo_mensajes = archivo_mensajes
        self.mensajes_db = JsonStore.cargar_json(archivo_mensajes)
        self.contactos = gestor_contactos
        self.claves = gestor_claves
        self.firmas = gestor_firmas


    def _cifrar_aes(self, texto):
        """Este metodo cifra el texto con una clave AES-256 aleatoria de 32
        bytes"""
        clave = get_random_bytes(32)
        # LOG: Registrar generación de clave
        CryptoLogger.log_crypto("GENERACIÓN CLAVE AES", {
            "Algoritmo": "AES-256",
            "Tamaño clave": "256 bits (32 bytes)",
            "Generador": "Crypto.Random.get_random_bytes (CSPRNG)",
            "Fuente entropía": "Sistema operativo (/dev/urandom o CryptGenRandom)",
            "Seguridad": "Criptográficamente seguro e impredecible"
        })

        cipher = AES.new(clave, AES.MODE_EAX)
        mensaje_bytes = texto.encode('utf-8')
        cifrado, etiqueta = cipher.encrypt_and_digest(mensaje_bytes)
        nonce = cipher.nonce    # Para garantizarnos que el cifrado sea unico

        # Los pasamos a texto legible para poder almacenarlo en JSON correctamente
        nonce_j = base64.b64encode(nonce).decode('utf-8')
        cifrado_j = base64.b64encode(cifrado).decode('utf-8')
        etiqueta_j = base64.b64encode(etiqueta).decode('utf-8')

        mensaje_cifrado_j = {
            "nonce": nonce_j,
            "cifrado": cifrado_j,
            "etiqueta": etiqueta_j
        }
        CryptoLogger.log_crypto("CIFRADO DE MENSAJE", {
            "Algoritmo": "AES-256-EAX",
            "Modo": "EAX (Cifrado Autenticado AEAD)",
            "Tamaño clave": "256 bits (32 bytes)",
            "Tamaño nonce": f"{len(nonce)} bytes",
            "Tamaño tag MAC": f"{len(etiqueta)} bytes",
            "Tamaño mensaje original": f"{len(mensaje_bytes)} bytes",
            "Tamaño mensaje cifrado": f"{len(cifrado)} bytes",
            "Propiedades": "Confidencialidad + Integridad + Autenticidad"
        })

        return clave, mensaje_cifrado_j

    def _descifrar_aes(self, datos, clave):
        """Este metodo descifra los datos con la clave"""
        try:
            # Asignamos el nonce y el mensaje cifrado y los pasamos a bytes
            nonce = base64.b64decode(datos["nonce"])
            cifrado = base64.b64decode(datos["cifrado"])
            # Obtenemos la etiqueta de autentificación del mensaje y pasamos a bytes
            etiqueta = base64.b64decode(datos["etiqueta"])
            cipher = AES.new(clave, AES.MODE_EAX, nonce=nonce)
            mensaje_descifrado_bytes = cipher.decrypt_and_verify(cifrado, etiqueta)

            CryptoLogger.log_crypto("DESCIFRADO DE MENSAJE", {
                "Algoritmo": "AES-256-EAX",
                "Operación": "Descifrado y verificación de integridad",
                "Estado verificación": "✓ CORRECTA - Mensaje íntegro y auténtico",
                "Tamaño descifrado": f"{len(mensaje_descifrado_bytes)} bytes",
                "Seguridad": "Sin modificaciones detectadas"
            })

            # Devolvemos el mensaje descifrado en string
            return mensaje_descifrado_bytes.decode('utf-8')

        except Exception as e:
            CryptoLogger.log_crypto("ERROR DESCIFRADO", {
                "Error": str(e),
                "Tipo": type(e).__name__
            })
            return None

    def enviar_mensaje(self, usuario):
        """
        Envía un mensaje cifrado a un contacto.
        Ahora permite opcionalmente firmar el mensaje con la clave privada RSA del usuario.
        """
        self.contactos.ver_contactos(usuario)
        destino = input("Correo del destinatario: ").strip().lower()

        lista = self.contactos.obtener_contactos(usuario["correo"])
        contacto_destino = next((c for c in lista if c["correo"] == destino), None)

        if not contacto_destino:
            print("Ese usuario no está en tu lista de contactos.")
            return

        # Verificamos primos si tenemos la clave pública del otro usuario
        ruta_pub_destino = contacto_destino.get("ruta_publica")
        if not ruta_pub_destino or not os.path.exists(ruta_pub_destino):
            print("Error: No se ha encontrado el archivo de clave pública de", destino)
            print("Ruta esperada:", ruta_pub_destino)
            print("Intenta borrar el contacto y agregarlo de nuevo")
            return

        mensaje = input("Escribe tu mensaje: ").strip()

        # Firma digital opcional
        firmado = False
        firma_b64 = None

        # Para la firma, usamos la clave privada del remitente
        opcion_firma = input("\n¿Desea firmar digitalmente este mensaje? (si/no): ").strip().lower()
        if opcion_firma == "si":
            contrasena_clave = getpass.getpass("Contraseña de su clave privada: ")
            try:
                # Cargamos nuestra clave privada
                clave_privada_pem = self.claves.cargar_clave_privada(usuario[
                                                                  "correo"], contrasena_clave)
                # Firmamos el texto original
                firma_b64 = self.firmas.firmar_mensaje(mensaje, clave_privada_pem,
                                                       contrasena_clave)
                # Verificamos si se ha verificado correctamente
                if firma_b64:
                    firmado = True
                    print("\nMensaje firmado digitalmente")
                else:
                    print("\n[ERROR] Fallo al generar la firma. Se enviará sin firmar")

            except Exception as e:
                print(f"\n[ERROR] No se pudo firmar el mensaje: {e}")
                print("El mensaje se enviará SIN firma.")

        # ----- Aplicamos el cifrado híbrido
        # Primero generamos la clave de sesion con AES y ciframos el mensaje con
        # esa clave
        clave_sesion, mensaje_cifrado = self._cifrar_aes(mensaje)

        # Ciframos la clave AES con la clave pública del destinatario
        try:
            # Leemos el archivo PEM de la pública
            with open(ruta_pub_destino, 'rb') as f:
                clave_pub_dat = f.read()
                clave_pub = RSA.import_key(clave_pub_dat)

            # Usamos PKCS1_OAEP para cifrar la llave AES y la codificamos a base64
            cipher_rsa = PKCS1_OAEP.new(clave_pub)
            clave_sesion_cifrada = cipher_rsa.encrypt(clave_sesion)
            clave_sesion_cifrada_b64 = base64.b64encode(clave_sesion_cifrada).decode('utf-8')

        except Exception as e:
            print("Error al cifrar la llave con RSA:", e)
            return

        # ----- Construimos paquete JSON
        datos_mensaje = {
            "de": usuario["correo"],
            "para": destino,
            "mensaje": mensaje_cifrado,
            "clave_cifrada": clave_sesion_cifrada_b64,  # clave protegida
            "firmado": firmado,
            "fecha": __import__('datetime').datetime.now().strftime(
                "%Y-%m-%d %H:%M:%S")
        }

        if firmado and firma_b64:
            datos_mensaje["firma"] = firma_b64

        self.mensajes_db.append(datos_mensaje)
        JsonStore.guardar_json(self.archivo_mensajes, self.mensajes_db)
        if firmado:
            print("\nMensaje enviado correctamente y firmado.")
        else:
            print("\nMensaje enviado correctamente.")

    def ver_mensajes_recibidos(self, usuario):
        """
        Muestra mensajes recibidos, descifrándolos.
        Si el mensaje tiene firma digital, permite verificarla.
        Requiere la contraseña de la clave privada del usuario para descifrar la clave AES.
        """
        recibidos = [m for m in self.mensajes_db if m["para"] == usuario["correo"]]
        if not recibidos:
            print("\nNo hay mensajes.")
            return

        # Pedimos la contraseña de la clave privada
        pass_priv = getpass.getpass("Para leer, introduce tu contraseña de "
                                    "clave privada: ")

        clave_priv_pem = self.claves.cargar_clave_privada(usuario["correo"],
                                                       pass_priv)
        if not clave_priv_pem:
            print("Contraseña incorrecta. Vuelva a intentarlo.")

        # Convertimos el PEM a un objeto RSA
        try:
            clave_priv_obj = RSA.import_key(clave_priv_pem)
        except Exception as e:
            print(f"Error al importar la clave privada: {e}")
            return

        print("\n--- MENSAJES RECIBIDOS ---")
        for m in recibidos:
            # Desciframos la clave AES
            try:
                cipher_rsa = PKCS1_OAEP.new(clave_priv_obj)
                clave_cifrada_b64 = m.get("clave_cifrada")

                # Decodificamos y desciframos con RSA
                clave_cifrada_bytes = base64.b64decode(clave_cifrada_b64)
                clave_aes = cipher_rsa.decrypt(clave_cifrada_bytes)

                # Desciframos el mensaje con la clave AES recuperada
                texto_plano = self._descifrar_aes(m["mensaje"], clave_aes)
                if texto_plano:
                    print(f"\nDe: {m['de']} | {m['fecha']}")
                    print(f"Mensaje: {texto_plano}")

                    # Si está firmado --> verificamos la firma
                    if m.get("firmado") and m["firma"]:
                        print("    El mensaje tiene firma digital. Verificando...")
                        try:
                            # Cargamos la pública del remitente y verificamos la firma
                            clave_pub_rem = self.claves.cargar_clave_publica(m["de"])
                            es_valida = self.firmas.verificar_firma(
                                texto_plano, m["firma"], clave_pub_rem)
                            if es_valida:
                                print("   FIRMA VÁLIDA - Mensaje auténtico e íntegro")
                            else:
                                print("   FIRMA INVÁLIDA - Mensaje comprometido "
                                      "o firma incorrecta")
                        except Exception as e:
                            print(f"        No se puedo verificar la firma: {e}")
                else:
                    print("    Error: El mensaje está corrupto o fue modificado")
            except Exception as e:
                print("    Error: No se puedo descifrar la clave AES", e)
