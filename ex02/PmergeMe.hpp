/*
This exercise is about comparing the difference in time efficiency based on the type of 
container used in sorting.
*/
#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <iostream>
# include <vector>
# include <deque>
# include <string>
# include <ctime>
# include <algorithm>
# include <iomanip>
# include <climits> // to use INT_MAX

class PmergeMe {
	private:
		std::vector<int> _vec;
		std::deque<int> _deq;

		double _vecTime;
		double _deqTime;

		void sortVector(std::vector<int>& v);
		void sortDeque(std::deque<int>& d);
		bool isPositiveInteger(const std::string& s);
		std::vector<int> generateJacobsthalSequence(int n);

	public:
		PmergeMe();
		PmergeMe(const PmergeMe& src);
		PmergeMe& operator=(const PmergeMe& src);
		~PmergeMe();

		void execute(int argc, char *argv[]);
};

#endif
