"""clase padre para validar atributos"""
import re
from uc3m_money.exception.account_management_exception import AccountManagementException

# pylint: disable=too-few-public-methods
class Attribute:
    """clase padre para validar atributos"""
    def __init__(self):
        self._attr_value = ""
        self._error_message = ""
        self._validation_pattern = r""

    def _validate(self, value):
        regex = re.compile(self._validation_pattern)
        regex_matches = regex.fullmatch(value)
        if not regex_matches:
            raise AccountManagementException(self._error_message)
        return value

    @property
    def value(self):
        """clase para value"""
        return self._attr_value

    @value.setter
    def value(self, attr_value):
        self._attr_value = attr_value
