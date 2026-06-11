# modulos/gestion_claves.py
"""
Módulo para la generación y almacenamiento seguro de claves asimétricas
Gestiona claves RSA para firma digital y cifrado asimétrico con protección por contraseña
"""

import os
import getpass
from Crypto.PublicKey import RSA
from .crypto_log import CryptoLogger
from .helpers import JsonStore

class GestorClaves:
    """Gestiona las claves de los usuarios"""
    DIR_CLAVES = "claves_usuarios"
    ARCHIVO_REGISTRO_CLAVES = os.path.join(DIR_CLAVES, "registro_claves.json")

    def __init__(self):
        os.makedirs(self.DIR_CLAVES, exist_ok=True)
        if not os.path.exists(self.ARCHIVO_REGISTRO_CLAVES):
            JsonStore.guardar_json(self.ARCHIVO_REGISTRO_CLAVES,
                                   {"usuarios_con_claves": []})

    # -------------------- GENERACIÓN DE PARES DE CLAVES RSA --------------------
    def generar_par_claves_rsa(self, tamano_bits=2048):
        """
        Genera un par de claves RSA (privada y pública)

        Args:
            tamano_bits: Tamaño de la clave en bits (por defecto 2048, recomendado 2048-4096)

        Returns:
            Tupla (clave_privada_obj, clave_publica_obj) objetos RSA de PyCryptodome
        """
        # Generar par de claves
        clave_privada = RSA.generate(tamano_bits)
        clave_publica = clave_privada.publickey()

        # LOG
        CryptoLogger.log_crypto("GENERACIÓN PAR CLAVES RSA", {
            "Algoritmo": "RSA",
            "Tamaño clave": f"{tamano_bits} bits",
            "Exponente público": f"{clave_privada.e}",
            "Uso": "Firma digital + Cifrado asimétrico",
            "Seguridad": "Nivel apropiado para uso actual (NIST recomienda ≥2048 bits)",
            "Componentes": "Clave privada (d, n) + Clave pública (e, n)",
            "Generador": "Crypto.PublicKey.RSA (PyCryptodome)"
        })
        return clave_privada, clave_publica


    # -------------------- ALMACENAMIENTO SEGURO DE CLAVES --------------------
    def guardar_claves_usuario(self, correo, nombre, contrasena_proteccion=None):
        """
        Genera y guarda de forma segura el par de claves RSA de un usuario

        La clave privada se almacena cifrada con una contraseña.
        La clave pública se almacena en texto plano (formato PEM).

        Args:
            correo: Correo electrónico del usuario
            nombre: Nombre del usuario
            contrasena_proteccion: Contraseña para proteger la clave privada (se solicita si es None)

        Returns:
            Tupla (ruta_privada, ruta_publica, clave_publica_pem) o (None, None, None) si falla
        """
        print(f"\n[CLAVES] Generando par de claves RSA para {nombre}...")

        # Solicitar contraseña para proteger clave privada si no se proporciona
        if contrasena_proteccion is None:
            print("    Esta contraseña se necesitará cada vez que firme un mensaje.")
            contrasena_proteccion = getpass.getpass("Ingrese contraseña de al menos 8 caracteres para proteger su clave privada: ")
            confirmacion = getpass.getpass("Confirme la contraseña: ")

            if contrasena_proteccion != confirmacion:
                print("  [CLAVES] ERROR: Las contraseñas no coinciden.")
                return None, None, None

            if len(contrasena_proteccion) < 8:
                print("  [CLAVES] ERROR: La contraseña debe tener al menos 8 caracteres.")
                return None, None, None

        # Generar par de claves RSA-2048
        clave_privada, clave_publica = self.generar_par_claves_rsa(2048)

        # Preparar rutas de archivos
        nombre_archivo = correo.replace("@", "_at_").replace(".", "_")
        ruta_privada = os.path.join(self.DIR_CLAVES,
                                    f"{nombre_archivo}_private.pem")
        ruta_publica = os.path.join(self.DIR_CLAVES,
                                    f"{nombre_archivo}_public.pem")

        try:
            # Guardar clave privada CIFRADA con contraseña
            with open(ruta_privada, "wb") as f:
                f.write(clave_privada.export_key(
                    format='PEM',
                    passphrase=contrasena_proteccion,
                    pkcs=8,
                    protection="PBKDF2WithHMAC-SHA256AndAES256-CBC"  # Esquema seguro
                ))

            # Guardar clave pública (sin cifrar, es pública)
            clave_publica_pem = clave_publica.export_key(format='PEM').decode('utf-8')
            with open(ruta_publica, "w") as f:
                f.write(clave_publica_pem)

            # Registrar en el archivo de control
            datos_existentes = JsonStore.cargar_json(self.ARCHIVO_REGISTRO_CLAVES)
            if isinstance(datos_existentes, list):
                registro = {"datos_existentes": []}
            else:
                registro = datos_existentes

            # Verificamos que existe la clave usuarios_con_claves
            if "usuarios_con_claves" not in registro:
                registro["usuarios_con_claves"] = []

            registro["usuarios_con_claves"].append({
                "correo": correo,
                "nombre": nombre,
                "archivo_clave_privada": ruta_privada,
                "archivo_clave_publica": ruta_publica,
                "fecha_generacion": __import__('datetime').datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
                "tamano_bits": 2048
            })
            JsonStore.guardar_json(self.ARCHIVO_REGISTRO_CLAVES, registro)

            # LOG
            CryptoLogger.log_crypto("ALMACENAMIENTO SEGURO CLAVES", {
                "Usuario": f"{nombre} ({correo})",
                "Clave privada": f"{ruta_privada}",
                "Protección": "✓ Cifrada con contraseña (PBKDF2 + AES-256-CBC)",
                "Clave pública": f"{ruta_publica}",
                "Formato": "PEM (PKCS#8 para privada, X.509 SubjectPublicKeyInfo para pública)",
                "Seguridad": "Acceso a clave privada requiere contraseña del usuario"
            })

            print(f"[CLAVES] ✓ Par de claves generado y almacenado")
            print(f"         Clave privada (protegida): {ruta_privada}")
            print(f"         Clave pública: {ruta_publica}")

            return ruta_privada, ruta_publica, clave_publica_pem

        except Exception as e:
            CryptoLogger.log_crypto("ERROR ALMACENAMIENTO CLAVES", {
                "Error": str(e),
                "Tipo": type(e).__name__
            })
            print(f"[CLAVES] ERROR: {e}")
            return None, None, None


    # -------------------- CARGA DE CLAVES --------------------

    def cargar_clave_privada(self, correo, contrasena):
        """
        Carga la clave privada de un usuario desde el archivo (requiere contraseña)

        Args:
            correo: Correo electrónico del usuario
            contrasena: Contraseña para descifrar la clave privada

        Returns:
            Clave privada en formato PEM (string) o None si falla
        """
        nombre_archivo = correo.replace("@", "_at_").replace(".", "_")
        ruta_privada = os.path.join(self.DIR_CLAVES, f"{nombre_archivo}_private.pem")

        if not os.path.exists(ruta_privada):
            print(f"  [CLAVES] ERROR: No se encontró clave privada para {correo}")
            return None

        try:
            with open(ruta_privada, "rb") as f:
                clave_bytes = f.read()

            # Intentar importar con contraseña
            clave_privada = RSA.import_key(clave_bytes, passphrase=contrasena)
            clave_privada_pem = clave_privada.export_key(format='PEM').decode('utf-8')

            CryptoLogger.log_crypto("CARGA CLAVE PRIVADA", {
                "Usuario": correo,
                "Estado": "✓ Clave privada descifrada exitosamente",
                "Archivo": ruta_privada,
                "Protección": "Contraseña verificada correctamente"
            })

            return clave_privada_pem

        except ValueError:
            CryptoLogger.log_crypto("ERROR CARGA CLAVE PRIVADA", {
                "Usuario": correo,
                "Error": "✗ Contraseña incorrecta",
                "Acción": "Acceso denegado a clave privada"
            })
            print("[CLAVES] ERROR: Contraseña incorrecta.")
            return None
        except Exception as e:
            CryptoLogger.log_crypto("ERROR CARGA CLAVE PRIVADA", {
                "Usuario": correo,
                "Error": str(e),
                "Tipo": type(e).__name__
            })
            print(f"[CLAVES] ERROR: {e}")
            return None


    def cargar_clave_publica(self, correo):
        """
        Carga la clave pública de un usuario desde el archivo

        Args:
            correo: Correo electrónico del usuario

        Returns:
            Clave pública en formato PEM (string) o None si no existe
        """
        nombre_archivo = correo.replace("@", "_at_").replace(".", "_")
        ruta_publica = os.path.join(self.DIR_CLAVES,
                                    f"{nombre_archivo}_public.pem")

        if not os.path.exists(ruta_publica):
            return None

        try:
            with open(ruta_publica, "r", encoding='utf-8') as f:
                clave_publica_pem = f.read()
            return clave_publica_pem
        except Exception as e:
            print(f"[CLAVES] ERROR al cargar clave pública: {e}")
            return None

    # -------------------- UTILIDADES --------------------

    def usuario_tiene_claves(self, correo):
        """
        Verifica si un usuario ya tiene un par de claves generado

        Args:
            correo: Correo electrónico del usuario

        Returns:
            True si tiene claves, False en caso contrario
        """
        nombre_archivo = correo.replace("@", "_at_").replace(".", "_")
        ruta_privada = os.path.join(self.DIR_CLAVES, f"{nombre_archivo}_private.pem")
        ruta_publica = os.path.join(self.DIR_CLAVES, f"{nombre_archivo}_public.pem")

        return os.path.exists(ruta_privada) and os.path.exists(ruta_publica)

    def buscar_ruta_publica_en_registro(self, correo_objetivo):
        """
        Simula una consulta a un Servidor de Claves (PKI).
        Busca en el registro global si el usuario ya generó sus claves.

        Returns:
            str: Ruta al archivo .pem de la clave pública o None si no existe.
        """
        if not os.path.exists(self.ARCHIVO_REGISTRO_CLAVES):
            return None

        # Cargamos la lista de usuarios con clave y buscamos su correo
        registro = JsonStore.cargar_json(self.ARCHIVO_REGISTRO_CLAVES)
        lista_usuarios = registro.get("usuarios_con_claves")

        for usuario in lista_usuarios:
            if usuario["correo"] == correo_objetivo:
                return usuario["archivo_clave_publica"]
        return None