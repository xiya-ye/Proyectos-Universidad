"""MODULE: account_balance. Contains the account balance class"""
import json
from datetime import datetime, timezone
from uc3m_money.config.account_management_config import TRANSACTIONS_STORE_FILE
from uc3m_money.attributes.attribute_iban import Iban
from uc3m_money.exception.account_management_exception import AccountManagementException

class AccountBalance:
    """clase account_balance, para el calculo del saldo"""
    def __init__(self, iban):
        self._iban = Iban(iban).value
        self.__last_balance_time = datetime.timestamp(datetime.now(timezone.utc))
        self.__balance = self.calculate_iban_balance()

    def calculate_iban_balance(self):
        """metodo que calcula el saldo"""
        transactions_list = self.read_json_file(TRANSACTIONS_STORE_FILE)
        iban_found = False
        balance_sum = 0
        for transaction in transactions_list:
            # print(transaction["IBAN"] + " - " + iban)
            if transaction["IBAN"] == self._iban:
                balance_sum += float(transaction["amount"])
                iban_found = True
        if not iban_found:
            raise AccountManagementException("IBAN not found")
        return balance_sum

    def read_json_file(self, store_file):
        """loads the content a file and returns a list"""
        try:
            with open(store_file, "r", encoding="utf-8", newline="") as file:
                input_list = json.load(file)
        except FileNotFoundError as ex:
            raise AccountManagementException("Wrong file  or file path") from ex
        except json.JSONDecodeError as ex:
            raise AccountManagementException("JSON Decode Error - Wrong JSON Format") from ex
        return input_list

    def to_json(self):
        """returns the object information in json format"""
        return {"IBAN": self._iban,
                "time": self.__last_balance_time,
                "BALANCE": self.__balance}
