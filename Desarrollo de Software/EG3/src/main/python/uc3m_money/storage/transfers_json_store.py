"""DepositsJsonStore class"""
from uc3m_money.storage.json_store import JsonStore
from uc3m_money.config.account_management_config import TRANSFERS_STORE_FILE
from uc3m_money.exception.account_management_exception import AccountManagementException

class TransfersJsonStore:
    """Clase TraansferJsonStore"""
    # pylint: disable = invalid-name
    class __TransfersJsonStore(JsonStore):
        """clase TransfersJsonStore"""

        _file_path = TRANSFERS_STORE_FILE
        _data_list = []

        def find_transfer_request(self, my_request):
            """find request in trasnfer list"""
            for transfer_item in self._data_list:
                # pylint: disable=too-many-boolean-expressions
                if transfer_item == my_request:
                    raise AccountManagementException(
                        "Duplicated transfer in transfer list")

            super().add_item_to_store(my_request)

    instance = None

    def __new__(cls):
        if not TransfersJsonStore.instance:
            TransfersJsonStore.instance = TransfersJsonStore.__TransfersJsonStore()
        return TransfersJsonStore.instance

    def __getattr__(self, item):
        return getattr(TransfersJsonStore.instance, item)

    def __setattr__(self, key, value):
        return setattr(TransfersJsonStore.instance, key, value)
