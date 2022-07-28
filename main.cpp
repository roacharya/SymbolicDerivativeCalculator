#include "Postfix.h"
#include "ExpressionTree.h"
#include <sstream>
#include <iostream>
#include "stdlib.h"

using namespace std;

int main() {
	string function;
	string postfix;

	cout << "Enter a function to take the derivative of: " << endl;
	getline(cin, function);
	postfix = to_postfix(function);	
	cout << endl;

	ExpressionTree* tree;
	tree = new ExpressionTree();
	tree->build(postfix);

	tree->derivative();
	tree->simplify();
	tree->simplify();
	cout << "f'(x) = ";
	tree->print();
	cout << endl;

	return 0;
}
