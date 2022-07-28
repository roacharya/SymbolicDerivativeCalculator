#include "ExpressionTree.h"

using namespace std;

// constructor
ExpressionTree::ExpressionTree() {
	val = "";
	left = NULL;
	right = NULL;
	// used in chain rule to help print extra parentheses
	is_function_leaf = false;
}

// recursively copies an ExpressionTree and returns the copy
ExpressionTree* ExpressionTree::copy() {
	ExpressionTree *copy = new ExpressionTree();
	copy->val = val;

	if (left != NULL)  {
		copy->left = left->copy();
	}

	if (right != NULL) {
		copy->right = right->copy();
	}

	return copy;
}

// builds ExpressionTree from postfix
void ExpressionTree::build(string postfix) {
	// use stack to create expression tree of operators and operands
	stack<ExpressionTree*> tree_stack;
	ExpressionTree *tree1;
	ExpressionTree *tree2;
	string curr_val = "";

	// iterate through postfix string
	while (postfix.length() > 0) {
		// whitespace
		if (postfix[0] == ' ') {
			postfix.erase(0, 1);			
		}
		// operator
		else if (is_operator(postfix[0])) {
			curr_val += postfix[0];
			postfix.erase(0, 1);
			// remove top 2 trees from stacks
			tree1 = tree_stack.top(); tree_stack.pop();
			tree2 = tree_stack.top(); tree_stack.pop();

			ExpressionTree *tree =  new ExpressionTree();		
			// set previous two trees to children of new tree and push new tree to stack
			tree->val = curr_val;				
			tree->left = tree2;
			tree->right = tree1;
			tree_stack.push(tree);
			curr_val = "";
		}
		// operand
		else if (is_operand(postfix)) {
			// get entire operand
			while (is_operand(postfix)) {
				curr_val+= postfix[0];		
				postfix.erase(0, 1);
			}

			ExpressionTree *tree = new ExpressionTree();
			// push new tree to stack
			tree->val = curr_val;
			tree_stack.push(tree);
			curr_val = "";
		}
	}	
	// update instance variables
	val = tree_stack.top()->val;
	left = tree_stack.top()->left;
	right = tree_stack.top()->right;
	tree_stack.pop();
}

// performs inorder traversal of ExpressionTree
void ExpressionTree::inorder() {
	// need extra parentheses for chain rule
	if (is_function_leaf) {
		inorder_chain_rule();
		return;
	}

	if (left != NULL) {
		cout << "(";
		left->inorder();
	}

	cout << ' ' << val << ' ';

	if (right != NULL) {
		right->inorder();
		cout << ")";
	}
}

// performs inorder traversal of ExpressionTree with extra parentheses for chain rule
void ExpressionTree::inorder_chain_rule() {
	if (left != NULL) {
		cout << "(";
		left->inorder();
	}	
	cout << '(';
	if (right != NULL)	{
		right->inorder();
	}
	cout << ')';
	if (right != NULL)	{
		cout << ")";
	}
}

// prints ExpressionTree in normal mathematical format
void ExpressionTree::print() {
	if (left != NULL) {
		left->inorder();
	}
	// prints value
	cout << ' ' << val << ' ';

	if (right != NULL) {
		right->inorder();
	}

	cout << endl;
}

// recursively destroys tree and its children		            
void ExpressionTree::destroy() {
	if (left != NULL) {
		left->destroy();
	}

	if (right != NULL) {
		right->destroy();
	}
	// delete current instance
	delete this;
}

// recursively converts ExpressionTree into its derivative
void ExpressionTree::derivative() {
	// allocates 5 expression trees - needed for quotient rule and product rule
	ExpressionTree *T1, *T2, *T3, *T4, *T5;
	// derivative of a variable is 1
	if (is_variable(val[0])) { 
		val = "1";		
	}
	// derivative of a constant is 0
	else if (is_numeric(val[0])) { 
		val = "0";
	}		
	// operand
	else {
		switch (val[0]) {	
		// for addition just take derivative of left and right
		case '+':	
 			left->derivative();
			right->derivative();
			break;
		// for subtraction just take derivative of left and right
		case '-':	
			left->derivative();
			right->derivative();
			break;
		// product rule
		case '*':
			T1 = left->copy();
			T2 = right->copy();
			T3 = right->copy();
			T4 = left->copy();
			// current tree will be addition
			val = "+";		
			// left and right will be the multiplication
			left->val = "*";	
			right->val = "*";
			// cut branches
			if (left->left != NULL) {
				left->left->destroy();
			}

			if (left->left != NULL) {
				left->right->destroy();
			}
			// first part of product rule (T1 * T2')
			left->left = T1;
			left->right = T2;
			T2->derivative();
			// cut branches
			if (right->left != NULL) {
				right->left->destroy();
			}

			if (right->right != NULL) {
				right->right->destroy();
			}
			// second part of product rule (T3 * T4')
			right->left = T3;
			right->right = T4;
			T4->derivative();

			break;
		// quotient rule
		case '/':	
			// numerators
			T1 = left->copy();
			T4 = left->copy();
			// denominators
			T2 = right->copy();
			T3 = right->copy();
			T5 = right->copy();

			// make left tree the subtraction
			left->val = "-";

			// allocate new tree
			if (left->left == NULL)	{
				left->left = new ExpressionTree();
			}

			// will be the multiplication tree
			left->left->val = "*";

			// cut branches
			if (left->left->left != NULL) {
				left->left->left->destroy();
			}

			if (left->left->right != NULL) {
				left->left->right->destroy();
			}
			// first part of quotient rule (T1' * T2)
			left->left->left = T1;		
			left->left->right = T2;		
			T1->derivative();

			// allocate new tree
			if (left->right == NULL) {
				left->right = new ExpressionTree();	
			}

			// will be the multiplication tree
			left->right->val = "*";

			// cut branches
			if (left->right->left != NULL) {
				left->right->left->destroy();		
			}

			if (left->right->right != NULL)	{
				left->right->right->destroy();	
			}
			
			// second part of quotient rule (T3' * T4)
			left->right->left = T3;		
			left->right->right = T4;		
			T3->derivative();

			// denominator in quotient rule (denom ^ 2)
			right->val = "^";
			right->left = T5;

			// allocate new tree
			if (right->right == NULL) {
				right->right = new ExpressionTree();
			}

			// power
			right->right->val = "2";

			// cut branches
			if (right->right->left != NULL) {
				right->right->left->destroy();
				right->right->left = NULL;
			}

			if (right->right->right != NULL) {
				right->right->right->destroy();
				right->right->right = NULL;
			}

			break;

		// exponentiation
		case '^':
			ExpressionTree * base = left->copy();	
			ExpressionTree* power = right->copy();
			// multiplication tree
			val = "*";
			left->destroy();
			right->destroy();
			left = NULL;
			right = NULL;

			// derivative of constant is 0
			if (is_numeric(power->val) && is_numeric(base->val)) {
				val = "0";
			}
			// if power is numerical
			else if (is_numeric(power->val)) {
				right = new ExpressionTree();
				// exponentiation tree
				right->val = "^";
				double powerDouble = stod(power->val);
				int decIndex;

				right->right = new ExpressionTree();
				// power rule since deriv of x ^ n is n * x ^ (n - 1)
				right->right->val = to_string((powerDouble - 1));
				decIndex = right->right->val.find('.');
				right->right->val = right->right->val.substr(0, decIndex + 2);

				right->left = base->copy();
				// if the base is not an operand
				if (!is_operand(base->val)) {
					ExpressionTree * toDeriv;
					toDeriv = base->copy();
					toDeriv->derivative();
					left = new ExpressionTree();
					// multiplication tree
					left->val = "*";
					left->left = toDeriv;
					left->right = power->copy();
				}

				else {
					left = power->copy();
				}
			}
			// if power is a variable
			else {
				// exponentiation tree
				left = new ExpressionTree();
				left->val = "^";

				left->left = base->copy();
				left->right = power->copy();
				//chain rule if there is more than a single numeric val in power
				if (power->left != NULL && power->right != NULL) { 
					// multiplication tree for exponentiation derivative
					// deriv of a ^ x is a ^ x * ln a
					right = new ExpressionTree();
					right->val = "*";
					right->left = power->copy();
					right->left->derivative();
					right->right = new ExpressionTree();
					right->right->is_function_leaf = true;
					right->right->left = new ExpressionTree();
					right->right->left->val = "ln";
					right->right->right = base->copy();

				}
				//do exponential function differentiation rule w/o chain rule
				else { 
					// deriv of a ^ x is a ^ x * ln a
					right = new ExpressionTree();
					right->is_function_leaf = true;
					right->left = new ExpressionTree();
					right->left->val = "ln";

					right->right = new ExpressionTree();
					right->right = base->copy();
				}
			}
			// clean up
			power->destroy();
			base->destroy();

			break;
		}
	}
}
// simplifies ExpressionTree where possible
void ExpressionTree::simplify() {
	// if val is numeric no need to simplify
	if (is_numeric(val))	{
		return;
	}
	// if val begins with varianle then no need to simplify either
	if (is_variable(val[0])) {
		return;
	}

	switch (val[0]) {
	// multiplication
	case '*':
		// multiplying by 0 rule
		if (left->val == "0" || right->val == "0") {
			val = "0";
			left->destroy();
			right->destroy();
			left = NULL;
			right = NULL;
		}
		// two numeric values so just multiply
		else if (is_numeric(left->val) && is_numeric(right->val)) {
			double a, b;
			int decIndex;
			a = stod(left->val);
			b = stod(right->val);
			val = to_string(a*b);
			decIndex = val.find('.');
			val = val.substr(0, decIndex + 2);
			left->destroy();
			right->destroy();
			left = NULL;
			right = NULL;
		}
		// if left val is 1 then just keep right only
		else if (left->val == "1" || left->val == "1.0") {
			ExpressionTree * T1;
			T1 = right->copy();
			left->destroy();
			right->destroy();
			val = T1->val;
			left = T1->left;
			right = T1->right;
		}
		// if the right is a multiplication tree
		else if (right->val == "*") {
			// if the left val is a number and the right has numerics too then just multiply 
			if (is_numeric(left->val) && is_numeric(right->left->val)) {
				ExpressionTree *T1;
				T1 = right->right->copy();
				double num1 = stod(left->val);
				double num2 = stod(right->left->val);
				right->left->destroy();
				right->right->destroy();
				double prod = num1 * num2;
				int decIndex;

				left->val = to_string(prod);
				decIndex = left->val.find('.');
				left->val = left->val.substr(0, decIndex + 2);
				right = T1;
			}

		}
		break;
	// addition
	case '+':
		// adding 0 is itself
		if (left->val == "0") {
			ExpressionTree *T1;
			T1 = right->copy();		
			right->destroy();				
			left->destroy();
			val = T1->val;		
			left = T1->left;		
			right = T1->right;
		}
		// adding 0 is itself
		else if (right->val == "0") {
			ExpressionTree *T1;
			T1 = left->copy();
			right->destroy();
			left->destroy();
			val = T1->val;
			left = T1->left;
			right = T1->right;
		}
		break;
	// subtraction
	case '-':
		// subtracting 0 is itself
		if (right->val == "0") {
			ExpressionTree *T1;
			T1 = left->copy();
			right->destroy();
			left->destroy();
			val = T1->val;
			left = T1->left;
			right = T1->right;

		}

		break;
	// exponentiation
	case '^':
		// raising to power of 1 is itself
		if (right->val == "1" || right->val == "1.0") {	//if it's to the power of 1, make this tree remove the power and make this tree the base tree												
			ExpressionTree *T1;
			T1 = left->copy();
			right->destroy();
			left->destroy();
			val = T1->val;
			left = T1->left;
			right = T1->right;
		}
		break;
	}
	// call simplify on left node
	if (left != NULL) {
		left->simplify();
	}
	// call simplify on right node
	if (right != NULL) {
		right->simplify();
	}
}

