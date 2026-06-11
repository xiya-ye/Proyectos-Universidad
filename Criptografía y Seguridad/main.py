"""main.py"""
import getpass
from modulos.usuarios import GestorUsuarios
from modulos.contactos import GestorContactos
from modulos.mensajes import GestorMensajes
from modulos.gestion_claves import GestorClaves
from modulos.pki import GestorPKI
from modulos.firma_digital import GestorFirmas

# -------------------- CONFIGURACIÓN DE ARCHIVOS --------------------
ARCHIVO_USUARIOS = "json/usuarios.json"
ARCHIVO_CONTACTOS = "json/contactos.json"
ARCHIVO_MENSAJES = "json/mensajes.json"
ARCHIVO_REGISTRO_CLAVES = "claves_usuarios/registro_claves.json"


# -------------------- INICIO DE SESIÓN --------------------
def iniciar_sesion(usuarios, contactos, mensajes, claves, pki):
    """Permite a un usuario iniciar sesión verificando sus credenciales"""
    print("\n--- INICIO DE SESIÓN ---")
    correo = input("Correo electrónico: ").strip().lower()
    contrasena = getpass.getpass("Contraseña: ")
    
    # Verifica contraseña con bcrypt
    contrasena_correcta = usuarios.autentificar_usuario(correo, contrasena)
    
    if contrasena_correcta:
        usuario_actual = usuarios.buscar_usuario(correo)
        print(f"\n¡Bienvenido/a, {usuario_actual['nombre']}!")
        
        # Configurar claves si no las tiene
        if not claves.usuario_tiene_claves(correo):
            print("\n  Aún no has configurado tus credenciales de seguridad.")
            # Pedir contraseña para proteger clave privada
            print("La clave privada debe protegerse con una contraseña")
            while True:
                contrasena_clave = getpass.getpass("Ingrese contraseña ("
                                                   "min 8 caracteres): ")
                if len(contrasena_clave) < 8:
                    print("\n[CLAVES] ERROR: La contraseña debe tener "
                          "al menos 8 caracteres.")
                    print("Inténtalo de nuevo.\n")
                    continue

                confirmacion = getpass.getpass("Confirme la contraseña: ")
                if contrasena_clave != confirmacion:
                    print("\n[CLAVES] ERROR: Las contraseñas no coinciden.")
                    print("Inténtalo de nuevo.\n")
                    continue
                break

            rutas = claves.guardar_claves_usuario(correo, usuario_actual[
                'nombre'], contrasena_clave)
            if rutas[2]:
                pki.emitir_certificado_usuario(correo, usuario_actual[
                    "nombre"], rutas[2])
        
        menu_usuario(usuario_actual, contactos, mensajes, claves, pki)
    else:
        print("\nCorreo o contraseña incorrectos.")

# -------------------- MENÚ DE USUARIO --------------------
def menu_usuario(usuario, contactos, mensajes, claves, pki):
    """Muestra el menú principal del usuario autenticado"""
    while True:
        print("\n--- MENÚ DE USUARIO ---")
        print("1. Ver contactos")
        print("2. Añadir contacto (con validación PKI)")
        print("3. Enviar mensaje seguro")
        print("4. Ver mensajes recibidos")
        print("5. Cerrar sesión")
        
        opcion = input("Elige una opción escribiendo solo el número (ej: 1): ")
        
        if opcion == "1":
            contactos.ver_contactos(usuario)
        
        elif opcion == "2":
            contactos.agregar_contacto(usuario)
        
        elif opcion == "3":
            mensajes.enviar_mensaje(usuario)
        
        elif opcion == "4":
            mensajes.ver_mensajes_recibidos(usuario)

        elif opcion == "5":
            print("Cerrando sesión...")
            break
        
        else:
            print("Opción no válida.")

# -------------------- MENÚ PRINCIPAL --------------------
def main():
    """
    Función principal de la aplicación.
    Carga datos y muestra el menú de inicio.
    """
    # ----- Inicializamos los gestores (configuración inicial del sistema)
    # PKI
    pki = GestorPKI()
    pki.generar_ac_raiz()
    pki.generar_ac_subordinada()

    # Claves y firmas
    claves = GestorClaves()
    firmas = GestorFirmas()

    # Usuarios
    usuarios = GestorUsuarios(ARCHIVO_USUARIOS, ARCHIVO_CONTACTOS)

    # Contactos
    contactos = GestorContactos(ARCHIVO_CONTACTOS, pki, claves, usuarios)

    # Mensajes
    mensajes = GestorMensajes(ARCHIVO_MENSAJES, contactos, claves, firmas)

    # Bucle principal
    while True:
        print("\n=== APP DE MENSAJERÍA SEGURA ===")
        print("1. Registrarse")
        print("2. Iniciar sesión")
        print("3. Salir")
        
        opcion = input("Elige una opción escribiendo solo el número (ej: 1): ")
        
        if opcion == "1":
            # --- REGISTRO---
            correo_nuevo = usuarios.registrar_usuario()
            if correo_nuevo:
                usuario = usuarios.buscar_usuario(correo_nuevo)
                nombre = usuario["nombre"]

                # Pedir contraseña para proteger clave privada
                print("La clave privada debe protegerse con una contraseña")
                while True:
                    contrasena_clave = getpass.getpass("Ingrese contraseña ("
                                                       "min 8 caracteres): ")
                    if len(contrasena_clave) < 8:
                        print( "\n[CLAVES] ERROR: La contraseña debe tener "
                               "al menos 8 caracteres.")
                        print("Inténtalo de nuevo.\n")
                        continue

                    confirmacion = getpass.getpass("Confirme la contraseña: ")
                    if contrasena_clave != confirmacion:
                        print("\n[CLAVES] ERROR: Las contraseñas no coinciden.")
                        print("Inténtalo de nuevo.\n")
                        continue
                    break

                # Generar claves RSA
                rutas = claves.guardar_claves_usuario(correo_nuevo, nombre, contrasena_clave)
                clave_publica_pem = rutas[2]

                # Emitir certificado X.509
                if clave_publica_pem:
                    pki.emitir_certificado_usuario(correo_nuevo, nombre, clave_publica_pem)

                menu_usuario(usuario, contactos, mensajes, claves, pki)
        
        elif opcion == "2":
            # --- Iniciar sesión ---
            iniciar_sesion(usuarios, contactos, mensajes, claves, pki)
        
        elif opcion == "3":
            print("¡Hasta luego!")
            break
        
        else:
            print("Opción no válida.")

if __name__ == "__main__":
    main()
