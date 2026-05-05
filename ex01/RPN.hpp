/* Reverse Polish Notation (RPN)
     A mathmatical notation where operators follow their operands.
     For example, 3 + 4 => 3 4 +
*/
#ifndef RPN_HPP
# define RPN_HPP

# include <iostream>
# include <stack>
# include <string>
# include <sstream>
# include <cstdlib> // std::exit

class RPN {
	private:
		// std::stack: elements are added and removed only from the top
		std::stack<int> _stack;

	public:
	// Orthodox Canonical Form
		RPN();
		RPN(const RPN& src);
		RPN& operator=(const RPN& src);
		~RPN();

	// Member functions
		// Checks if provided argument is '+', '-', '*' or '/'.
		bool isOperator(char c);
		int performOperation(int first, int second, char op);
		void calculate(const std::string& expression);
};

#endif