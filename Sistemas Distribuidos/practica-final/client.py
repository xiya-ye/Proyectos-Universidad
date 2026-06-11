from enum import Enum
import argparse
import os
import socket
import threading
import requests
import json

class client :

    # ******************** TYPES *********************
    # *
    # * @brief Return codes for the protocol methods
    class RC(Enum) :
        OK = 0
        ERROR = 1
        USER_ERROR = 2

    # ****************** ATTRIBUTES ******************
    _server = None
    _port = -1
    _user = None
    _listen_thread = None
    _listen_socket = None
    _usuarios_conectados = {}

    # ******************** METHODS *******************

    @staticmethod
    def _write_string(sock, text):
        sock.sendall(text.encode() + b'\0')  

    @staticmethod
    def _read_string(sock):
        a = ''
        while True:
            msg = sock.recv(1)
            if (msg == b'\0'):
                break
            a += msg.decode()
        return a

    @staticmethod
    def _tratar_hilo():
        while True:
            try:
                # Aceptamos conexiones entrantes del servidor
                conexion, direccion = client._listen_socket.accept()

                # Leemos la operación que manda el servidor
                operacion = client._read_string(conexion)

                if operacion == "SEND_MESSAGE":
                    remitente = client._read_string(conexion)
                    id_mensaje = client._read_string(conexion)
                    mensaje = client._read_string(conexion)
                    print(f"\nc> MESSAGE {id_mensaje} FROM {remitente}\n{mensaje}\nEND\nc> ", end="", flush=True)
                
                elif operacion == "SEND_MESS_ACK":
                    id_mensaje = client._read_string(conexion)
                    print(f"\nc> SEND MESSAGE {id_mensaje} OK\nc> ", end="", flush=True)
                
                elif operacion == "SEND_MESSAGE_ATTACH":
                    remitente = client._read_string(conexion)
                    id_mensaje = client._read_string(conexion)
                    mensaje = client._read_string(conexion)
                    fichero = client._read_string(conexion)
                    print(f"\nc> MESSAGE {id_mensaje} FROM {remitente}\n{mensaje}\nEND\nFILE: {fichero}\nc> ", end="", flush=True)
                
                elif operacion == "SEND_MESSAGE_ATTACH_ACK":
                    id_mensaje = client._read_string(conexion)
                    fichero = client._read_string(conexion)
                    print(f"\nc> SENDATTACH MESSAGE {id_mensaje} {fichero} OK\nc> ", end="", flush=True)

                elif operacion == "GET_FILE":
                    remitente = client._read_string(conexion)
                    fichero = client._read_string(conexion)

                    try:
                        with open(fichero, 'rb') as f:
                            while True:
                                datos = f.read(2048)
                                if not datos:   # final de archivo
                                    break
                                conexion.sendall(datos)
                    # Si el fichero no existe
                    except FileNotFoundError:
                        pass
                conexion.close()
            except Exception as e:
                break
    
    @staticmethod
    def _remove_nwhitespaces(message):
        """
        Elimina los espacios en blanco repetidos de un mensaje
        """
        try:
            # enviamos peticion al servidor web
            response = requests.post(
                'http://localhost:8000/remove_nwhitespaces',
                json={'message': message},
                timeout=2 #timeout de 2 segundos para evitar bloqueos
            )

            # si la respuesta es exitosa, devolver el mensaje normalizado
            if response.status_code == 202:
                return response.text
            else:
                # si hay error, usar mensaje original
                return message
        except Exception as e:
            #si el servicio web no esta disponible, usar mensaje original
            print(f"c> Warning: Message normalizer is not available: {e}")
            return message

    # *
    # * @param user - User name to register in the system
    # * 
    # * @return OK if successful
    # * @return USER_ERROR if the user is already registered
    # * @return ERROR if another error occurred
    @staticmethod
    def  register(user) :
        #  Write your code here
        # Creamos el socket TCP y conectamos al servidor
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_address = (client._server, client._port)

        try:
            sock.connect(server_address)

            # Enviamos el comando REGISTER y el nombre de usuario al servidor
            client._write_string(sock, "REGISTER")
            client._write_string(sock, user)

            # Recibimos la respuesta del servidor
            respuesta = sock.recv(1)

            if not respuesta:
                print("c> REGISTER FAIL")
                return client.RC.ERROR
            
            res_byte = int.from_bytes(respuesta, byteorder='big')

            if res_byte == 0:
                print("c> REGISTER OK")
                return client.RC.OK
            elif res_byte == 1:
                print("c> REGISTER FAIL, USER ALREADY EXISTS")
                return client.RC.USER_ERROR
            else:
                print("c> REGISTER FAIL")
                return client.RC.ERROR
        
        except socket.error as e:
            print("c> REGISTER FAIL")
            return client.RC.ERROR
        
        finally:
            sock.close()

    # *
    # 	 * @param user - User name to unregister from the system
    # 	 * 
    # 	 * @return OK if successful
    # 	 * @return USER_ERROR if the user does not exist
    # 	 * @return ERROR if another error occurred
    @staticmethod
    def  unregister(user) :
        #  Write your code here
        # Creamos el socket TCP y conectamos al servidor
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_address = (client._server, client._port)

        try:
            sock.connect(server_address)

            # Enviamos el comando UNREGISTER y el nombre de usuario al servidor
            client._write_string(sock, "UNREGISTER")
            client._write_string(sock, user)

            # Recibimos la respuesta del servidor
            respuesta = sock.recv(1)

            if not respuesta:
                print("c> UNREGISTER FAIL")
                return client.RC.ERROR
            
            res_byte = int.from_bytes(respuesta, byteorder='big')

            if res_byte == 0:
                print("c> UNREGISTER OK")
                return client.RC.OK
            elif res_byte == 1:
                print("c> USER DOES NOT EXIST")
                return client.RC.USER_ERROR
            else:
                print("c> UNREGISTER FAIL")
                return client.RC.ERROR
        
        except socket.error as e:
            print("c> UNREGISTER FAIL")
            return client.RC.ERROR
        
        finally:
            sock.close()


    # *
    # * @param user - User name to connect to the system
    # * 
    # * @return OK if successful
    # * @return USER_ERROR if the user does not exist or if it is already connected
    # * @return ERROR if another error occurred
    @staticmethod
    def  connect(user) :
        #  Write your code here
        # Buscamos un puerto válido que esté libre
        try:
            sock_listen = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock_listen.bind(('', 0))
            port = sock_listen.getsockname()[1]
            sock_listen.listen(5)
            client._listen_socket = sock_listen
        except socket.error as e:
            print("c> CONNECT FAIL")
            return client.RC.ERROR

        client._listen_thread = threading.Thread(target=client._tratar_hilo)
        client._listen_thread.daemon = True
        client._listen_thread.start()

        # Creamos el socket TCP y conectamos al servidor
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_address = (client._server, client._port)

        try:
            sock.connect(server_address)

            # Enviamos el comando CONNECT y el nombre de usuario al servidor
            client._write_string(sock, "CONNECT")
            client._write_string(sock, user)
            client._write_string(sock, str(port))

            # Recibimos la respuesta del servidor
            respuesta = sock.recv(1)
            if not respuesta:
                print("c> CONNECT FAIL")
                return client.RC.ERROR
            res_byte = int.from_bytes(respuesta, byteorder='big')

            if res_byte == 0:
                print("c> CONNECT OK")
                client._user = user     # guardamos el usuario conectado
                return client.RC.OK
            elif res_byte == 1:
                print("c> CONNECT FAIL, USER DOES NOT EXIST")
                return client.RC.USER_ERROR
            elif res_byte == 2:
                print("c> USER ALREADY CONNECTED")
                return client.RC.USER_ERROR
            else:
                print("c> CONNECT FAIL")
                return client.RC.ERROR
        
        except socket.error as e:
            print("c> CONNECT FAIL")
            return client.RC.ERROR

        finally:
            sock.close()

        return client.RC.ERROR

    # *
    # * @return OK if successful
    # * @return USER_ERROR if the user does not exist or if it is already connected
    # * @return ERROR if another error occurred
    @staticmethod
    def users(no_imprimir=False) :
        if client._user is None:
            print("c> CONNECTED USERS FAIL, USER IS NOT CONNECTED")
            return client.RC.ERROR
        #  Write your code here
        # Creamos el socket TCP y conectamos al servidor
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_address = (client._server, client._port)

        try:
            sock.connect(server_address)

            # Enviamos el comando USERS al servidor
            client._write_string(sock, "USERS")

            # Enviamos el nombre del usuario
            client._write_string(sock, client._user)

            # Recibimos la respuesta del servidor
            respuesta = sock.recv(1)
            if not respuesta:
                print("c> CONNECTED USERS FAIL")
                return client.RC.ERROR
            res_byte = int.from_bytes(respuesta, byteorder='big')

            if res_byte == 0:
                # Leemos el número de usuarios conectados
                num_users_str = client._read_string(sock)
                num_users = int(num_users_str)
                if not no_imprimir:
                    print(f"c> CONNECTED USERS ({num_users} users connected) OK")

                client._usuarios_conectados.clear()
                # Imprimimos los datos de los usuarios conectados
                for _ in range(num_users):
                    user_conectado = client._read_string(sock)

                    partes = [parte.strip() for parte in user_conectado.split(" :: ")]

                    if len(partes) >= 3:
                        nombre = partes[0]
                        ip = partes[1]
                        puerto = int(partes[2])

                        client._usuarios_conectados[nombre] = (ip, puerto)

                        if not no_imprimir:
                            print(nombre)
                return client.RC.OK
            
            elif res_byte == 1:
                print("c> CONNECTED USERS FAIL, USER IS NOT CONNECTED")
                return client.RC.USER_ERROR
            else:
                print("c> CONNECTED USERS FAIL")
                return client.RC.ERROR
            
        except socket.error as e:
            print("c> CONNECTED USERS FAIL")
            return client.RC.ERROR
        
        finally:
            sock.close()


    # *
    # * @param user - User name to disconnect from the system
    # * 
    # * @return OK if successful
    # * @return USER_ERROR if the user does not exist
    # * @return ERROR if another error occurred
    @staticmethod
    def  disconnect(user) :
        #  Write your code here
        # Creamos el socket TCP y conectamos al servidor
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_address = (client._server, client._port)

        try: 
            sock.connect(server_address)

            # Enviamos el comando DISCONNECT y el nombre de usuario al servidor
            client._write_string(sock, "DISCONNECT")
            client._write_string(sock, user)

            # Recibimos la respuesta del servidor
            respuesta = sock.recv(1)
            if not respuesta:
                print("c> DISCONNECT FAIL")
                return client.RC.ERROR
            res_byte = int.from_bytes(respuesta, byteorder='big')

            if res_byte == 0:
                print("c> DISCONNECT OK")
                if client._listen_socket:
                    client._listen_socket.close()
                client._user = None     # borramos al usuario
                return client.RC.OK
            
            elif res_byte == 1:
                print("c> DISCONNECT FAIL, USER DOES NOT EXIST")
                return client.RC.USER_ERROR
            
            elif res_byte == 2:
                print("c> DISCONNECT FAIL, USER NOT CONNECTED")
                return client.RC.USER_ERROR
            
            else:
                print("c> DISCONNECT FAIL")
                return client.RC.ERROR
        
        except socket.error as e:
            print("c> DISCONNECT FAIL")
            return client.RC.ERROR
        
        finally:
            sock.close()

    # *
    # * @param user    - Receiver user name
    # * @param message - Message to be sent
    # * 
    # * @return OK if the server had successfully delivered the message
    # * @return USER_ERROR if the user is not connected (the message is queued for delivery)
    # * @return ERROR the user does not exist or another error occurred
    @staticmethod
    def  send(user,  message) :
        #  Write your code here
        # Si el usuario no está conectado
        if client._user is None:
            print("c> SEND FAIL")
            return client.RC.ERROR
        
        #Normalizamos el mensaje antes de enviarlo al servidor
        normalized_message = client._remove_nwhitespaces(message)
        
        # Creamos el socket TCP y conectamos al servidor
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_address = (client._server, client._port)

        try:
            sock.connect(server_address)

            # Enviamos el comando SEND, el remitente, destinatario y el mensaje al servidor
            client._write_string(sock, "SEND")
            client._write_string(sock, client._user)
            client._write_string(sock, user)
            client._write_string(sock, normalized_message)

            # Recibimos la respuesta del servidor
            respuesta = sock.recv(1)
            if not respuesta:
                print("c> SEND FAIL")
                return client.RC.ERROR
            res_byte = int.from_bytes(respuesta, byteorder='big')

            if res_byte == 0:
                id_mensaje = client._read_string(sock)
                print(f"c> SEND OK - MESSAGE {id_mensaje}")
                return client.RC.OK
            
            elif res_byte == 1:
                print("c> SEND FAIL, USER DOES NOT EXIST")
                return client.RC.USER_ERROR
            else:
                print("c> SEND FAIL")
                return client.RC.ERROR
        
        except socket.error as e:
            print("c> SEND FAIL")
            return client.RC.ERROR
        
        finally:
            sock.close()

    # *
    # * @param user    - Receiver user name
    # * @param file    - file  to be sent
    # * @param message - Message to be sent
    # * 
    # * @return OK if the server had successfully delivered the message
    # * @return USER_ERROR if the user is not connected (the message is queued for delivery)
    # * @return ERROR the user does not exist or another error occurred
    @staticmethod
    def  sendAttach(user,  file,  message) :
        #  Write your code here
        if client._user is None:
            print("c> SENDATTACH FAIL")
            return client.RC.ERROR

        
        #normalizamos el mensaje
        normalized_message = client._remove_nwhitespaces(message)
        
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_address = (client._server, client._port)

        try:
            sock.connect(server_address)

            # Enviamos el comando SENDATTACH, el remitente, destinatario, el mensaje y el nombre del fichero
            client._write_string(sock, "SENDATTACH")
            client._write_string(sock, client._user)
            client._write_string(sock, user)
            client._write_string(sock, normalized_message)
            client._write_string(sock, file)

            respuesta = sock.recv(1)
            if not respuesta:
                print("c> SENDATTACH FAIL")
                return client.RC.ERROR
            
            res_byte = int.from_bytes(respuesta, byteorder='big')

            if res_byte == 0:
                id_mensaje = client._read_string(sock)
                print(f"c> SENDATTACH OK - MESSAGE {id_mensaje}")
                return client.RC.OK
            elif res_byte == 1:
                print("c> SENDATTACH FAIL, USER DOES NOT EXIST")
                return client.RC.USER_ERROR
            else:
                print("c> SENDATTACH FAIL")
                return client.RC.ERROR
        except socket.error as e:
            print("c> SENDATTACH FAIL")
            return client.RC.ERROR
        finally:
            sock.close()

    @staticmethod
    def getfile(user, file_name, local_file_name):
        # Si el usuario no está conectado
        if user not in client._usuarios_conectados:
            client.users(no_imprimir=True)   # actualizamos la lista de usuarios conectados
        
        # Comprobamos si el usuario está conectado
        if user not in client._usuarios_conectados:
            print("c> FILE TRANSFER FAILED, user not connected")
            return client.RC.USER_ERROR
        
        # Obtenemos la IP y puerto del usuario
        ip, puerto = client._usuarios_conectados[user]

        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.connect((ip, puerto))

            # Enviamos el comando GETFILE, el nombre del fichero y el nombre del fichero local
            client._write_string(sock, "GET_FILE")
            client._write_string(sock, client._user)
            client._write_string(sock, file_name)

            # Recibimos la respuesta del usuario y guardamos en el archivo local
            with open(local_file_name, 'wb') as f:
                while True:
                    datos = sock.recv(2048)
                    if not datos:   # final de archivo
                        break
                    f.write(datos)
            print(f"c> FILE {local_file_name} DOWNLOADED OK")
            return client.RC.OK
        
        except socket.error as e:
            print("c> FILE TRANSFER FAILED, user not connected")
            return client.RC.ERROR
        
        finally:
            sock.close()



    # *
    # **
    # * @brief Command interpreter for the client. It calls the protocol functions.
    @staticmethod
    def shell():

        while (True) :
            try :
                command = input("c> ")
                line = command.split(" ")
                if (len(line) > 0):

                    line[0] = line[0].upper()

                    if (line[0]=="REGISTER") :
                        if (len(line) == 2) :
                            client.register(line[1])
                        else :
                            print("Syntax error. Usage: REGISTER <userName>")

                    elif(line[0]=="UNREGISTER") :
                        if (len(line) == 2) :
                            client.unregister(line[1])
                        else :
                            print("Syntax error. Usage: UNREGISTER <userName>")

                    elif(line[0]=="CONNECT") :
                        if (len(line) == 2) :
                            client.connect(line[1])
                        else :
                            print("Syntax error. Usage: CONNECT <userName>")

                    elif(line[0]=="DISCONNECT") :
                        if (len(line) == 2) :
                            client.disconnect(line[1])
                        else :
                            print("Syntax error. Usage: DISCONNECT <userName>")

                    elif(line[0]=="USERS") :
                        if (len(line) == 1) :
                            client.users()
                        else :
                            print("Syntax error. Usage: USERS")

                    elif(line[0]=="SEND") :
                        if (len(line) >= 3) :
                            #  Remove first two words
                            message = ' '.join(line[2:])
                            client.send(line[1], message)
                        else :
                            print("Syntax error. Usage: SEND <userName> <message>")

                    elif(line[0]=="SENDATTACH") :
                        if (len(line) >= 4) :
                            user = line[1]
                            nombre_fichero = line[-1]
                            mensaje = ' '.join(line[2:-1])
                            client.sendAttach(user, nombre_fichero, mensaje)
                        else :
                            print("Syntax error. Usage: SENDATTACH <userName> <filename> <message>")

                    elif (line[0]=="GETFILE"):
                        if (len(line) >= 4):
                            client.getfile(line[1], line[2], line[3])
                        else :
                            print("Syntax error. Usage: GETFILE <userName> <fileName> <localFileName>")

                    elif(line[0]=="QUIT") :
                        if (len(line) == 1) :
                            break
                        else :
                            print("Syntax error. Use: QUIT")
                    else :
                        print("Error: command " + line[0] + " not valid.")
            except sock.err as e:
                print("sock.err: " + str(e))

    # *
    # * @brief Prints program usage
    @staticmethod
    def usage() :
        print("Usage: python3 client.py -s <server> -p <port>")


    # *
    # * @brief Parses program execution arguments
    @staticmethod
    def  parseArguments(argv) :
        parser = argparse.ArgumentParser()
        parser.add_argument('-s', type=str, required=True, help='Server IP')
        parser.add_argument('-p', type=int, required=True, help='Server Port')
        args = parser.parse_args()

        if (args.s is None):
            parser.error("Usage: python3 client.py -s <server> -p <port>")
            return False

        if ((args.p < 1024) or (args.p > 65535)):
            parser.error("Error: Port must be in the range 1024 <= port <= 65535");
            return False;
        
        client._server = args.s
        client._port = args.p

        return True


    # ******************** MAIN *********************
    @staticmethod
    def main(argv) :
        if (not client.parseArguments(argv)) :
            client.usage()
            return

        #  Write code here
        client.shell()
        print("+++ FINISHED +++")
    

if __name__=="__main__":
    client.main([])
