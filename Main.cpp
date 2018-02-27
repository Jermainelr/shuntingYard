//Program Created by Jermaine Lara
// 2/21/18
// Program does the shunting yard algorithm to print out postfix notation
// It then takes the postfix notation and builds a binary tree where the user can choose to output post,pre and infix
// References: https://en.wikipedia.org/wiki/Binary_expression_tree https://en.wikipedia.org/wiki/Shunting-yard_algorithm
// Printing notation: https://www.daniweb.com/programming/software-development/threads/310229/printing-infix-prefix-postfix-from-expression-tree
// Building Tree: https://stackoverflow.com/questions/9136153/build-binary-expression-tree

#include <stdio.h>
#include <iostream>
#include "Node.h"
#include <string.h>
using namespace std;

struct Token {
	char value;
	Token* next;
};

struct NodeStack {
	Node* node;
	NodeStack* next;
};

bool isNumber(char value);
bool isOperator(char value);
bool isLeftAssociative(char value);
int getPrecedence(char value);
void tokenOutputPush(Token* & output, char value);
void tokenStackPop(Token* &stack);
void tokenStackPush(Token* & stack, char value);
Token* shuntingYard(char* input);
NodeStack* buildTree(Token* output);
void nodePush(NodeStack* & nodeStack, Node* node);
Node* nodePop(NodeStack* &nodeStack);
void printInfix(Node* root);
void printPostfix(Node* root);
void printPrefix(Node* root);

int main() {
    char input[81];
    cout << "Enter Infix Expression" << endl;
    gets(input);
	//Apply shunting yard algorithm
    Token* output = shuntingYard(input);
    // output the postfix expression
    for (Token* t = output; t != NULL; t = t->next) {
		cout << t->value;
    }
	cout << endl;
	//Build the expression tree
    NodeStack* nodeStack = buildTree(output);
  	char command[81];
	//Main Loop 
    do {
		cout << "Enter Output Notation (INFIX, POSTFIX, PREFIX or QUIT)" << endl;
		gets(command);
		if (strcmp(command,"INFIX") == 0) {
			printInfix(nodeStack->node);
			cout << endl;
		}
		else if (strcmp(command,"POSTFIX") == 0) {
			printPostfix(nodeStack->node);
			cout << endl;
		}
		else if (strcmp(command, "PREFIX") == 0) {
			printPrefix(nodeStack->node);
			cout << endl;
		}
		else if (strcmp(command, "QUIT") != 0) {
			cout << "Invalid Command" << endl;
		}
	} while(strcmp(command, "QUIT") != 0); 
}

//Shunting yard algorithm
Token* shuntingYard(char* input) {
	Token* output = NULL;
	Token* stack = NULL;
	int length = strlen(input);
	//Walk the input tokens
	for (int i = 0; i < length; i++) {
		// If a number pass it to the output
		if(isNumber(input[i])) {
			tokenOutputPush(output, input[i]);
		}
		// If an operator push in the right order based on precedence
		else if (isOperator(input[i])) {
			if(stack != NULL && (getPrecedence(stack->value) > getPrecedence(input[i])
				|| getPrecedence(stack->value) == getPrecedence(input[i])
				&& isLeftAssociative(stack->value))) {
					tokenOutputPush(output, stack->value);
					tokenStackPop(stack);
			}
			tokenStackPush(stack, input[i]);
		}
		// If opening expression put in on the stack
		else if (input[i] == '(') {
			tokenStackPush(stack, input[i]);
		}
		else if (input[i] == ')') {
			// If closing an expression output the current stack
			while (stack != NULL && stack->value != '(') {
				tokenOutputPush(output, stack->value);
				tokenStackPop(stack);
			}
			tokenStackPop(stack);
		}
	}
	// Empty the remaining stack into the output
	for (Token* t = stack; t != NULL; t = t->next) {
		tokenOutputPush(output, t->value);
	}
	return output;
}

NodeStack* buildTree(Token* output) {
	NodeStack* nodeStack = NULL;
    // Walk the tokens
	for (Token* t = output; t != NULL; t = t->next) {
		// If a number simply push it to the top of the stack
		if (isNumber(t->value))
		{
            Node* node = new Node();
			node->setValue(t->value);
            nodePush(nodeStack, node);
        }
		// If an operator  the pointers to two trees T1 and T2 
		// are popped from the stack and a new tree whose root 
		// is the operator and whose left and right children point to T2 and T1 respectively is formed 
        else if (isOperator(t->value))
        {
            Node* node = new Node();
			node->setValue(t->value);
            node->setRight(nodePop(nodeStack));
            node->setLeft(nodePop(nodeStack));
            nodePush(nodeStack, node);
        }
    }
	return nodeStack;
}

// Adds node on top of the stack
void nodePush(NodeStack* & nodeStack, Node* node) {
     NodeStack* entry = new NodeStack;
	 entry->node = node;
	 entry->next = nodeStack;
     nodeStack = entry;	
}

// Removes the node on top of the stack and returns its value
Node* nodePop(NodeStack* &nodeStack) {
	Node* node = NULL;
	if (nodeStack != NULL) {
		node = nodeStack->node;
	    NodeStack* temp = nodeStack;
		nodeStack = nodeStack->next;
		delete temp;
	}
	return node;
}

// Tells if a token is a number
bool isNumber(char value) {
	return value >= '0' && value <= '9';
}

// Tells if operator
bool isOperator(char value) {
	return value == '^' || value == 'x' || value == '/' || value == '+' || value == '-';
}

// Tells if an operator is left associative
bool isLeftAssociative(char value) {
	return value == 'x' || value == '/' || value == '+' || value == '-';
}

// Gives a numerical value that represents the precedence of a given token
int getPrecedence(char value) {
	int precedence = 0;
	if (value == '^') {
		precedence = 4;
	}
	else if (value == '/') {
		precedence = 3;
	}
	else if (value == 'x') {
		precedence = 3;
	}
	else if (value == '-') {
		precedence = 2;
	}
	else if (value == '+') {
		precedence = 2;
	}
	return precedence;
}

// Adds a token at the end of the output
void tokenOutputPush(Token* & output, char value) {
	 Token* entry = new Token;
	 entry->value = value;
	 entry->next = NULL;
	 
	 Token* last = NULL;
	 for (Token* t = output; t != NULL; t = t->next) {
		 last = t;
	 }
	 if (last != NULL) {
		 last->next = entry;
	 }
	 else { 
		output = entry;
	 }
}

// Removes a token from the top of the stack
void tokenStackPop(Token* &stack) {
	if (stack != NULL) {
	    Token* temp = stack;
		stack = stack->next;
		delete temp;
	}
}

// Adds a token at the end of the stack
void tokenStackPush(Token* & stack, char value) {
	 Token* entry = new Token;
	 entry->value = value;
	 entry->next = stack;
     stack = entry;
}

// Infix algorithm https://en.wikipedia.org/wiki/Binary_expression_tree
void printInfix(Node* root) {
	if (root != NULL) {
		if (isOperator(root->getValue()))
		{
			cout << "(";
		}
		printInfix(root->getLeft());
		cout<< root->getValue();
		printInfix(root->getRight());
		if (isOperator(root->getValue()))
		{
			cout << ")"; 
		}
	} 
}

// Postfix Algorithm https://en.wikipedia.org/wiki/Binary_expression_tree
void printPostfix(Node* root) {
	if (root != NULL) {
		printPostfix(root->getLeft());
		printPostfix(root->getRight());
		cout<< root->getValue();
	}
}

// Prefix algorithm
void printPrefix(Node* root) {
	if (root != NULL) {
		cout<< root->getValue();
		printPostfix(root->getLeft());
		printPostfix(root->getRight());
	}
}
