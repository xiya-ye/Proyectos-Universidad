"""clase para validar amount para deposit_into_account"""
from uc3m_money.attributes.attribute import Attribute
from uc3m_money.exception.account_management_exception import AccountManagementException

# pylint: disable=too-few-public-methods
class AmountDeposit(Attribute):
    """Class for providing the methods for the validation of amount"""
    def __init__(self, attr_value):
        super().__init__()
        self._error_message = "Error - Invalid deposit amount"
        self._validation_pattern = r"^EUR [0-9]{4}\.[0-9]{2}"
        self._attr_value = self._validate(attr_value)

    def _validate(self, value):
        """valida amount para el metodo deposit into account"""
        super()._validate(value)
        deposit_amount_float = float(value[4:])
        if deposit_amount_float == 0:
            raise AccountManagementException("Error - Deposit must be greater than 0")
        return deposit_amount_float
