"""class for testing the deposit_into_account method"""
import json
import os
import unittest
from uc3m_money import AccountManager, AccountManagementException, JSON_FILES_PATH
from freezegun import freeze_time


# pylint: disable=too-many-public-methods
class MyTestCase(unittest.TestCase):
    """class for testing the deposit_into_account method"""

    @freeze_time("03/03/2025")
    def test_valid_tc1(self):
        """test valido 1"""
        file_content = {"IBAN": "ES9121000418450200051332", "AMOUNT":"EUR 1000.25"}

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            json.dump(file_content, file, indent=2)

        my_manager = AccountManager()

        result = my_manager.deposit_into_account(prueba)
        self.assertEqual(result,
                         "b104a49ed37ac5cad72fb50d8149b7ee1e947d67a4d1e7faab8f5dcc385881be")

        # comprobamos que se ha creado correctamente
        transfer_request_file = JSON_FILES_PATH + "RF2/deposit_requests.json"
        with (open(transfer_request_file, "r", encoding="utf-8", newline="")
              as file):
            data_list = json.load(file)
        found = False
        for item in data_list:
            if item['deposit_signature'] == result:
                found = True
        self.assertTrue(found)

        # eliminamos el archivo creado para la prueba
        os.remove(prueba)

    def test_not_valid_tc2(self):
        """test no valido 2"""
        my_manager = AccountManager()
        file_content = ''

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: input JSON decode error, "
                         "wrong input JSON format")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)

    def test_not_valid_tc3(self):
        """test no valido 3"""
        my_manager = AccountManager()
        file_content = ('{"IBAN": "ES9121000418450200051332", "AMOUNT":"EUR 1000.25"}'
                        '{"IBAN": "ES9121000418450200051332", "AMOUNT":"EUR 1000.25"}')

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: input JSON decode error, "
                         "wrong input JSON format")
       # eliminamos el archivo creado para la prueba
        os.remove(prueba)

    def test_not_valid_tc4(self):
        """test no valido 4"""
        my_manager = AccountManager()
        file_content = '"IBAN": "ES9121000418450200051332", "AMOUNT":"EUR 1000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: input JSON decode error, "
                         "wrong input JSON format")
       # eliminamos el archivo creado para la prueba
        os.remove(prueba)

    def test_not_valid_tc5(self):
        """test no valido 5"""
        my_manager = AccountManager()
        file_content = '{{"IBAN": "ES9121000418450200051332", "AMOUNT":"EUR 1000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: input JSON decode error, "
                         "wrong input JSON format")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc6(self):
        """test no valido 6"""
        my_manager = AccountManager()
        file_content = '{, "AMOUNT":"EUR 1000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: input JSON decode error, "
                         "wrong input JSON format")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc7(self):
        """test no valido 7"""
        my_manager = AccountManager()
        file_content = ('{"IBAN": "ES9121000418450200051332""IBAN": '
                        '"ES9121000418450200051332", "AMOUNT":"EUR 1000.25"}')

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: input JSON decode error, "
                         "wrong input JSON format")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc8(self):
        """test no valido 8"""
        my_manager = AccountManager()
        file_content = ('{"IBAN": "ES9121000418450200051332""IBAN": '
                        '"ES9121000418450200051332", "AMOUNT":"EUR 1000.25"}')

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: input JSON decode error, "
                         "wrong input JSON format")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)



    def test_not_valid_tc9(self):
        """test no valido 9"""
        my_manager = AccountManager()
        file_content = '{""IBAN": "ES9121000418450200051332", "AMOUNT":"EUR 1000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: input JSON decode error, "
                         "wrong input JSON format")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc10(self):
        """test no valido 10"""
        my_manager = AccountManager()
        file_content = '{"": "ES9121000418450200051332", "AMOUNT":"EUR 1000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: field is missing")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc11(self):
        """test no valido 11"""
        my_manager = AccountManager()
        file_content = '{"IBANIBAN": "ES9121000418450200051332", "AMOUNT":"EUR 1000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: field is missing")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)

    def test_not_valid_tc12(self):
        """test no valido 12"""
        my_manager = AccountManager()
        file_content = '{"IBAN" "ES9121000418450200051332", "AMOUNT":"EUR 1000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: input JSON decode error, "
                         "wrong input JSON format")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc13(self):
        """test no valido 13"""
        my_manager = AccountManager()
        file_content = '{"IBAN":: "ES9121000418450200051332", "AMOUNT":"EUR 1000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: input JSON decode error, "
                         "wrong input JSON format")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc14(self):
        """test no valido 14"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "", "AMOUNT":"EUR 1000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: IBAN not valid")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc15(self):
        """test no valido 15"""
        my_manager = AccountManager()
        file_content = ('{"IBAN": "ES9121000418450200051332ES9121000418450200051332",'
                        ' "AMOUNT":"EUR 1000.25"}')

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: IBAN not valid")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc16(self):
        """test no valido 16"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "ES", "AMOUNT":"EUR 1000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: IBAN not valid")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)



    def test_not_valid_tc17(self):
        """test no valido 17"""
        my_manager = AccountManager()
        file_content = ('{"IBAN": "ES91210004184502000513329121000418450200051332", '
                        '"AMOUNT":"EUR 1000.25"}')

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: IBAN not valid")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc18(self):
        """test no valido 18"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "ES9121000418450200051332" "AMOUNT":"EUR 1000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: input JSON decode error, "
                         "wrong input JSON format")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc19(self):
        """test no valido 19"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "ES9121000418450200051332",, "AMOUNT":"EUR 1000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: input JSON decode error, "
                         "wrong input JSON format")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc20(self):
        """test no valido 20"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "ES9121000418450200051332", }'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: input JSON decode error, "
                         "wrong input JSON format")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc21(self):
        """test no valido 21"""
        my_manager = AccountManager()
        file_content = ('{"IBAN": "ES9121000418450200051332", "AMOUNT":"EUR 1000.25"'
                        ' "AMOUNT":"EUR 1000.25"}')

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: input JSON decode error, "
                         "wrong input JSON format")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc22(self):
        """test no valido 22"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "ES9121000418450200051332", "":"EUR 1000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: field is missing")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)



    def test_not_valid_tc23(self):
        """test no valido 23"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "ES9121000418450200051332", "AMOUNTAMOUNT":"EUR 1000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: field is missing")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)



    def test_not_valid_tc24(self):
        """test no valido 24"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "ES9121000418450200051332", "AMOUNT":"1000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: Amount not valid")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)



    def test_not_valid_tc25(self):
        """test no valido 25"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "ES9121000418450200051332", "AMOUNT":"EUR EUR 1000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: Amount not valid")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc26(self):
        """test no valido 26"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "ES9121000418450200051332", "AMOUNT":"EUR "}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: Amount not valid")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc27(self):
        """test no valido 27"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "ES9121000418450200051332", "AMOUNT":"EUR 1000.251000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: Amount not valid")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc28(self):
        """test no valido 28"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "ES9121000418450200051332", "AMOUNT":"EUR .25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: Amount not valid")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc29(self):
        """test no valido 29"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "ES9121000418450200051332", "AMOUNT":"EUR 10001000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: Amount not valid")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)



    def test_not_valid_tc30(self):
        """test no valido 30"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "ES9121000418450200051332", "AMOUNT":"EUR 100025"} '

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: Amount not valid")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc31(self):
        """test no valido 31"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "ES9121000418450200051332", "AMOUNT":"EUR 1000..25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: Amount not valid")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc32(self):
        """test no valido 32"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "ES9121000418450200051332", "AMOUNT":"EUR 1000."}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: Amount not valid")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc33(self):
        """test no valido 33"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "ES9121000418450200051332", "AMOUNT":"EUR 1000.2525"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: Amount not valid")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc34(self):
        """test no valido 34"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "ES9121000418450200051332", "AMOUNT":"EUR 1000.25"'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: input JSON decode error, "
                         "wrong input JSON format")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc35(self):
        """test no valido 35"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "ES9121000418450200051332", "AMOUNT":"EUR 1000.25"}}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: input JSON decode error, "
                         "wrong input JSON format")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)



    def test_not_valid_tc36(self):
        """test no valido 36"""
        my_manager = AccountManager()
        file_content = 'X"IBAN": "ES9121000418450200051332", "AMOUNT":"EUR 1000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: input JSON decode error, "
                         "wrong input JSON format")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc37(self):
        """test no valido 37"""
        my_manager = AccountManager()
        file_content = '{XIBAN": "ES9121000418450200051332", "AMOUNT":"EUR 1000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: input JSON decode error, "
                         "wrong input JSON format")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc38(self):
        """test no valido 38"""
        my_manager = AccountManager()
        file_content = '{"IBXN":"ES9121000418450200051332", "AMOUNT":"EUR 1000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: field is missing")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc39(self):
        """test no valido 39"""
        my_manager = AccountManager()
        file_content = '{"IBAN"X"ES9121000418450200051332", "AMOUNT":"EUR 1000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: input JSON decode error, "
                         "wrong input JSON format")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc40(self):
        """test no valido 40"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "EX9121000418450200051332", "AMOUNT":"EUR 1000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: IBAN not valid")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc41(self):
        """test no valido 41"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "ES91210X0418450200051332", "AMOUNT":"EUR 1000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: IBAN not valid")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)



    def test_not_valid_tc42(self):
        """test no valido 42"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "ES9121000418450200051332"X"AMOUNT""EUR 1000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: input JSON decode error, "
                         "wrong input JSON format")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)



    def test_not_valid_tc43(self):
        """test no valido 43"""
        my_manager = AccountManager()
        file_content = '{"IBAN":"ES9121000418450200051332", "AMXUNT":"EUR 1000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: field is missing")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc44(self):
        """test no valido 44"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "ES9121000418450200051332", "AMOUNT":"EXR 1000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: Amount not valid")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)



    def test_not_valid_tc45(self):
        """test no valido 45"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "ES9121000418450200051332", "AMOUNT":"EUR X000.25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: Amount not valid")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)



    def test_not_valid_tc46(self):
        """test no valido 46"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "ES9121000418450200051332", "AMOUNT":"EUR 1000X25"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: Amount not valid")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc47(self):
        """test no valido 47"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "ES9121000418450200051332", "AMOUNT":"EUR 1000.X5"}'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: Amount not valid")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)


    def test_not_valid_tc48(self):
        """test no valido 48"""
        my_manager = AccountManager()
        file_content = '{"IBAN": "ES9121000418450200051332", "AMOUNT":"EUR 1000.25"X'

        # creamos un archivo json con los datos que queremos probar
        prueba = JSON_FILES_PATH + "RF2/test_deposit_into_account.json"
        with open(prueba, "w", encoding="utf-8", newline="") as file:
            file.write(file_content)

        with self.assertRaises(AccountManagementException) as cm:
            my_manager.deposit_into_account(prueba)
        self.assertEqual(cm.exception.message,
                         "ERROR: input JSON decode error, "
                         "wrong input JSON format")
        # eliminamos el archivo creado para la prueba
        os.remove(prueba)
