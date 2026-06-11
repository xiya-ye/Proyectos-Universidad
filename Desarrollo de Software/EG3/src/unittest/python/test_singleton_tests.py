"""tests dde prueba"""
import unittest
from unittest import TestCase
from uc3m_money import AccountManager
from uc3m_money.storage.transfers_json_store import TransfersJsonStore
from uc3m_money.storage.deposits_json_store import DepositsJsonStore
from uc3m_money.storage.balance_json_store import BalancesJsonStore

class TestSingleton(TestCase):
    """clase para tests"""
    def test_account_manager_singleton(self):
        """primer test"""
        am1 = AccountManager()
        am2 = AccountManager()
        am3 = AccountManager()
        self.assertEqual(am1, am2)
        self.assertEqual(am2, am3)
        self.assertEqual(am1, am3)

    def test_transfer_singleton(self):
        tr1 = TransfersJsonStore()
        tr2 = TransfersJsonStore()
        tr3 = TransfersJsonStore()
        self.assertEqual(tr1, tr2)
        self.assertEqual(tr2, tr3)
        self.assertEqual(tr1, tr3)

    def test_deposit_singleton(self):
        tr1 = DepositsJsonStore()
        tr2 = DepositsJsonStore()
        tr3 = DepositsJsonStore()
        self.assertEqual(tr1, tr2)
        self.assertEqual(tr2, tr3)
        self.assertEqual(tr1, tr3)

    def test_deposit_singleton(self):
        dep1 = DepositsJsonStore()
        dep2 = DepositsJsonStore()
        dep3 = DepositsJsonStore()
        self.assertEqual(dep1, dep2)
        self.assertEqual(dep2, dep3)
        self.assertEqual(dep1, dep3)

    def test_balance_singleton(self):
        bal1 = BalancesJsonStore()
        bal2 = BalancesJsonStore()
        bal3 = BalancesJsonStore()
        self.assertEqual(bal1, bal2)
        self.assertEqual(bal2, bal3)
        self.assertEqual(bal1, bal3)

if __name__ == '_main_':
    unittest.main()