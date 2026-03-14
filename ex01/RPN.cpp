#include "RPN.hpp"

// orthodox canonial form
RPN::RPN() {}

RPN::RPN(const RPN& src) {
	*this = src;
}

RPN& RPN::operator=(const RPN& src) {
	if (this != &src)  {
		this->_stack = src._stack;
	}
	return *this;
}

RPN::~RPN() {}
//===================================

bool RPN::isOperator(char c) {
	return (c == '+' || c == '-' || c == '*' || c == '/');
}

int RPN::performOperation(int first, int second, char op) {
	if (op == '+') return first + second;
	if (op == '-') return first - second;
	if (op == '*') return first * second;
	if (op == '/') {
		if (second == 0) {
			std::cerr << "Error: Division by zero" << std::endl;
			std::exit(1);
		}
		return first / second;
	}
	return 0;
}

void RPN::calculate(const std::string& expression) {
	for (size_t i = 0; i < expression.length(); ++i) {
		char c = expression[i];

		if (std::isspace(c)) continue;

		if (std::isdigit(c)) {
			_stack.push(c - '0');
		} else if (isOperator(c)) {
			if (_stack.size() < 2) {
				std::cerr<< "Error" << std::endl;
				return;
			}

			int second = _stack.top(); _stack.pop();
			int first = _stack.top(); _stack.pop();
			
			_stack.push(performOperation(first, second, c));
		} else {
			std::cerr << "Error" << std::endl;
			return;
		}
	}

	if (_stack.size() != 1) {
		std::cerr << "Error" << std::endl;
	} else {
		std::cout << _stack.top() << std::endl;
	}
}