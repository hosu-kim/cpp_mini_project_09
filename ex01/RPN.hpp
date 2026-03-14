#ifndef RPN_HPP
# define RPN_HPP

# include <iostream>
# include <stack>
# include <string>
# include <sstream>
# include <cstdlib> // std::exit

class RPN {
	private:
		std::stack<int> _stack;

	public:
		RPN();
		RPN(const RPN& src);
		RPN& operator=(const RPN& src);
		~RPN();

		bool isOperator(char c);
		int performOperation(int first, int second, char op);
		void calculate(const std::string& expression);
};

#endif