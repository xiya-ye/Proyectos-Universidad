"""contactos.py"""
import os.path
from .helpers import JsonStore


class GestorContactos:
    """Gestiona la lista de contactos y su validación"""
    ARCHIVO_USUARIOS = "json/usuarios.json"
    ARCHIVO_CONTACTOS = "json/contactos.json"
    ARCHIVO_MENSAJES = "json/mensajes.json"
    ARCHIVO_REGISTRO_CLAVES = "claves_usuarios/registro_claves.json"

    def __init__(self, archivo_contactos, gestor_pki, gestor_claves, gestor_usuarios):
        self.archivo_contactos = archivo_contactos
        self.contactos_db = JsonStore.cargar_json(archivo_contactos)
        self.pki = gestor_pki
        self.claves = gestor_claves
        self.usuarios = gestor_usuarios

    def obtener_contactos(self, correo):
        """obtiene la lista de contactos de un usuario"""
        for c in self.contactos_db:
            if c["usuario"] == correo:
                return c["contactos"]
        # Crear si no existe
        nueva_entrada = {"usuario": correo, "contactos": []}
        self.contactos_db.append(nueva_entrada)
        return nueva_entrada["contactos"]

    def ver_contactos(self, usuario):
        """Muestra la lista de contactos del usuario"""
        lista = self.obtener_contactos(usuario["correo"])
        if not lista:
            print("\nNo tienes contactos agregados.")
        else:
            print("\nTus contactos:")
            for item_contacto in lista:
                correo = item_contacto["correo"]
                contacto = self.usuarios.buscar_usuario(correo)
                if contacto:
                    print("-", contacto["nombre"], f"({correo})")

    def agregar_contacto(self, usuario):
        """
        Añade un nuevo contacto a la lista del usuario e intercambian sus claves públicas.
        La relación es bidireccional (se añaden mutuamente).
        """
        nuevo = input("\nCorreo del contacto a agregar: ").strip().lower()
        actual = usuario["correo"]
        if nuevo == actual:
            print("\nNo puedes agregarte a ti mismo.")
            return
        if not self.usuarios.buscar_usuario(nuevo):
            print("\nEse usuario no existe.")
            return

        # Comprobamos si ya tiene el contacto agregado
        actual_contactos = self.obtener_contactos(actual)
        encontrado = False
        for contacto in actual_contactos:
            if contacto["correo"] == nuevo:
                encontrado = True
                break
        if encontrado:
            print("\nYa tienes agregado a ese contacto.")
            return

        # ------ Verificamos la cadena de certificación del contacto a añadir
        print(f"Iniciando protocolo de seguridad para validar a {nuevo}.")

        # Buscamos su certificado
        ruta_cert = self.pki.buscar_certificado(nuevo)
        if not ruta_cert:
            print(f"El usuario {nuevo} no tiene un Certificado Digital emitido")
            print("No puedes añadirlo hasta que inicie sesión y genere sus "
                  "credenciales de seguridad")
            return

        # Leemos el certificado
        try:
            with open(ruta_cert, 'r', encoding='utf-8') as f:
                cert_pem = f.read()
        except Exception as e:
            print(f"Error al leer el archivo de certificado: {e}")
            return

        # Verficamos la cadena
        es_valido = self.pki.verificar_cadena_certificación(cert_pem)
        if not es_valido:
            print(f"¡Cuidado! El certificado de {nuevo} no ha pasado la "
                  f"validación de confianza")
            print("El contacto no se añadirá por tu seguridad")

        # Buscamos la ruta de la clave publica
        ruta_publica_nuevo = self.pki.buscar_certificado(nuevo)
        if not ruta_publica_nuevo:
            print(f"\nEl usuario {nuevo} aún no ha generado sus claves de seguridad.")
            print("   No puedes agregarlo hasta que configure su seguridad.")
            return
        ruta_publica_actual = self.claves.buscar_ruta_publica_en_registro(
            actual)

        # Agregar contacto
        nuevo_obj = {
            "correo": nuevo,
            "ruta_publica": ruta_publica_nuevo,
            "nombre_archivo": os.path.basename(ruta_publica_nuevo)
        }
        actual_contactos.append(nuevo_obj)

        # Añadimos a la vez el contacto al otro usuario
        nuevo_contactos = self.obtener_contactos(nuevo)
        actual_obj = {
            "correo": actual,
            "ruta_publica": ruta_publica_actual,
            "nombre_archivo": os.path.basename(ruta_publica_actual)
        }
        nuevo_contactos.append(actual_obj)

        JsonStore.guardar_json(self.archivo_contactos, self.contactos_db)
        print("\nContacto agregado correctamente.")
