"""Test Registers in modbus_types.

Example usage:
    $ python3 -m unittest -v modbus_types_test.py
"""
import unittest
from modbus_types import Registers


class TestPairs(unittest.TestCase):
    """Test two registers interpreted as different types"""

    def setUpClass(self):
        """Create two 2-byte test registers"""
        self.reg0 = Registers(0x1234)
        self.reg1 = Registers(0xabcd)

    def test_uint16(self):
        """Interpret registers as 16-bit unsigned integers"""
        defs = {"rep": "int", "sig": "+", "bit": "16"}
        self.reg0.set_typedef(defs)
        self.assertEqual(self.reg0.to_data(), 4660)
        self.assertEqual(self.reg1.to_data(), 43981)
