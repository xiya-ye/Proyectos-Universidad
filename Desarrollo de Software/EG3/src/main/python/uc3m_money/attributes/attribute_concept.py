"""clase hija para validar concept"""
from uc3m_money.attributes.attribute import Attribute

# pylint: disable=too-few-public-methods
class Concept(Attribute):
    """Class for providing the methods for the validation of concept"""
    def __init__(self, attr_value):
        super().__init__()
        self._error_message = "Invalid concept format"
        self._validation_pattern = r"^(?=^.{10,30}$)([a-zA-Z]+(\s[a-zA-Z]+)+)$"
        self._attr_value = self._validate(attr_value)
