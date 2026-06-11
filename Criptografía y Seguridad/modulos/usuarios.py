"""usuarios.py"""
import bcrypt
import getpass
from .helpers import JsonStore
from .crypto_log import CryptoLogger

class GestorUsuarios:
    """Gestiona el registro y autenticación de usuarios"""

    def __init__(self, archivo_usuarios, archivo_contactos):
        self.archivo_usuarios = archivo_usuarios
        self.archivo_contactos = archivo_contactos
        self.usuarios = JsonStore.cargar_json(archivo_usuarios)
        self.contactos = JsonStore.cargar_json(archivo_contactos)

    def buscar_usuario(self, correo):
        """busca un usuario por su correo electrónico"""
        for u in self.usuarios:
            if u["correo"] == correo:
                return u
        return None


    def autentificar_usuario(self, correo, contrasena):
        """verifica la contraseña de un usuario usando bcrypt"""
        # Buscamos la contraseña correcta (hasheado) del usuario para compararlo
        usuario_obj = self.buscar_usuario(correo)
        contrasena_correcta = usuario_obj['password'].encode('utf-8')

        # Convertimos la contraseña introducida a bytes para poder compararlas
        contrasena_bytes = contrasena.encode('utf-8')

        # Verificamos la contraseña y la devolvemos true o false
        resultado = bcrypt.checkpw(contrasena_bytes, contrasena_correcta)

        # LOG: Registrar operación de verificación
        CryptoLogger.log_crypto("AUTENTICACIÓN USUARIO", {
            "Algoritmo": "bcrypt",
            "Operación": "Verificación de contraseña",
            "Usuario": correo,
            "Resultado": "✓ Correcta" if resultado else "✗ Incorrecta",
            "Método": "Comparación en tiempo constante (previene timing attacks)"
        })

        return resultado


    def registrar_usuario(self):
        """Registra un nuevo usuario en el sistema"""
        print("\n--- REGISTRO ---")
        correo = input("Correo electrónico: ").strip().lower()

        # Comprobamos que el correo no se haya sido previamente usado
        if self.buscar_usuario(correo):
            print("Ese correo ya está registrado. Debes iniciar sesión con tu correo y contraseña.")
            return None

        nombre = input("Nombre: ").strip()
        contrasena = getpass.getpass("Contraseña: ")
        # convertimos la contraseña a un array de bytes
        bytes = contrasena.encode('utf-8')
        # generamos el salt
        salt = bcrypt.gensalt()
        # hasheamos la contraseña
        hashed = bcrypt.hashpw(bytes, salt).decode('utf-8')

        # LOG: Registrar operación de hash
        CryptoLogger.log_crypto("REGISTRO USUARIO", {
            "Algoritmo": "bcrypt",
            "Operación": "Hash de contraseña",
            "Función": "Key Derivation Function (KDF) adaptativa",
            "Salt": "Generado automáticamente (16 bytes aleatorios)",
            "Work factor": "12 (2^12 = 4096 iteraciones)",
            "Tiempo aprox": "~100-300ms (intencionalmente lento)",
            "Resistencia": "Fuerza bruta, rainbow tables, timing attacks, GPU/ASIC"
        })

        # Guardar usuario
        self.usuarios.append({
            "correo": correo,
            "nombre": nombre,
            "password": hashed
        })

        # Inicializar lista de contactos vacía
        self.contactos.append({"usuario": correo, "contactos": []})

        JsonStore.guardar_json(self.archivo_usuarios, self.usuarios)
        JsonStore.guardar_json(self.archivo_contactos, self.contactos)
        print(f"Usuario '{nombre}' registrado correctamente.")
        return correo