"""clase para validar type para transfer_request"""
from uc3m_money.attributes.attribute import Attribute

# pylint: disable=too-few-public-methods
class Type(Attribute):
    """Class for providing the methods for the validation of type"""
    def __init__(self, attr_value):
        super().__init__()
        self._error_message = "Invalid transfer type"
        self._validation_pattern = r"(ORDINARY|INMEDIATE|URGENT)"
        self._attr_value = self._validate(attr_value)
