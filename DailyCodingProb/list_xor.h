class Node {
 public:
  int val_;
  int both_; // xor of next and prev
};

class XList {
 public:
  XList();
  void add(int val);
  int get();
 private:
  Node node_;
};