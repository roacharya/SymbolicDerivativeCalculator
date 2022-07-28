#ifndef EXPRESSION_TREE
#define EXPRESSION_TREE

#include "Postfix.h"
#include <string>
#include <stack>
#include <iostream>
#include <cmath>

class ExpressionTree
{
private:
	string val;
	ExpressionTree *left, *right;
	bool is_function_leaf;
public:
	// constructor
	ExpressionTree();

	// recursively copies an ExpressionTree and returns the copy
	ExpressionTree * copy();

	// builds ExpressionTree from postfix
	void build(string PF);

	// performs inorder traversal of ExpressionTree
	virtual void inorder();
	
	// performs inorder traversal of ExpressionTree with extra parentheses for chain rule
	virtual void inorder_chain_rule();

	// prints ExpressionTree in normal mathematical format
	void print();

	// recursively destroys tree and its children
	void destroy();

	// recursively converts ExpressionTree into its derivative
	void derivative();

	// simplifies ExpressionTree where possible
	void simplify();
};

#endif
