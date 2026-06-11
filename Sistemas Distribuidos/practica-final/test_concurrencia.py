import pexpect
import threading
import time
import random

SERVER_IP = "localhost"
SERVER_PORT = "8888"

def cliente(nombre_remitente, nombre_destino, num_mensajes):
    print(f"{nombre_remitente} Arrancando...")
    
    c = pexpect.spawn(f'python3 client.py -s {SERVER_IP} -p {SERVER_PORT}', encoding='utf-8')
    
    try:
        c.expect('c> ', timeout=5)
        
        # 1. Registro y conexión
        c.sendline(f'REGISTER {nombre_remitente}')
        c.expect('c> ')
        
        c.sendline(f'CONNECT {nombre_remitente}')
        c.expect('c> ')
        
        # 2. Esperamos un poco para asegurarnos de que todos los usuarios han tenido 
        # tiempo de conectarse antes de empezar a mandar mensajes
        time.sleep(2)
        
        # 3. Bombardeo de mensajes
        print(f"{nombre_remitente} Empezando a enviar mensajes a {nombre_destino}")
        for i in range(1, num_mensajes + 1):
            c.sendline(f'SEND {nombre_destino} Mensaje {i} de {nombre_remitente}')
            c.expect('c> ')
            
            time.sleep(random.uniform(0.01, 0.1))
            
        # 4. Salir
        c.sendline(f'DISCONNECT {nombre_remitente}')
        c.expect('c> ')
        c.sendline('QUIT')
        
        print(f"{nombre_remitente} Terminó con éxito.")
        
    except pexpect.exceptions.TIMEOUT:
        print(f"{nombre_remitente} se quedo esperando una respuesta y se agotó el tiempo.")
    finally:
        c.close()


if __name__ == "__main__":
    print("Iniciando prueba de concurrencia:")
    
    usuario = ["Lucia", "Jose", "Diana"]
    num_mensajes = 50
    hilos = []

    # Lanzamos un hilo por cada usuario
    for nombre in usuario:
        # Cada usuario enviará mensajes a "Jorge"
        hilo = threading.Thread(target=cliente, args=(nombre, "Jorge", num_mensajes))
        hilos.append(hilo)
        hilo.start()

    # Esperamos a que todos los usuarios terminen de madar los mensajes
    for hilo in hilos:
        hilo.join()

    print("Prueba de concurrencia finalizada.")


