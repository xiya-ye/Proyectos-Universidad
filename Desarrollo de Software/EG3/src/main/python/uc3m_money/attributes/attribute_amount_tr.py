"""clase hija para validar concept"""
from uc3m_money.attributes.attribute import Attribute
from uc3m_money.exception.account_management_exception import AccountManagementException

# pylint: disable=too-few-public-methods
class AmountTR(Attribute):
    """Class for providing the methods for the validation of concept"""
    def __init__(self, attr_value):
        super().__init__()
        self._error_message = "Invalid concept format"
        self._attr_value = self._validate(attr_value)

    def _validate(self, value):
        """valida amount para el metodo transfer request"""
        try:
            float_amount = float(value)
        except ValueError as exc:
            raise AccountManagementException(
                "Invalid transfer amount") from exc
        number_to_string = str(float_amount)
        if '.' in number_to_string:
            decimales = len(number_to_string.split('.')[1])
            if decimales > 2:
                raise AccountManagementException("Invalid transfer amount")
        if float_amount < 10 or float_amount > 10000:
            raise AccountManagementException("Invalid transfer amount")
        return value
