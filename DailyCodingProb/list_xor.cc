// Daily Coding Problem 6.
//   Implement a doubly-linked list, storing only a single address,
//   the xor of next and prev nodes.
// Brett Deaton - Nov 2021

#include "list_xor.h"
#include <cassert>
#include <iostream>

#define assertm(exp, msg) assert(((void)msg, exp))

XList::XList() {}

void XList::add(int val) {}

int XList::get() {}

int main() {
  std::cout << "Testing XList" << std::endl;
  XList a;
  //assertm(a!=NULL, "Xlist object not instantiated.")
  std::cout << " - Add an element" << std::endl;
  a.add(1);
  std::cout << " - Get an element" << std::endl;
  int val = a.get();
  assertm(val==1, "Unexpected value retrieved");
}