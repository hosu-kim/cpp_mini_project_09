/*
This exercise is about comparing the difference in time efficiency based on the type of 
container used in sorting.

std::vector is used to maximize the random access efficiency of the Ford-Johnson algorithm.
    1. it stores data contiguously in memory, so index access is very fast.
    2. In this algorithm, middle insertions are not frequent, it is the most efficient.
std::deque is used for bidirectional data scalability(데이터 양방향 확장성) and flexibility of memory allocation
    1. it consists of multiple memory blocks (chunks), so unlike Vector,
       there is no need to copy the entire data when allocating new memory.
    2. When there is a possibility of manipulating data back and forth, memory reallocation costs are lower than Vector.
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
