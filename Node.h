#pragma once
class Node {
 public:
  Node();
  virtual ~Node();
  void setRight(Node* node);
  Node* getRight();
  void setLeft(Node* node);
  Node* getLeft();
  void setValue(char node);
  char getValue();
 private:
  Node* right;
  Node* left;
  char value;
};

