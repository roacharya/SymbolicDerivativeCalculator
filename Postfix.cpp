#include "Postfix.h"
#include <cmath>

string to_postfix(string infix) {
	string postfix = "";
	stack<char> op_stack;
	char curr_op;
	istringstream input_stream(infix);
	// iterate through input stream from infix string
	while (!input_stream.eof()) {	
		// remove whitespaces
		input_stream >> ws;
		while (infix[0] == ' ')  { 
			infix.erase(0, 1);			
		}
		// left parenthesis
		if (infix[0] == '(') {										
			input_stream >> curr_op;										
			input_stream >> ws;												

			op_stack.push(infix[0]);									
			infix.erase(0, 1);											
		}
		// right parenthesis
		else if (infix[0] == ')') {											//
			input_stream >> curr_op;											
			input_stream >> ws;												

			infix.erase(0, 1);											
			// backtrack thru stack until a left parenthesis is reached
			while (!op_stack.empty() && op_stack.top() != '(') {	
				postfix = postfix + op_stack.top() + ' ';					
				op_stack.pop();										
			}
			// if there is a remaining left parenthesis then pop it
			if (op_stack.top() == '(') {
				op_stack.pop();			
			}
		}
		// if infix begins with operator
		else if (is_operator(infix[0]))	{ 
			input_stream >> curr_op;
			input_stream >> ws;
			// go through stack
			if (!op_stack.empty())	{
				// if the precedence is higher than what is on the top of the stack
				if (get_order(infix[0]) > get_order(op_stack.top())) {	
					op_stack.push(infix[0]);										
					infix.erase(0, 1);												
				}
				// lower precedence
				else { 
					// add stack operators into the postfix string while they are higher precedence than the current operator
					while (!op_stack.empty() && get_order(op_stack.top()) > get_order(infix[0])) {
						postfix = postfix + op_stack.top() + ' ';
						op_stack.pop();
					}

					op_stack.push(infix[0]);
					infix.erase(0, 1);			
				}
			}
			// no operators in the stack so just push current one
			else {						
				op_stack.push(infix[0]);	
				infix.erase(0, 1);				
			}

		}
		// if the infix begins with something other than an operator
		else { 
			// if the character is numeric
			if (is_numeric(infix[0])) {	
				// read the operand and add it to the postfix string
				int operand;				
				input_stream >> operand;
				postfix = postfix + to_string(operand) + ' ';	

				// remove infix until first character is not numeric
				while (is_numeric(infix[0])) {
					infix.erase(0, 1);
				}
			}
			// if the infix begins with a variable
			else if (is_variable(infix[0])) {	
				// read variable and add to the postfix
				char operand;					
				input_stream >> operand;			
				postfix = postfix + operand + ' ';	

				infix.erase(0, 1);		
			}

		}
	}
	// build postfix from stack
	while (!op_stack.empty()) {
		postfix = postfix + op_stack.top() + ' ';
		op_stack.pop();
	}
	input_stream >> curr_op;

	return postfix;
}

bool is_operand(char c) {
	if (c >= 'a' && c <= 'z') {
		return true;
	}
	if (c >= '0' && c <= '9') {
		return true;
	}
	return false;
}

bool is_operand(string s) {
	if (is_numeric(s)) {
		return true;
	}
	if (is_variable(s[0])) {
		return true;
	}
	return false;
}

bool is_operator(char c) {
	if (c == '^' || c == '+' || c == '-' || c == '*' || c == '/') {
		return true;
	}
	return false;
}

bool is_numeric(char c) {
	if (c >= '0' && c <= '9') {
		return true;
	}
	return false;
}

bool is_numeric(string s) {
	if (is_numeric(s[0])) {
		return true;
	}
	if (s.length() > 1 && s[0] == '-' && is_numeric(s[1])) {
		return true;
	}
	return false;
}

bool is_variable(char c) {
	if (c >= 'a' && c <= 'z') {
		return true;
	}
	return false;
}

int	 get_order(char c) {
	if (c == '^') {
		return 4; 
	}
	else if (c == '*' || c == '/') {
		return 3;
	}
	else if (c == '-') {
		return 2;
	}
	else if (c == '+') {
		return 1;
	}
	return 0;
}
