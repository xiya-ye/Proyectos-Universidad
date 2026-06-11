"""clase para almacenes de account deposit"""
from uc3m_money.storage.json_store import JsonStore
from uc3m_money.config.account_management_config import DEPOSITS_STORE_FILE

class DepositsJsonStore:
    """DepositsJsonStore class"""
    # pylint: disable=invalid-name
    class __DepositsJsonStore(JsonStore):
        """clase privada de DepositJsonStore"""
        _file_path = DEPOSITS_STORE_FILE

    instance = None

    def __new__(cls):
        if not DepositsJsonStore.instance:
            DepositsJsonStore.instance = (
                DepositsJsonStore.__DepositsJsonStore())
        return DepositsJsonStore.instance

    def __getattr__(self, item):
        return getattr(DepositsJsonStore.instance, item)

    def __setattr__(self, key, value):
        return setattr(DepositsJsonStore.instance, key, value)
