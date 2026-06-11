"""clase para validar date para transfer_request"""
from datetime import datetime, timezone
from uc3m_money.attributes.attribute import Attribute
from uc3m_money.exception.account_management_exception import AccountManagementException

class Date(Attribute):
    """Class for providing the methods for the validation of date"""
    # pylint: disable=too-few-public-methods

    def __init__(self, attr_value):
        super().__init__()
        self._error_message = "Invalid date format"
        self._validation_pattern = r"^(([0-2]\d|3[0-1])\/(0\d|1[0-2])\/\d\d\d\d)$"
        self._attr_value = self._validate(attr_value)

    def _validate(self, value):
        """validates the arrival date format using regex"""
        super()._validate(value)

        try:
            my_date = datetime.strptime(value, "%d/%m/%Y").date()
        except ValueError as ex:
            raise AccountManagementException("Invalid date format") from ex

        if my_date < datetime.now(timezone.utc).date():
            raise AccountManagementException("Transfer date must be today or later.")

        if my_date.year < 2025 or my_date.year > 2050:
            raise AccountManagementException("Invalid date format")
        return value
