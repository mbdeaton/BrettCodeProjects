#!/usr/bin/env python3
"""Daily Coding Problem 16.
Implement an order record of fixed size, retaining no more than N orders.
"""

import unittest

class OrderRecord:
    """A capped-length queue of order ids."""
    def __init__(self, max_orders=10):
        self.max_orders = max_orders
        self.orders = [] # queue with oldest at the front
    
    def record(self, order_id):
        """Add a new record to the log"""
        if len(self.orders) >= self.max_orders:
            del self.orders[0]
        self.orders.append(order_id)

    def get_last(self, i=0):
        """Return the ith most recent order from the log, 0<=i<max"""
        reversed_orders = list(reversed(self.orders))
        return reversed_orders[i]

    def __repr__(self):
        return str(self.orders)

class TestRecordNOrders(unittest.TestCase):
    def test_create_blank_record(self):
        orders = OrderRecord()
    
    def test_record_and_retrieve_order(self):
        orders = OrderRecord()
        id = 1000
        orders.record(id)
        self.assertEqual(orders.get_last(), id)

    def test_record_too_many_orders(self):
        max_orders = 3
        orders = OrderRecord(max_orders)
        extra_orders = 10
        ids = [1000+x for x in range(max_orders+extra_orders)]
        for id in ids:
            orders.record(id)
        self.assertEqual(orders.get_last(), ids[-1]) # check newest
        self.assertEqual(orders.get_last(max_orders-1), # check oldest
                         ids[extra_orders])

if __name__ == "__main__":
    unittest.main(verbosity=2)
