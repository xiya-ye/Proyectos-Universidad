# modulos/pki.py
"""
Módulo para la gestión de PKI (Public Key Infrastructure)
Implementa una jerarquía de Autoridades de Certificación (AC raíz y subordinada)
y emisión de certificados X.509 para usuarios
"""

import os
from datetime import datetime, timedelta, timezone
from cryptography import x509
from cryptography.x509.oid import NameOID
from cryptography.hazmat.primitives import hashes, serialization
from cryptography.hazmat.primitives.asymmetric import rsa, padding
from cryptography.hazmat.backends import default_backend
from .crypto_log import CryptoLogger
from .helpers import JsonStore

class GestorPKI:
    """Gestiona la Infraestructura de Clave Pública (PKI)"""

    DIR_PKI = "pki"
    DIR_CA_RAIZ = os.path.join(DIR_PKI, "ca_raiz")
    DIR_CA_SUBORDINADA = os.path.join(DIR_PKI, "ca_subordinada")
    DIR_USUARIOS = os.path.join(DIR_PKI, "usuarios")
    ARCHIVO_PKI_CONFIG = os.path.join(DIR_PKI, "pki_config.json")

    def __init__(self):
        self.inicializar_pki()

    def inicializar_pki(self):
        """Crea directorios y configuración si no existen"""
        os.makedirs(self.DIR_CA_RAIZ, exist_ok=True)
        os.makedirs(self.DIR_CA_SUBORDINADA, exist_ok=True)
        os.makedirs(self.DIR_USUARIOS, exist_ok=True)

        # Crear archivo de configuración si no existe
        if not os.path.exists(self.ARCHIVO_PKI_CONFIG):
            config = {
                "ca_raiz": {
                    "nombre": "AC Raíz - Mensajería Segura UC3M",
                    "pais": "ES",
                    "organizacion": "Universidad Carlos III Madrid",
                    "unidad": "Criptografía y Seguridad",
                    "fecha_creacion": datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
                    "validez_anos": 20
                },
                "ca_subordinada": {
                    "nombre": "AC Subordinada - Usuarios Mensajería",
                    "pais": "ES",
                    "organizacion": "Universidad Carlos III Madrid",
                    "unidad": "Servicio de Mensajería",
                    "fecha_creacion": None,
                    "validez_anos": 10
                },
                "certificados_emitidos": []
            }
            JsonStore.guardar_json(self.ARCHIVO_PKI_CONFIG, config)


    # -------------------- GENERACIÓN DE AUTORIDADES DE CERTIFICACIÓN --------------------

    def generar_ac_raiz(self):
        """
        Genera la Autoridad de Certificación Raíz con certificado autofirmado

        Returns:
            True si se generó correctamente, False si ya existe
        """
        ruta_clave = os.path.join(self.DIR_CA_RAIZ, "ca_raiz_private.pem")
        ruta_cert = os.path.join(self.DIR_CA_RAIZ, "ca_raiz_cert.pem")

        # Verificar si ya existe
        if os.path.exists(ruta_clave) and os.path.exists(ruta_cert):
            print("\n[PKI] La AC Raíz ya existe.")
            return False

        print("\n[PKI] Generando Autoridad de Certificación Raíz...")

        # Cargar configuración
        config = JsonStore.cargar_json(self.ARCHIVO_PKI_CONFIG)
        config_raiz = config["ca_raiz"]

        # Generar par de claves RSA-2048
        clave_privada = rsa.generate_private_key(
            public_exponent=65537,
            key_size=2048,
            backend=default_backend()
        )
        clave_publica = clave_privada.public_key()

        # Crear nombre distinguido (DN) del emisor y sujeto
        nombre = x509.Name([
            x509.NameAttribute(NameOID.COUNTRY_NAME, config_raiz["pais"]),
            x509.NameAttribute(NameOID.ORGANIZATION_NAME, config_raiz["organizacion"]),
            x509.NameAttribute(NameOID.ORGANIZATIONAL_UNIT_NAME, config_raiz["unidad"]),
            x509.NameAttribute(NameOID.COMMON_NAME, config_raiz["nombre"]),
        ])

        # Crear certificado autofirmado
        certificado = (
            x509.CertificateBuilder()
            .subject_name(nombre)
            .issuer_name(nombre)  # Autofirmado: emisor = sujeto
            .public_key(clave_publica)
            .serial_number(x509.random_serial_number())
            .not_valid_before(datetime.utcnow())
            .not_valid_after(datetime.utcnow() + timedelta(days=365 * config_raiz["validez_anos"]))
            .add_extension(
                x509.BasicConstraints(ca=True, path_length=1),  # Puede certificar otras ACs
                critical=True,
            )
            .add_extension(
                x509.KeyUsage(
                    digital_signature=True,
                    key_cert_sign=True,  # Puede firmar certificados
                    crl_sign=True,       # Puede firmar listas de revocación
                    key_encipherment=False,
                    content_commitment=False,
                    data_encipherment=False,
                    key_agreement=False,
                    encipher_only=False,
                    decipher_only=False,
                ),
                critical=True,
            )
            .add_extension(
                x509.SubjectKeyIdentifier.from_public_key(clave_publica),
                critical=False,
            )
            .sign(clave_privada, hashes.SHA256(), default_backend())
        )

        # Guardar clave privada (protegida con contraseña)
        contrasena_ca = b"AC_RAIZ_2025_UC3M_SEGURA"  # En producción: solicitar al usuario
        with open(ruta_clave, "wb") as f:
            f.write(clave_privada.private_bytes(
                encoding=serialization.Encoding.PEM,
                format=serialization.PrivateFormat.PKCS8,
                encryption_algorithm=serialization.BestAvailableEncryption(contrasena_ca)
            ))

        # Guardar certificado público
        with open(ruta_cert, "wb") as f:
            f.write(certificado.public_bytes(serialization.Encoding.PEM))

        # LOG
        CryptoLogger.log_crypto("CREACIÓN AC RAÍZ", {
            "Tipo": "Autoridad de Certificación Raíz (Root CA)",
            "Nombre": config_raiz["nombre"],
            "Algoritmo clave": "RSA",
            "Tamaño clave": "2048 bits",
            "Algoritmo firma": "SHA-256 con RSA",
            "Validez": f"{config_raiz['validez_anos']} años",
            "Certificado": "Autofirmado",
            "Uso": "Firma de certificados de ACs subordinadas",
            "Path length": "1 (puede certificar 1 nivel de ACs)"
        })

        print(f"[PKI] ✓ AC Raíz creada exitosamente")
        print(f"      Certificado: {ruta_cert}")
        print(f"      Clave privada: {ruta_clave} (protegida con contraseña)")

        return True


    def generar_ac_subordinada(self):
        """
        Genera una Autoridad de Certificación Subordinada certificada por la AC Raíz

        Returns:
            True si se generó correctamente, False si ya existe o falla
        """
        ruta_clave = os.path.join(self.DIR_CA_SUBORDINADA, "ca_sub_private.pem")
        ruta_cert = os.path.join(self.DIR_CA_SUBORDINADA, "ca_sub_cert.pem")
        ruta_ca_raiz_cert = os.path.join(self.DIR_CA_RAIZ, "ca_raiz_cert.pem")
        ruta_ca_raiz_key = os.path.join(self.DIR_CA_RAIZ, "ca_raiz_private.pem")

        # Verificar que existe la AC Raíz
        if not os.path.exists(ruta_ca_raiz_cert) or not os.path.exists(ruta_ca_raiz_key):
            print("\n[PKI] ERROR: Debe crear primero la AC Raíz.")
            return False

        # Verificar si ya existe
        if os.path.exists(ruta_clave) and os.path.exists(ruta_cert):
            print("\n[PKI] La AC Subordinada ya existe.")
            return False

        print("\n[PKI] Generando Autoridad de Certificación Subordinada...")

        # Cargar configuración
        config = JsonStore.cargar_json(self.ARCHIVO_PKI_CONFIG)
        config_sub = config["ca_subordinada"]

        # Generar par de claves RSA-2048 para AC subordinada
        clave_privada_sub = rsa.generate_private_key(
            public_exponent=65537,
            key_size=2048,
            backend=default_backend()
        )
        clave_publica_sub = clave_privada_sub.public_key()

        # Cargar certificado y clave de AC Raíz
        with open(ruta_ca_raiz_cert, "rb") as f:
            cert_raiz = x509.load_pem_x509_certificate(f.read(), default_backend())

        with open(ruta_ca_raiz_key, "rb") as f:
            clave_raiz = serialization.load_pem_private_key(
                f.read(),
                password=b"AC_RAIZ_2025_UC3M_SEGURA",
                backend=default_backend()
            )

        # Crear nombre distinguido de la AC subordinada
        nombre_sub = x509.Name([
            x509.NameAttribute(NameOID.COUNTRY_NAME, config_sub["pais"]),
            x509.NameAttribute(NameOID.ORGANIZATION_NAME, config_sub["organizacion"]),
            x509.NameAttribute(NameOID.ORGANIZATIONAL_UNIT_NAME, config_sub["unidad"]),
            x509.NameAttribute(NameOID.COMMON_NAME, config_sub["nombre"]),
        ])

        # Crear certificado firmado por AC Raíz
        certificado_sub = (
            x509.CertificateBuilder()
            .subject_name(nombre_sub)
            .issuer_name(cert_raiz.subject)  # Emisor es la AC Raíz
            .public_key(clave_publica_sub)
            .serial_number(x509.random_serial_number())
            .not_valid_before(datetime.utcnow())
            .not_valid_after(datetime.utcnow() + timedelta(days=365 * config_sub["validez_anos"]))
            .add_extension(
                x509.BasicConstraints(ca=True, path_length=0),  # Es AC pero no puede certificar otras ACs
                critical=True,
            )
            .add_extension(
                x509.KeyUsage(
                    digital_signature=True,
                    key_cert_sign=True,  # Puede firmar certificados de usuarios finales
                    crl_sign=True,
                    key_encipherment=False,
                    content_commitment=False,
                    data_encipherment=False,
                    key_agreement=False,
                    encipher_only=False,
                    decipher_only=False,
                ),
                critical=True,
            )
            .add_extension(
                x509.SubjectKeyIdentifier.from_public_key(clave_publica_sub),
                critical=False,
            )
            .add_extension(
                x509.AuthorityKeyIdentifier.from_issuer_public_key(cert_raiz.public_key()),
                critical=False,
            )
            .sign(clave_raiz, hashes.SHA256(), default_backend())  # Firmado por AC Raíz
        )

        # Guardar clave privada (protegida)
        contrasena_sub = b"AC_SUB_2025_UC3M_SEGURA"
        with open(ruta_clave, "wb") as f:
            f.write(clave_privada_sub.private_bytes(
                encoding=serialization.Encoding.PEM,
                format=serialization.PrivateFormat.PKCS8,
                encryption_algorithm=serialization.BestAvailableEncryption(contrasena_sub)
            ))

        # Guardar certificado
        with open(ruta_cert, "wb") as f:
            f.write(certificado_sub.public_bytes(serialization.Encoding.PEM))

        # Actualizar configuración
        config["ca_subordinada"]["fecha_creacion"] = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        JsonStore.guardar_json(self.ARCHIVO_PKI_CONFIG, config)

        # LOG
        CryptoLogger.log_crypto("CREACIÓN AC SUBORDINADA", {
            "Tipo": "Autoridad de Certificación Subordinada (Intermediate CA)",
            "Nombre": config_sub["nombre"],
            "Algoritmo clave": "RSA",
            "Tamaño clave": "2048 bits",
            "Algoritmo firma": "SHA-256 con RSA",
            "Validez": f"{config_sub['validez_anos']} años",
            "Certificado": "Firmado por AC Raíz",
            "Uso": "Firma de certificados de usuarios finales",
            "Path length": "0 (NO puede certificar otras ACs)"
        })

        print(f"[PKI] AC Subordinada creada y certificada por AC Raíz")
        print(f"      Certificado: {ruta_cert}")
        print(f"      Clave privada: {ruta_clave} (protegida con contraseña)")

        return True


    # -------------------- EMISIÓN DE CERTIFICADOS PARA USUARIOS --------------------

    def emitir_certificado_usuario(self, correo, nombre, clave_publica_pem):
        """
        Emite un certificado X.509 para un usuario, firmado por la AC Subordinada

        Args:
            correo: Correo electrónico del usuario
            nombre: Nombre completo del usuario
            clave_publica_pem: Clave pública RSA del usuario en formato PEM

        Returns:
            Certificado en formato PEM (string) o None si falla
        """
        ruta_ca_sub_cert = os.path.join(self.DIR_CA_SUBORDINADA,
                                        "ca_sub_cert.pem")
        ruta_ca_sub_key = os.path.join(self.DIR_CA_SUBORDINADA,
                                       "ca_sub_private.pem")

        # Verificar que existe la AC Subordinada
        if not os.path.exists(ruta_ca_sub_cert) or not os.path.exists(ruta_ca_sub_key):
            print("\n[PKI] ERROR: Debe crear primero la AC Subordinada.")
            return None

        try:
            # Cargar certificado y clave de AC Subordinada
            with open(ruta_ca_sub_cert, "rb") as f:
                cert_sub = x509.load_pem_x509_certificate(f.read(), default_backend())

            with open(ruta_ca_sub_key, "rb") as f:
                clave_sub = serialization.load_pem_private_key(
                    f.read(),
                    password=b"AC_SUB_2025_UC3M_SEGURA",
                    backend=default_backend()
                )

            # Cargar clave pública del usuario
            if isinstance(clave_publica_pem, str):
                clave_publica_pem = clave_publica_pem.encode('utf-8')
            clave_publica_usuario = serialization.load_pem_public_key(
                clave_publica_pem,
                backend=default_backend()
            )

            # Crear nombre distinguido del usuario
            nombre_usuario = x509.Name([
                x509.NameAttribute(NameOID.COUNTRY_NAME, "ES"),
                x509.NameAttribute(NameOID.ORGANIZATION_NAME, "Universidad Carlos III Madrid"),
                x509.NameAttribute(NameOID.ORGANIZATIONAL_UNIT_NAME, "Usuarios Mensajería"),
                x509.NameAttribute(NameOID.COMMON_NAME, nombre),
                x509.NameAttribute(NameOID.EMAIL_ADDRESS, correo),
            ])

            # Crear certificado para el usuario
            certificado_usuario = (
                x509.CertificateBuilder()
                .subject_name(nombre_usuario)
                .issuer_name(cert_sub.subject)  # Emisor es la AC Subordinada
                .public_key(clave_publica_usuario)
                .serial_number(x509.random_serial_number())
                .not_valid_before(datetime.utcnow())
                .not_valid_after(datetime.utcnow() + timedelta(days=365 * 2))  # 2 años
                .add_extension(
                    x509.BasicConstraints(ca=False, path_length=None),  # NO es AC
                    critical=True,
                )
                .add_extension(
                    x509.KeyUsage(
                        digital_signature=True,      # Para firmar mensajes
                        key_encipherment=True,       # Para cifrado asimétrico
                        content_commitment=True,     # No repudio
                        key_cert_sign=False,
                        crl_sign=False,
                        data_encipherment=False,
                        key_agreement=False,
                        encipher_only=False,
                        decipher_only=False,
                    ),
                    critical=True,
                )
                .add_extension(
                    x509.ExtendedKeyUsage([
                        x509.oid.ExtendedKeyUsageOID.EMAIL_PROTECTION,
                    ]),
                    critical=False,
                )
                .add_extension(
                    x509.SubjectAlternativeName([
                        x509.RFC822Name(correo),
                    ]),
                    critical=False,
                )
                .add_extension(
                    x509.SubjectKeyIdentifier.from_public_key(clave_publica_usuario),
                    critical=False,
                )
                .add_extension(
                    x509.AuthorityKeyIdentifier.from_issuer_public_key(cert_sub.public_key()),
                    critical=False,
                )
                .sign(clave_sub, hashes.SHA256(), default_backend())  # Firmado por AC Subordinada
            )

            # Convertir certificado a PEM
            cert_pem = certificado_usuario.public_bytes(serialization.Encoding.PEM).decode('utf-8')

            # Guardar certificado del usuario
            nombre_archivo = correo.replace("@", "_at_").replace(".", "_")
            ruta_cert_usuario = os.path.join(self.DIR_USUARIOS, f"{nombre_archivo}_cert.pem")
            with open(ruta_cert_usuario, "w", encoding='utf-8') as f:
                f.write(cert_pem)

            # Registrar en configuración
            config = JsonStore.cargar_json(self.ARCHIVO_PKI_CONFIG)
            config["certificados_emitidos"].append({
                "correo": correo,
                "nombre": nombre,
                "fecha_emision": datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
                "numero_serie": str(certificado_usuario.serial_number),
                "archivo": ruta_cert_usuario
            })
            JsonStore.guardar_json(self.ARCHIVO_PKI_CONFIG, config)

            # LOG
            CryptoLogger.log_crypto("EMISIÓN CERTIFICADO USUARIO", {
                "Tipo": "Certificado X.509 de usuario final",
                "Usuario": f"{nombre} ({correo})",
                "Emisor": "AC Subordinada - Usuarios Mensajería",
                "Número de serie": certificado_usuario.serial_number,
                "Algoritmo firma": "SHA-256 con RSA",
                "Validez": "2 años",
                "Uso clave": "Firma digital + Cifrado + No repudio",
                "Cadena confianza": "Usuario → AC Subordinada → AC Raíz"
            })

            print(f"[PKI] Certificado emitido para {nombre} ({correo})")
            print(f"      Archivo: {ruta_cert_usuario}")

            return cert_pem

        except Exception as e:
            CryptoLogger.log_crypto("ERROR EMISIÓN CERTIFICADO", {
                "Error": str(e),
                "Tipo": type(e).__name__
            })
            print(f"[PKI] ERROR al emitir certificado: {e}")
            return None

    def verificar_firma_criptografica(self, cert_hijo, clave_pub_padre):
        """
        Verifica matemáticamente que cert_hijo fue firmado por clave_pub_padre
        :param cert_hijo: Certificado del hijo
        :param clave_pub_padre: Clave pública del padre
        :return: Trie si cert_hijo se firmó con la clave pública del padre y
        False en caso contrario
        """
        try:
            clave_pub_padre.verify(
                cert_hijo.signature,
                cert_hijo.tbs_certificate_bytes,
                padding.PKCS1v15(),
                cert_hijo.signature_hash_algorithm
            )
            return True
        except Exception:
            return False



    def verificar_cadena_certificación(self, cert_usuario_pem):
        """
        Verifica toda la cadena: Usuario --> AC Subordinada --> AC Raíz
        Args:
            cert_usuario_pem: Certificado del usuario a verificar
        Return:
             True si la cadena de certificados es de confianza y False en caso
             contrario.
        """
        print("\n[PKI] Iniciando verificación de cadena de certificación...")

        try:
            # Cargamos el certificado del usuario
            if isinstance(cert_usuario_pem, str):
                cert_usuario_pem = cert_usuario_pem.encode('utf-8')
            cert_usuario = x509.load_pem_x509_certificate(cert_usuario_pem, default_backend())

            # Verificamos la fecha de validez
            ahora = datetime.now(timezone.utc)
            if not (cert_usuario.not_valid_before_utc <= ahora <=
                    cert_usuario.not_valid_after_utc):
                print("    El certificado del usuario ha expirado o no es válido aún.")
                return False

            # Cargamos el certificado AC Subordinada
            ruta_sub = os.path.join(self.DIR_CA_SUBORDINADA, "ca_sub_cert.pem")
            if not os.path.exists(ruta_sub):
                print("    Error: No se encuentra el certificado de la AC "
                      "Subordinada")
                return False
            with open(ruta_sub, "rb") as f:
                cert_sub = x509.load_pem_x509_certificate(f.read(), default_backend())

            # Verificamos con la clave pub de la sub si firmó la del usuario
            clave_pub_sub = cert_sub.public_key()
            if self.verificar_firma_criptografica(cert_usuario, clave_pub_sub):
                print("    Eslabón 1: Certificado Usuario firmado correctamente "
                      "por AC Subordinada")
            else:
                print("    Eslabón 1 ROTO: La firma de la AC Subordinada sobre "
                      "el usuario es inválida")
                return False

            # Cargamos el certificado AC Raíz
            ruta_raiz = os.path.join(self.DIR_CA_RAIZ, "ca_raiz_cert.pem")
            if not os.path.exists(ruta_raiz):
                print("    Error: No se encuentra el certificado de la AC "
                      "Subordinada")
                return False

            with open(ruta_raiz, "rb") as f:
                cert_raiz = x509.load_pem_x509_certificate(f.read(), default_backend())

            # Verificamos con la clave pub de la raíz si firmó la de la subordinada
            clave_pub_raiz = cert_raiz.public_key()
            if self.verificar_firma_criptografica(cert_sub, clave_pub_raiz):
                print("    Eslabón 2: Certificado  AC Subordinada firmado "
                      "correctamente por AC Raíz")
            else:
                print("    Eslabón 2 ROTO: La firma de la AC Raíz sobre la "
                      "Subordinada es inválida")
                return False

            CryptoLogger.log_crypto("VALIDACIÓN CADE CERTIFICACIÓN", {
                "Usuario" : cert_usuario.subject.rfc4514_string(),
                "Estado": "VÁLIDO",
                "Cadena": "Usuario -> AC Sub -> AC Raíz"
            })
            print("    Cadena de certificación completa verificada")
            return True
        except Exception as e:
            print(f"    Error durante la verificación: {e}")
            return False

    def buscar_certificado(self, correo_objetivo):
        """
        Busca el archivo de certificado X.509 del usuario en la carpeta PKI.
        """
        # Construimos el nombre del archivo con el formato correcto
        nombre_archivo = correo_objetivo.replace("@", "_at_").replace(".", "_")
        ruta_cert = os.path.join(self.DIR_USUARIOS, f"{nombre_archivo}_cert.pem")

        if os.path.exists(ruta_cert):
            return ruta_cert
        return None