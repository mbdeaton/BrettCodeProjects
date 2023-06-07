#!/usr/bin/env python3
# Brett Deaton - June 2023
"""Conversion between byte strings and standard Modbus types.

For reference, see SEL whitepaper: https://selinc.com/api/download/114988.
Assume most-significant-bit (MSB) representations throughout.
"""

import argparse


class Registers:
    """Store, display, and interpret a bytestring."""

    def __init__(self, bytestring):
        """Args:
            bytestring (bytes): bitwise values stored in the registers"""
        self.bytestring = bytestring
        self.func = lambda x: None

    def __repr__(self):
        """Returns:
            a string initializer for a copy of this object"""
        return f"Registers({self.bytestring})"

    def __str__(self):
        """Returns:
            the hex representation of these registers"""
        return self.bytestring.hex(" ")

    def set_typedef(self, typedef):
        """Set the type definition for the data stored in these registers.

        The type definition includes four factors:
        * rep (i.e. representation): int or float
        * sig (i.e. signedness): signed vs unsigned
        * bit (i.e. number of bits): 16-bit vs 32-bit

        Args:
            typedef ({str: str}): e.g. {"rep": "int"
                                        "sig": "+",
                                        "bit": "16"}
        """
        match typedef:
            case {"rep": "int", "sig": "-", "bit": "16"}:
                self.func = self.__to_si16
            case {"rep": "int", "sig": "+", "bit": "16"}:
                self.func = self.__to_ui16
            case {"rep": "int", "sig": "-", "bit": "32"}:
                self.func = self.__to_si32
            case {"rep": "int", "sig": "+", "bit": "32"}:
                self.func = self.__to_ui32
            case {"rep": "float", "sig": "+", "bit": "32"}:
                self.func = self.__to_f32
            case _:
                self.func = self.__to_bits

    def __to_si16(self, bytestring):
        """16-bit signed integer"""
        return None

    def __to_ui16(self, bytestring):
        """16-bit unsigned integer"""
        return 256*bytestring[0] + bytestring[1]

    def __to_si32(self, bytestring):
        """32-bit signed integer"""
        return None

    def __to_ui32(self, bytestring):
        """16-bit unsigned integer"""
        return None

    def __to_f32(self, bytestring):
        """32-bit float"""
        return None

    def __to_bits(self, bytestring):
        """16-bit bit-packed register"""
        bits = [f"{byte:08b}" for byte in bytestring]
        return " ".join(bits)

    def to_data(self):
        """Convert to the data type specified in the previously-set typedef"""
        return self.func(self.bytestring)


if __name__ == "__main__":
    p = argparse.ArgumentParser(
        description="Convert byte registers to modbus types")
    p.add_argument("bytestring", default=None, metavar="N", type=bytes,
                   help="bytes: value to store in the register(s)")
    p.add_argument("-f", "--float", action="store_true")
    p.add_argument("-s", "--signed", action="store_true")
    p.add_argument("-l", "--long", action="store_true")
    args = p.parse_args()

    r = Registers(args.bytestring)
    defs = {"rep": "float" if args.float else "float",
            "sig": "-" if args.signed else "+",
            "bit": "32" if args.long else "16",
            }
    r.set_typedef(defs)
