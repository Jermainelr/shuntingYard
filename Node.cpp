#include "Node.h"
#include <iostream>
using namespace std;

Node::Node() {
	right = NULL;
	left = NULL;
}

Node::~Node() {

}

Node* Node::getRight() {
  return right;
}

Node* Node::getLeft() {
  return left;
}

void Node::setRight(Node* node) {
  right = node;
}

void Node::setLeft(Node* node) {
  left = node;
}

void Node::setValue(char val) {
	value = val;
}

char Node::getValue() {
	return value;
}