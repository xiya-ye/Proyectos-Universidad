"""Module account_manager"""
import json
from pathlib import Path
from datetime import datetime, timezone
from uc3m_money.account_management_exception import AccountManagementException
from uc3m_money.transfer_request import TransferRequest
from uc3m_money.account_deposit import AccountDeposit

#from uc3m_money import AccountManagementException, TransferRequest

JSON_FILES_PATH = (str(Path.home()) +
                   "/PycharmProjects/G85.2025.T06.EG2/src/JsonFiles/")

class AccountManager:
    """Class for providing the methods for managing the orders"""
    def __init__(self):
        pass

    @staticmethod
    def validate_iban(iban: str):
        """RETURNs TRUE IF THE IBAN RECEIVED IS VALID SPANISH IBAN,
        OR FALSE IN OTHER CASE"""
        abecedario = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
                      'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
                      'W', 'X', 'Y', 'Z']

        # Comprobar que el formato de IBAN es correcto
        if not isinstance(iban, str):
            return False
        if (len(iban) != 24 or iban[0] not in abecedario or iban[1] not in
                abecedario):
            return False

        # Colocar los caracteres del iban
        iban = iban[4:] + iban[0:4]

        # Cambiar la primera letra
        for i, _ in enumerate(abecedario):
            if iban[20] == abecedario[i]:
                num = i + 10
                iban = iban[:20] + str(num) + iban[21:]

        # Cambiar la segunda letra
        for i, _ in enumerate(abecedario):
            if iban[22] == abecedario[i]:
                num = i + 10
                iban = iban[:22] + str(num) + iban[23:]

        # Comprobar que en los ultimos 22 digitos sean todos de tipo int
        # digitos = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"]
        for i in iban:
            if not i.isdigit():
                return False

        # Convertir el iban a una variable tipo int
        iban = int(iban)
        # Comprobar el iban
        return bool(iban % 97 == 1)

        # return True

    @staticmethod
    def validate_concept(concept: str):
        """metodo que valida el concepto de una transferencia"""
        # comprobamos si es disntinto de string
        if not isinstance(concept, str):
            return False

        # comprobamos que tenga entre 10 y 30 chars
        if not 10 <= len(concept) <= 30:
            return False

        # comprobamos si esta formado por caracteres de [a-z A-Z]
        abecedario = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
                      'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
                      'y', 'z','A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                      'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
                      'W', 'X', 'Y', 'Z', " "]
        for caracter in concept:
            if caracter not in abecedario:
                return False

        # comprobamos que haya al menos dos cadenas de caracteres
        palabras = concept.split()
        if len(palabras) < 2:
            return False

        return True

    @staticmethod
    def validate_type(type_: str):
        """metodo que valida el type de una transferencia"""
        if not isinstance(type_, str):
            return False
        valid_type = ["ORDINARY", "URGENT", "INMEDIATE"]
        if type_ not in valid_type:
            return False

        return True

    @staticmethod
    def validate_date(date:str):
        """metodo que valida la fecha para la transferencia"""
        if not isinstance(date, str):
            return False
        if len(date) != 10:
            return False

        fecha = date.split("/")
        if len(fecha) != 3:
            return False

        if (1 <= int(fecha[0]) <= 31) and ( 1 <= int(fecha[1]) <= 12) and (
                2025 <= int(fecha[2]) <= 2050 ):
            return True
        return False

    @staticmethod
    def validate_amount_tr(amount: float):
        """metodo que valida el amount para la transferencia"""
        if not isinstance(amount, float):
            return False

        partes = str(amount).split(".")
        if len(partes[1]) > 2:
            return False

        if amount < 10.00 or amount > 10000.00:
            return False

        return True


    @staticmethod
    def validate_amount_deposit(deposit_amount: str):
        """metodo que valida la cantidad"""
        # numeros = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"]


        #Comprobar si la longitud es correcta
        if len(deposit_amount) != 11:
            return False
        #Comprobar el caracter 8 de amount
        if deposit_amount[8] != ".":
            return False

        #Validar EUR:
        if (deposit_amount[0] != "E" or deposit_amount[1] != "U"
            or deposit_amount[2] != "R" or deposit_amount[3] != " "):
            return  False

        #Validar primera parte de amount
        i = 3
        while i <= 6:
            i += 1
            if not deposit_amount[i].isdigit():
                return  False

        #Validar segunda parte de amount
        i = 8
        while i <= 9:
            i += 1
            if not deposit_amount[i].isdigit():
                return False

        return True

    # pylint: disable=too-many-arguments
    # pylint: disable=too-many-positional-arguments
    @staticmethod
    def transfer_request(from_iban, to_iban, concept, type_, date, amount):
        """función 1"""
        if not AccountManager.validate_iban(from_iban):
            raise AccountManagementException("ERROR: from_iban not valid")

        if not AccountManager.validate_iban(to_iban):
            raise AccountManagementException("ERROR: to_iban not valid")

        if not AccountManager.validate_concept(concept):
            raise AccountManagementException("ERROR: concept not valid")

        if not AccountManager.validate_type(type_):
            raise AccountManagementException("ERROR: type not valid")

        if not AccountManager.validate_amount_tr(amount):
            raise AccountManagementException("ERROR: amount not valid")

        if not AccountManager.validate_date(date):
            raise AccountManagementException("ERROR: date not valid")

        tr = TransferRequest(from_iban=from_iban, transfer_type=type_,
                             to_iban=to_iban, transfer_concept=concept,
                             transfer_date=date, transfer_amount=amount)

        # definimos la ruta del archivo de las transferencias
        file_transfer_store = JSON_FILES_PATH + "RF1/transfer_requests.json"

        # leemos el archivo JSON y si no existe la creamos
        try:
            with open(file_transfer_store, "r", encoding="utf-8",
                      newline="") as file:
                data_list = json.load(file)
        except FileNotFoundError:
            data_list = []
        except json.JSONDecodeError as e:
            raise AccountManagementException("ERROR: JSON decode error, "
                                             "wrong JSON format") from e

        # comprobamos si ya existe una transferencia con los mismos datos
        for data in data_list:
            if data['transfer_code'] == tr.transfer_code:
                raise AccountManagementException("ERROR: transfer request "
                                                 "with same data already "
                                                 "exists")

        # agregamos la nueva transferencia al archivo JSON
        data_list.append(tr.to_json())
        try:
            with open(file_transfer_store, "w", encoding="utf-8",
                      newline="") as file:
                json.dump(data_list, file, indent=2)
        except FileNotFoundError as e:
            raise AccountManagementException("ERROR: wrong file or file "
                                             "path") from e

        # devolvemos MD5
        md5_signature = tr.transfer_code
        return md5_signature

    @staticmethod
    def deposit_into_account(input_file:str):
        """ Metodo que valida una solicitud de deposito desde un archivo
        JSON"""

        #Leer el archivo JSON y crearlo en caso de que no exista
        try:
            with open(input_file, "r", encoding="utf-8", newline="") as file:
                data = json.load(file)
        except FileNotFoundError as exc:
            raise AccountManagementException("ERROR: input JSON not found") \
                from exc
        except json.JSONDecodeError as e:
            raise AccountManagementException("ERROR: input JSON decode error, "
                                             "wrong input JSON format") from e


        #Validar que tiene las claves requeridas
        required_fields = ["IBAN", "AMOUNT"]
        for field in required_fields:
            if field not in data:
                raise AccountManagementException("ERROR: field is missing")

        #Validar los valores del depósito
        if not AccountManager.validate_iban(data['IBAN']):
            raise AccountManagementException("ERROR: IBAN not valid")

        if not AccountManager.validate_amount_deposit(data['AMOUNT']):
            raise AccountManagementException("ERROR: Amount not valid")

        # generamos la instancia de la clase Account Deposit
        dia = AccountDeposit(to_iban=data['IBAN'], deposit_amount=data['AMOUNT'])

        # Ruta del archivo json
        deposit_account_file = JSON_FILES_PATH + "RF2/deposit_requests.json"

        # leemos el archivo JSON y si no existe la creamos
        try:
            with open(deposit_account_file, "r", encoding="utf-8",
                      newline="") as file:
                data_list = json.load(file)
        except FileNotFoundError:
            data_list = []
        except json.JSONDecodeError as e:
            raise AccountManagementException("ERROR: JSON decode error, "
                                             "wrong JSON format") from e

        # agregamos los datos al archivo JSON
        data_list.append(dia.to_json())
        with (open(deposit_account_file, "w", encoding="utf-8", newline="")
              as file):
            json.dump(data_list, file, indent=2)

        # devolvemos el hash
        try:
            hash_signature = dia.deposit_signature
        except Exception as exc:
            raise AccountManagementException("ERROR: error generating "
                                             "deposit signature") from exc

        return hash_signature

    @staticmethod
    def calculate_balance(iban_number:str):
        """funcion 3"""

        #Verificar sii el iban es valido con la función validate_iban
        if not AccountManager.validate_iban(iban_number):
            raise AccountManagementException("ERROR: iban_number not valid")

        # abrimos fichero transaction.json
        transaction_file = JSON_FILES_PATH + "RF3/transactions.json"
        try:
            with open(transaction_file, "r", encoding="utf-8",
                      newline="") as file:
                data_list = json.load(file)
        except FileNotFoundError as exc:
            raise AccountManagementException("ERROR: wrong file or file "
                                             "path") from exc
        except json.JSONDecodeError as e:
            raise AccountManagementException("ERROR: JSON decode error") from e

        total_amount = 0.00
        found = False
        for transaction in data_list:
            if transaction['IBAN'] == iban_number:
                found = True
                amount = float(transaction['amount'])
                total_amount += amount

        if not found:
            raise AccountManagementException("El IBAN introducido no está en el fichero")

        justnow = datetime.now(timezone.utc)
        time_stamp = datetime.timestamp(justnow)
        result = {
            "IBAN": iban_number,
            "balance": round(total_amount,2),
            "time_stamp": time_stamp
        }

        # leemos el archivo JSON y si no existe la creamos
        calculate_balance_file = JSON_FILES_PATH + "RF3/balances.json"
        try:
            with open(calculate_balance_file, "r", encoding="utf-8",
                      newline="") as file:
                data_list = json.load(file)
        except FileNotFoundError:
            data_list = []
        except json.JSONDecodeError as e:
            raise AccountManagementException("ERROR: JSON decode error") from e

        # agregamos los datos al archivo JSON
        data_list.append(result)
        with (open(calculate_balance_file, "w", encoding="utf-8", newline="")
              as file):
            json.dump(data_list, file, indent=2)

        return True
