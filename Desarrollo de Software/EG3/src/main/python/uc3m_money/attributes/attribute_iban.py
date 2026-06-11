"""clase para validar iban"""

from uc3m_money.attributes.attribute import Attribute
from uc3m_money.exception.account_management_exception import AccountManagementException

class Iban(Attribute):
    """Class for providing the methods for the validation of iban"""
    # pylint: disable=too-few-public-methods

    def __init__(self, attr_value):
        super().__init__()
        self._error_message = "Invalid IBAN format"
        self._validation_pattern = r"^ES[0-9]{22}"
        self._attr_value = self._validate(attr_value)

    def _validate(self, value) -> str:
        """method for validating the iban"""
        super()._validate(value)

        iban = value
        original_code = iban[2:4]
        # replacing the control
        iban = iban[:2] + "00" + iban[4:]
        iban = iban[4:] + iban[:4]

        # Convertir el IBAN en una cadena numérica, reemplazando letras por números
        iban = (iban.replace('A', '10').replace('B', '11').
                replace('C', '12').replace('D', '13').replace('E', '14').
                replace('F', '15'))
        iban = (iban.replace('G', '16').replace('H', '17').
                replace('I', '18').replace('J', '19').replace('K', '20').
                replace('L', '21'))
        iban = (iban.replace('M', '22').replace('N', '23').
                replace('O', '24').replace('P', '25').replace('Q', '26').
                replace('R', '27'))
        iban = (iban.replace('S', '28').replace('T', '29').replace('U', '30').
                replace('V', '31').replace('W', '32').replace('X', '33'))
        iban = iban.replace('Y', '34').replace('Z', '35')

        # Mover los cuatro primeros caracteres al final

        # Convertir la cadena en un número entero
        int_iban = int(iban)

        # Calcular el módulo 97
        modulo_iban = int_iban % 97

        # Calcular el dígito de control (97 menos el módulo)
        digito_control = 98 - modulo_iban

        if int(original_code) != digito_control:
            # print(digito_control)
            raise AccountManagementException("Invalid IBAN control digit")

        return value
