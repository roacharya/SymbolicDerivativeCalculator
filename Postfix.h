#ifndef POSTFIX
#define POSTFIX

#include <string>
#include <stack>
#include <sstream>

using namespace std;

// converts infix to postfix to help build ExpressionTree
string to_postfix(string infix);

//checks if character is an operand
bool is_operand(char c);

//checks if string is an operand
bool is_operand(string s);

//checks if character is an operator
bool is_operator(char c);

// checks if character is numeric
bool is_numeric(char c);

//checks if string is numeric
bool is_numeric(string s);

// checks if character is a variable
bool is_variable(char c);

// gets precedence of character based on order of operations
int	 get_order(char c);

#endif 