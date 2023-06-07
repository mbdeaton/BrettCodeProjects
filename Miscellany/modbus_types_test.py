import unittest
from modbus_types import Registers

# Execute with python3 -m unittest -v modbus_types_test.py


class TestPairs(unittest.TestCase):
    def setUpClass(self):
        self.reg0 = Registers(0x1234)
        self.reg2 = Registers(0xabcd)

    def test_uint16(self):
        defs = {"rep": "int", "sig": "+", "bit": "16"}
        self.reg0.set_typedef(defs)
        self.assertEqual(self.reg0.to_data(), 4660)
        self.assertEqual(self.reg1.to_data(), 43981)
