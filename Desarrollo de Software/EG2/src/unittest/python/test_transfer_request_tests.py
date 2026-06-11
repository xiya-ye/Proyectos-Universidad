"""class for testing the transfer_request method"""
import json
import os
import unittest
from uc3m_money import AccountManager, AccountManagementException, JSON_FILES_PATH
from freezegun import freeze_time


class TransferRequest(unittest.TestCase):
    """class for testing the register_order method"""

    # pylint: disable=too-many-public-methods
    def tearDown(self):
        """eliminar archivo json después de cada test"""
        print("tearDown")
        transfer_request_file = JSON_FILES_PATH + "RF1/transfer_requests.json"
        if os.path.isfile(transfer_request_file):
            os.remove(transfer_request_file)
            print(" removed transfer-request_file")


    def setUp(self):
        """eliminar archivo json antes de cada test"""
        print("setUp")
        transfer_request_file = JSON_FILES_PATH + "RF1/transfer_requests.json"
        if os.path.isfile(transfer_request_file):
            os.remove(transfer_request_file)
            print(" removed transfer-request_file")


    @freeze_time("01/01/2025 12:00:00")
    def test_valid_tc1(self):
        """test valido 1"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "text valid"
        amount_to_test = 10.00
        date_to_test = "01/01/2025"
        type_to_test = "ORDINARY"

        result = my_manager.transfer_request(from_iban=from_iban_to_test,
                                             to_iban=to_iban_to_test,
                                             concept=concept_to_test,
                                             amount=amount_to_test,
                                             date=date_to_test,
                                             type_=type_to_test)
        self.assertEqual(result, "f4bb226d359c7bf295860cf8553172a7")
        transfer_request_file = JSON_FILES_PATH + "RF1/transfer_requests.json"
        with (open(transfer_request_file, "r", encoding="utf-8", newline="")
              as file):
            data_list = json.load(file)
        found = False
        for item in data_list:
            if item['transfer_code'] == result:
                found = True
        self.assertTrue(found)


    @freeze_time("03/03/2025")
    def test_valid_tc2(self):
        """test valido 2"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "concept val"
        amount_to_test = 10.01
        date_to_test = "02/02/2026"
        type_to_test = "URGENT"
        result = my_manager.transfer_request(from_iban=from_iban_to_test,
                                             to_iban=to_iban_to_test,
                                             concept=concept_to_test,
                                             amount=amount_to_test,
                                             date=date_to_test,
                                             type_=type_to_test)
        self.assertEqual(result, "837de840d31c16b9e49689e9d9e8dabc")
        transfer_request_file = JSON_FILES_PATH + "RF1/transfer_requests.json"
        with (open(transfer_request_file, "r", encoding="utf-8", newline="")
              as file):
            data_list = json.load(file)
        found = False
        for item in data_list:
            if item['transfer_code'] == result:
                found = True
        self.assertTrue(found)


    @freeze_time("03/03/2025")
    def test_valid_tc3(self):
        """test valido 3"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "transferencia inmediata viaje"
        amount_to_test = 9999.99
        date_to_test = "30/11/2049"
        type_to_test = "INMEDIATE"
        result = my_manager.transfer_request(from_iban=from_iban_to_test,
                                             to_iban=to_iban_to_test,
                                             concept=concept_to_test,
                                             amount=amount_to_test,
                                             date=date_to_test,
                                             type_=type_to_test)
        self.assertEqual(result, "65e1968e6e01ae409fc3686c5f452c8a")
        transfer_request_file = JSON_FILES_PATH + "RF1/transfer_requests.json"
        with (open(transfer_request_file, "r", encoding="utf-8", newline="")
              as file):
            data_list = json.load(file)
        found = False
        for item in data_list:
            if item['transfer_code'] == result:
                found = True
        self.assertTrue(found)


    @freeze_time("03/03/2025")
    def test_valid_tc4(self):
        """test valido 4"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "trasnfere compra semana pasada"
        amount_to_test = 10000.00
        date_to_test = "31/12/2050"
        type_to_test = "ORDINARY"
        result = my_manager.transfer_request(from_iban=from_iban_to_test,
                                             to_iban=to_iban_to_test,
                                             concept=concept_to_test,
                                             amount=amount_to_test,
                                             date=date_to_test,
                                             type_=type_to_test)
        self.assertEqual(result, "8a638c613358ea3b8434b42f54fa6a0b")
        transfer_request_file = JSON_FILES_PATH + "RF1/transfer_requests.json"
        with (open(transfer_request_file, "r", encoding="utf-8", newline="")
              as file):
            data_list = json.load(file)
        found = False
        for item in data_list:
            if item['transfer_code'] == result:
                found = True
        self.assertTrue(found)


    @freeze_time("03/03/2025")
    def test_valid_tc5(self):
        """test valido 5"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "transferencia bancaria"
        amount_to_test = 123.3
        date_to_test = "02/02/2026"
        type_to_test = "ORDINARY"
        result = my_manager.transfer_request(from_iban=from_iban_to_test,
                                             to_iban=to_iban_to_test,
                                             concept=concept_to_test,
                                             amount=amount_to_test,
                                             date=date_to_test,
                                             type_=type_to_test)
        self.assertEqual(result, "c2edbb4cf1aa3ecf7d3ccbabdeb21e12")
        transfer_request_file = JSON_FILES_PATH + "RF1/transfer_requests.json"
        with (open(transfer_request_file, "r", encoding="utf-8", newline="")
              as file):
            data_list = json.load(file)
        found = False
        for item in data_list:
            if item['transfer_code'] == result:
                found = True
        self.assertTrue(found)


    def test_not_valid_tc6( self ):
        """test no valido 6"""
        my_manager = AccountManager()
        from_iban_to_test = 123
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "transferencia bancaria"
        type_to_test = "ORDINARY"
        date_to_test = "02/02/2026"
        amount_to_test = 123.3
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: from_iban not valid")


    def test_not_valid_tc7( self ):
        """test no valido 7"""
        my_manager = AccountManager()
        from_iban_to_test = "ES3400491500950012345679"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "transferencia bancaria"
        type_to_test = "ORDINARY"
        date_to_test = "02/02/2026"
        amount_to_test = 123.3
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: from_iban not valid")


    def test_not_valid_tc8( self ):
        """test no valido 8"""
        my_manager = AccountManager()
        from_iban_to_test = "S34004915009500123456780"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "transferencia bancaria"
        type_to_test = "ORDINARY"
        date_to_test = "02/02/2026"
        amount_to_test = 123.3
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: from_iban not valid")


    def test_not_valid_tc9( self ):
        """test no valido 9"""
        my_manager = AccountManager()
        from_iban_to_test = "ES23456789UYTR3456789654"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "transferencia bancaria"
        type_to_test = "ORDINARY"
        date_to_test = "02/02/2026"
        amount_to_test = 123.3
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: from_iban not valid")


    def test_not_valid_tc10( self ):
        """test no valido 10"""
        my_manager = AccountManager()
        from_iban_to_test = "ES123456789123456789112"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "transferencia bancaria"
        type_to_test = "ORDINARY"
        date_to_test = "02/02/2026"
        amount_to_test = 123.3
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: from_iban not valid")


    def test_not_valid_tc11( self ):
        """test no valido 11"""
        my_manager = AccountManager()
        from_iban_to_test = "ES34004915009500123456791"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "transferencia bancaria"
        type_to_test = "ORDINARY"
        date_to_test = "02/02/2026"
        amount_to_test = 123.3
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: from_iban not valid")


    def test_not_valid_tc12( self ):
        """test no valido 12"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = 123
        concept_to_test = "transferencia bancaria"
        type_to_test = "ORDINARY"
        date_to_test = "02/02/2026"
        amount_to_test = 123.3
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: to_iban not valid")


    def test_not_valid_tc13( self ):
        """test no valido 13"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES3400491500950012345679"
        concept_to_test = "transferencia bancaria"
        type_to_test = "ORDINARY"
        date_to_test = "02/02/2026"
        amount_to_test = 123.3
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: to_iban not valid")


    def test_not_valid_tc14( self ):
        """test no valido 14"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "S34004915009500123456780"
        concept_to_test = "transferencia bancaria"
        type_to_test = "ORDINARY"
        date_to_test = "02/02/2026"
        amount_to_test = 123.3
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: to_iban not valid")


    def test_not_valid_tc15( self ):
        """test no valido 15"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES23456789UYTR3456789654"
        concept_to_test = "transferencia bancaria"
        type_to_test = "ORDINARY"
        date_to_test = "02/02/2026"
        amount_to_test = 123.3
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: to_iban not valid")


    def test_not_valid_tc16( self ):
        """test no valido 16"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES123456789123456789112"
        concept_to_test = "tranferencia bancaria"
        type_to_test = "ORDINARY"
        date_to_test = "02/02/2026"
        amount_to_test = 123.3
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: to_iban not valid")


    def test_not_valid_tc17( self ):
        """test no valido 17"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES34004915009500123456791"
        concept_to_test = "tranferencia bancaria"
        type_to_test = "ORDINARY"
        date_to_test = "02/02/2026"
        amount_to_test = 123.3
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: to_iban not valid")


    def test_not_valid_tc18( self ):
        """test no valido 18"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = 123
        type_to_test = "ORDINARY"
        date_to_test = "02/02/2026"
        amount_to_test = 123.3
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: concept not valid")


    def test_not_valid_tc19(self):
        """test no valido 19"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "Pay Final"
        amount_to_test = 123.3
        date_to_test = "02/02/2026"
        type_to_test = "ORDINARY"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: concept not valid")


    def test_not_valid_tc20(self):
        """test no valido 20"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "transferencia para javier perez"
        amount_to_test = 123.3
        date_to_test = "02/02/2026"
        type_to_test = "ORDINARY"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: concept not valid")


    def test_not_valid_tc21(self):
        """test no valido 20"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "pagoparapablo"
        amount_to_test = 123.3
        date_to_test = "02/02/2026"
        type_to_test = "ORDINARY"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: concept not valid")


    def test_not_valid_tc22(self):
        """test no valido 22"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "Pago para **pablo"
        amount_to_test = 123.3
        date_to_test = "02/02/2026"
        type_to_test = "ORDINARY"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: concept not valid")


    def test_not_valid_tc23(self):
        """test no valido 23"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "transferencia bancaria"
        amount_to_test = 123.3
        date_to_test = "02/02/2026"
        type_to_test = 123
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: type not valid")


    def test_not_valid_tc24(self):
        """test no valido 24"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "transferencia bancaria"
        amount_to_test = 123.3
        date_to_test = "02/02/2026"
        type_to_test = "OTHER"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: type not valid")


    def test_not_valid_tc25(self):
        """test no valido 25"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "transferencia bancaria"
        amount_to_test = 123.3
        date_to_test = 123
        type_to_test = "ORDINARY"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: date not valid")


    def test_not_valid_tc26(self):
        """test no valido 26"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "transferencia bancaria"
        amount_to_test = 123.3
        date_to_test = "02.02.2026"
        type_to_test = "ORDINARY"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: date not valid")


    def test_not_valid_tc27(self):
        """test no valido 27"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "transferencia bancaria"
        amount_to_test = 123.3
        date_to_test = "00/02/2026"
        type_to_test = "ORDINARY"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: date not valid")


    def test_not_valid_tc28(self):
        """test no valido 28"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "transferencia bancaria"
        amount_to_test = 123.3
        date_to_test = "32/02/2026"
        type_to_test = "ORDINARY"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: date not valid")


    def test_not_valid_tc29(self):
        """test no valido 29"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "transferencia bancaria"
        amount_to_test = 123.3
        date_to_test = "02/00/2026"
        type_to_test = "ORDINARY"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: date not valid")


    def test_not_valid_tc30(self):
        """test no valido 30"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "transferencia bancaria"
        amount_to_test = 123.3
        date_to_test = "02/13/2026"
        type_to_test = "ORDINARY"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: date not valid")


    def test_not_valid_tc31(self):
        """test no valido 31"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "transferencia bancaria"
        amount_to_test = 123.3
        date_to_test = "02/02/2024"
        type_to_test = "ORDINARY"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: date not valid")


    def test_not_valid_tc32(self):
        """test no valido 32"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "transferencia bancaria"
        amount_to_test = 123.3
        date_to_test = "32/02/2026"
        type_to_test = "ORDINARY"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: date not valid")


    def test_not_valid_tc33(self):
        """test no valido 33"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "transferencia bancaria"
        amount_to_test = "num"
        date_to_test = "02/02/2026"
        type_to_test = "ORDINARY"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: amount not valid")


    def test_not_valid_tc34(self):
        """test no valido 34"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "transferencia bancaria"
        amount_to_test = 9.99
        date_to_test = "02/02/2026"
        type_to_test = "ORDINARY"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: amount not valid")


    def test_not_valid_tc35(self):
        """test no valido 35"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "transferencia bancaria"
        amount_to_test = 10000.01
        date_to_test = "02/02/2026"
        type_to_test = "ORDINARY"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: amount not valid")


    def test_not_valid_tc36(self):
        """test no valido 36"""
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "transferencia bancaria"
        amount_to_test = 123.123
        date_to_test = "02/02/2026"
        type_to_test = "ORDINARY"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message, "ERROR: amount not valid")


    @freeze_time("01/01/2025 12:00:00")
    def test_not_valid_tc37(self):
        """test no valido 37"""
        # creamos primero una transferencia
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "text valid"
        amount_to_test = 10.00
        date_to_test = "01/01/2025"
        type_to_test = "ORDINARY"

        result = my_manager.transfer_request(from_iban=from_iban_to_test,
                                             to_iban=to_iban_to_test,
                                             concept=concept_to_test,
                                             amount=amount_to_test,
                                             date=date_to_test,
                                             type_=type_to_test)
        self.assertEqual(result, "f4bb226d359c7bf295860cf8553172a7")
        # comprobamos que se ha hecho bien la transferencia
        transfer_request_file = JSON_FILES_PATH + "RF1/transfer_requests.json"
        with (open(transfer_request_file, "r", encoding="utf-8", newline="")
              as file):
            data_list = json.load(file)
        found = False
        for item in data_list:
            if item['transfer_code'] == result:
                found = True
        self.assertTrue(found)

        # intentamos insertar otra transferencia con los mismos datos
        my_manager = AccountManager()
        from_iban_to_test = "ES9121000418450200051332"
        to_iban_to_test = "ES6812345678061234567890"
        concept_to_test = "text valid"
        amount_to_test = 10.00
        date_to_test = "01/01/2025"
        type_to_test = "ORDINARY"
        with self.assertRaises(AccountManagementException) as cm:
            my_manager.transfer_request(from_iban=from_iban_to_test,
                                        to_iban=to_iban_to_test,
                                        concept=concept_to_test,
                                        type_=type_to_test,
                                        date=date_to_test,
                                        amount=amount_to_test)
        self.assertEqual(cm.exception.message,
                         "ERROR: transfer request with same data already exists")

if __name__ == '__main__':
    unittest.main()
