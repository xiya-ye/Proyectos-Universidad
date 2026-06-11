"""Account manager module """
from uc3m_money.data.transfer_request import TransferRequest
from uc3m_money.data.account_deposit import AccountDeposit
from uc3m_money.data.account_balance import AccountBalance
from uc3m_money.storage.transfers_json_store import TransfersJsonStore
from uc3m_money.storage.deposits_json_store import DepositsJsonStore
from uc3m_money.storage.balance_json_store import BalancesJsonStore

class AccountManager:
    """Class for providing the methods for managing the orders"""

    # pylint: disable = invalid-name
    class __AccountManager:
        """Private class for providing the methods for managing the orders"""
        def __init__(self):
            pass

        #pylint: disable=too-many-positional-arguments
        #pylint: disable=too-many-arguments
        def transfer_request(self, from_iban: str,
                             to_iban: str,
                             concept: str,
                             transfer_type: str,
                             date: str,
                             amount: float)->str:
            """first method: receives transfer info and
            stores it into a file"""
            my_request = TransferRequest(from_iban=from_iban,
                                         to_iban=to_iban,
                                         transfer_concept=concept,
                                         transfer_type=transfer_type,
                                         transfer_date=date,
                                         transfer_amount=amount)
            transfer_list = TransfersJsonStore()
            transfer_list.find_transfer_request(my_request.to_json())
            return my_request.transfer_code


        def deposit_into_account(self, input_file:str)->str:
            """second method: manages the deposits received for accounts"""
            deposit_obj = AccountDeposit.load_deposit_from_file(input_file)
            deposit_store = DepositsJsonStore()
            deposit_store.add_item_to_store(deposit_obj.to_json())
            return deposit_obj.deposit_signature

        def calculate_balance(self, iban:str)->bool:
            """third method: calculate the balance for a given iban"""

            iban_balance = AccountBalance(iban=iban)
            balance_list = BalancesJsonStore()
            balance_list.add_item_to_store(iban_balance.to_json())
            return True

    instance = None

    def __new__(cls):
        if not AccountManager.instance:
            AccountManager.instance = AccountManager.__AccountManager()
        return AccountManager.instance

    def __getattr__(self, item):
        return getattr(AccountManager.instance, item)

    def __setattr__(self, key, value):
        return setattr(AccountManager.instance, key, value)
