"""clase JsonStore"""
import json
from uc3m_money.exception.account_management_exception import AccountManagementException

class JsonStore:
    """clase padre de json store"""
    _file_path = ""
    _data_list = []
    _error_message_json = "JSON Decode Error - Wrong JSON Format"
    _error_message_file = "Wrong file  or file path"

    def __init__(self):
        self.read_store()

    def read_store(self):
        """lee el fichero json_file_path"""
        try:
            with open(self._file_path, "r", encoding="utf-8",
                      newline="") as file:
                self._data_list = json.load(file)
        except FileNotFoundError:
            self._data_list = []
        except json.JSONDecodeError as ex:
            raise AccountManagementException(self._error_message_json) from ex
        return self._data_list

    def save_store(self):
        """guarda los datos de la data list en el fichero"""
        try:
            with open(self._file_path, "w", encoding="utf-8",
                      newline="") as file:
                json.dump(self._data_list, file, indent=2)
        except FileNotFoundError as ex:
            raise AccountManagementException(self._error_message_file) from ex
        except json.JSONDecodeError as ex:
            raise AccountManagementException(self._error_message_json) from ex

    def add_item_to_store(self, item_to_add):
        """añade un nuevo item a data list"""
        self.read_store()
        self._data_list.append(item_to_add)
        self.save_store()
