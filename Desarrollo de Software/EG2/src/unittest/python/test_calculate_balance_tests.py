"""class for testing the calculate_balance method"""
import json
import os
import random
import unittest
from uc3m_money import AccountManager, AccountManagementException, JSON_FILES_PATH
from freezegun import freeze_time

class MyTestCase(unittest.TestCase):
    """class for testing the calculate_balance method"""

    @freeze_time("03/03/2025")
    def test_valid_tc1(self):
        """test valido 1"""
        # leemos el fichero y lo guardamos en una variable
        transaction_file = JSON_FILES_PATH + "RF3/transactions.json"
        with open(transaction_file, "r", encoding="utf-8",
                      newline="") as file:
            data_list = json.load(file)

        # escribimos en el fichero los datos de la prueba
        file_content = [
                        {
                            "IBAN": "ES8658342044541216872704",
                            "amount": "-1280.06"
                        }
                        ]
        with open(transaction_file, "w", encoding="utf-8", newline="") as file:
            json.dump(file_content, file, indent=2)

        # llamamos a nuestra función
        my_manager = AccountManager()
        iban_number = "ES8658342044541216872704"
        result = my_manager.calculate_balance(iban_number)
        self.assertEqual(result, True)

        # devolvemos el contenido original del archivo
        with open(transaction_file, "w", encoding="utf-8", newline="") as file:
            json.dump(data_list, file, indent=2)

    def test_not_valid_tc2(self):
        """test no valido 2"""

        # llamamos a nuestra función
        my_manager = AccountManager()
        iban_number = 4486583420445412168727045
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.calculate_balance(iban_number)
        self.assertEqual(str(cm.exception), "ERROR: iban_number not valid")





    def test_not_valid_tc3(self):
        """test no valido 3"""
        # llamamos a nuestra función
        my_manager = AccountManager()
        iban_number = "ES86583420445412168727045654"

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.calculate_balance(iban_number)
        self.assertEqual(str(cm.exception), "ERROR: iban_number not valid")

    def test_not_valid_tc4(self):
        """test no valido 4"""

        # llamamos a nuestra función
        my_manager = AccountManager()
        iban_number = "0S86583420445412168727045"

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.calculate_balance(iban_number)
        self.assertEqual(str(cm.exception), "ERROR: iban_number not valid")


    def test_not_valid_tc5(self):
        """test no valido 5"""

        # llamamos a nuestra función
        my_manager = AccountManager()
        iban_number = "E086583420445412168727045"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.calculate_balance(iban_number)

        self.assertEqual(str(cm.exception), "ERROR: iban_number not valid")

        ###TEST 6,7,8,9,10


    def test_not_valid_tc11(self):
        """test no valido 11"""

        # llamamos a nuestra función
        my_manager = AccountManager()
        iban_number = "ESXXXXXXXXXXXXXXXXXXXXXX"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.calculate_balance(iban_number)

        self.assertEqual(str(cm.exception), "ERROR: iban_number not valid")


    def test_not_valid_tc12(self):
        """test no valido 12"""

        # llamamos a nuestra función
        my_manager = AccountManager()
        iban_number = "ES86583420445412168727047"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.calculate_balance(iban_number)

        self.assertEqual(str(cm.exception), "ERROR: iban_number not valid")




    @freeze_time("03/03/2025")
    def test_not_valid_tc13(self):
        """test no valido 13"""
        # leemos el fichero y lo guardamos en una variable
        transaction_file = JSON_FILES_PATH + "RF3/transactions.json"
        with open(transaction_file, "r", encoding="utf-8",
                  newline="") as file:
            data_list = json.load(file)

        # eliminamos el archivo
        os.remove(transaction_file)

        # llamamos a nuestra función
        my_manager = AccountManager()
        iban_number = "ES8658342044541216872704"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.calculate_balance(iban_number)
        self.assertEqual(cm.exception.message,
                         "ERROR: wrong file or file path")

        # devolvemos el contenido original del archivo
        with open(transaction_file, "w", encoding="utf-8", newline="") as file:
            json.dump(data_list, file, indent=2)

    @freeze_time("03/03/2025")
    def test_not_valid_tc14(self):
        """test no valido 14"""
        # leemos el fichero y lo guardamos en una variable
        transaction_file = JSON_FILES_PATH + "RF3/transactions.json"
        with open(transaction_file, "r", encoding="utf-8",
                  newline="") as file:
            data_list = json.load(file)

        # escribimos en el fichero los datos de la prueba
        file_content = """"[
                "IBAN": "ES8658342044541216872704",
                "amount": "-1280.06"
            }
        ]"""
        with open(transaction_file, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        # llamamos a nuestra función
        my_manager = AccountManager()
        iban_number = "ES8658342044541216872704"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.calculate_balance(iban_number)
        self.assertEqual(cm.exception.message,"ERROR: JSON decode error")

        # devolvemos el contenido original del archivo
        with open(transaction_file, "w", encoding="utf-8", newline="") as file:
            json.dump(data_list, file, indent=2)

    @freeze_time("03/03/2025")
    def test_not_valid_tc15(self):
        """test no valido 15"""
        # leemos el fichero y lo guardamos en una variable
        transaction_file = JSON_FILES_PATH + "RF3/transactions.json"
        with open(transaction_file, "r", encoding="utf-8",
                  newline="") as file:
            data_list = json.load(file)

        # escribimos en el fichero los datos de la prueba
        file_content = []
        with open(transaction_file, "w", encoding="utf-8", newline="") as file:
            json.dump(file_content, file, indent=2)

        # llamamos a nuestra función
        my_manager = AccountManager()
        iban_number = "ES8658342044541216872704"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.calculate_balance(iban_number)
        self.assertEqual(cm.exception.message, "El IBAN introducido no está "
                                               "en el fichero")

        # devolvemos el contenido original del archivo
        with open(transaction_file, "w", encoding="utf-8", newline="") as file:
            json.dump(data_list, file, indent=2)

    @freeze_time("03/03/2025")
    def test_not_valid_tc16(self):
        """test no valido 16"""
        # llamamos a nuestra función con un iban que en el fichero no hay
        my_manager = AccountManager()
        iban_number = "ES9121000418450200051332"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.calculate_balance(iban_number)
        self.assertEqual(cm.exception.message, "El IBAN introducido no está "
                                               "en el fichero")

    @freeze_time("03/03/2025")
    def test_not_valid_tc18(self):
        """test no valido 18"""
        # leemos el fichero balances.json y lo guardamos en una variable
        transaction_file = JSON_FILES_PATH + "RF3/balances.json"
        with open(transaction_file, "r", encoding="utf-8",
                  newline="") as file:
            data_list = json.load(file)

        # escribimos en el fichero los datos de la prueba
        file_content = ']'
        with open(transaction_file, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        # llamamos a nuestra función
        my_manager = AccountManager()
        iban_number = "ES8658342044541216872704"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.calculate_balance(iban_number)
        self.assertEqual(cm.exception.message, "ERROR: JSON decode error")

        # devolvemos el contenido original del archivo
        with open(transaction_file, "w", encoding="utf-8", newline="") as file:
            json.dump(data_list, file, indent=2)

    @freeze_time("03/03/2025")
    def test_valid_tc19(self):
        """test valido 19"""
        # leemos el fichero balances.json y lo guardamos en una variable
        transaction_file = JSON_FILES_PATH + "RF3/balances.json"
        with open(transaction_file, "r", encoding="utf-8",
                  newline="") as file:
            data_list = json.load(file)

        # eliminamos balances.json
        os.remove(transaction_file)

        # llamamos a nuestra función
        my_manager = AccountManager()
        iban_number = "ES8658342044541216872704"
        result = my_manager.calculate_balance(iban_number)
        self.assertEqual(result, True)

        # al contenido original añadimos lo nuevo
        with open(transaction_file, "r", encoding="utf-8",
                  newline="") as file:
            new_list = json.load(file)
        data_list.append(new_list[0])

        # devolvemos el contenido original del archivo
        with open(transaction_file, "w", encoding="utf-8", newline="") as file:
            json.dump(data_list, file, indent=2)


    @freeze_time("03/03/2025")
    def test_valid_tc20(self):
        """test valido 20"""
        # leemos el fichero y lo guardamos en una variable
        transaction_file = JSON_FILES_PATH + "RF3/transactions.json"
        with open(transaction_file, "r", encoding="utf-8",
                  newline="") as file:
            data_list = json.load(file)

        # escribimos en el fichero los datos de la prueba
        file_content = [
            {
                "IBAN": "ES8658342044541216872704",
                "amount": "-1280.06"
            },
            {
                "IBAN": "ES7156958200176924034556",
                "amount": "+4321.12"
            }
        ]
        with open(transaction_file, "w", encoding="utf-8", newline="") as file:
            json.dump(file_content, file, indent=2)

        # llamamos a nuestra función
        my_manager = AccountManager()
        iban_number = "ES7156958200176924034556"
        result = my_manager.calculate_balance(iban_number)
        self.assertEqual(result, True)

        # devolvemos el contenido original del archivo
        with open(transaction_file, "w", encoding="utf-8", newline="") as file:
            json.dump(data_list, file, indent=2)

    @freeze_time("03/03/2025")
    def test_valid_tc21(self):
        """test valido 21"""
        # leemos el fichero y lo guardamos en una variable
        transaction_file = JSON_FILES_PATH + "RF3/transactions.json"
        with open(transaction_file, "r", encoding="utf-8",
                  newline="") as file:
            data_list = json.load(file)

        # escribimos en el fichero los datos de la prueba
        file_content = []
        for _ in range(99):
            amount = round(random.uniform(-5000,5000), 2)
            transaction = {
                    "IBAN": "ES8658342044541216872704",
                    "amount": amount
                        }
            file_content.append(transaction)

        with open(transaction_file, "w", encoding="utf-8", newline="") as file:
            json.dump(file_content, file, indent=2)

        # llamamos a nuestra función
        my_manager = AccountManager()
        iban_number = "ES8658342044541216872704"
        result = my_manager.calculate_balance(iban_number)
        self.assertEqual(result, True)

        # devolvemos el contenido original del archivo
        with open(transaction_file, "w", encoding="utf-8", newline="") as file:
            json.dump(data_list, file, indent=2)

    @freeze_time("03/03/2025")
    def test_valid_tc22(self):
        """test valido 22"""
        # leemos el fichero y lo guardamos en una variable
        transaction_file = JSON_FILES_PATH + "RF3/transactions.json"
        with open(transaction_file, "r", encoding="utf-8",
                  newline="") as file:
            data_list = json.load(file)

        # escribimos en el fichero los datos de la prueba
        file_content = []
        for _ in range(100):
            amount = round(random.uniform(-5000, 5000), 2)
            transaction = {
                "IBAN": "ES8658342044541216872704",
                "amount": amount
            }
            file_content.append(transaction)

        with open(transaction_file, "w", encoding="utf-8", newline="") as file:
            json.dump(file_content, file, indent=2)

        # llamamos a nuestra función
        my_manager = AccountManager()
        iban_number = "ES8658342044541216872704"
        result = my_manager.calculate_balance(iban_number)
        self.assertEqual(result, True)

        # devolvemos el contenido original del archivo
        with open(transaction_file, "w", encoding="utf-8", newline="") as file:
            json.dump(data_list, file, indent=2)

if __name__ == '__main__':
    unittest.main()
