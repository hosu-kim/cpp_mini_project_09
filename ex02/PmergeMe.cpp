#include "PmergeMe.hpp"

//========================== Orthodox Canonical Form ===========================
PmergeMe::PmergeMe() : _vecTime(0), _deqTime(0) {}
PmergeMe::PmergeMe(const PmergeMe& src) { *this = src; }
PmergeMe& PmergeMe::operator=(const PmergeMe& src) {
	if (this != &src) {
		_vec = src._vec;
		_deq = src._deq;
	}
	return *this;
}
PmergeMe::~PmergeMe() {}
//==============================================================================
// checks if the input string is a valid integer after conversion
bool PmergeMe::isPositiveInteger(const std::string& s) {

	// if the string is empty
	if (s.empty()) return false;
	// if the string has any non-digit character
	for (size_t i = 0; i < s.length(); ++i) {
		if (!std::isdigit(s[i])) return false;
	}

	// if the string after conversion to int overflows or negative integer
	long long val = std::atoll(s.c_str());
	if (val > INT_MAX || val < 0) return false;

	return true;
}

// Implementation of Jacobsthal Sequence: J_n = J_{n-1} + 2 * J_{n-2}
// => the next number is 'the previous number + 2 * the number before the previous one'
// e.g., 1 3 5 11 21...
// int n: numbers of Jacobsthal Sequence elements you wanna make
// The sequence makes all elements find their places with the fewest possible comparisons.
std::vector<int> PmergeMe::generateJacobsthalSequence(int n) {
	std::vector<int> jacob;
	jacob.push_back(1); // setting the start points of the sequence.
	jacob.push_back(3); // setting the start points of the sequence.
	// back(): returns the reference of the last element
	// CAUTION: if the container is empty, using it causes runtime error!
	while (jacob.back() < n) {
		jacob.push_back(jacob.back() + 2 * jacob[jacob.size() - 2]);
	}
	return jacob;
}

// 원본 데이터를 직접 수정하기 위해 참조를 인수로 받음.
// The implementation of Ford-Johnson algorithm
// => This algorithm uses a Merge-Insertion method that recursively sorts
// the winners and then inserts the losers back into their appropriate positions.
void PmergeMe::sortVector(std::vector<int>& v) {
	// if no element or only one element
	if (v.size() <= 1) return;

	//------------------------------ Code block 1 ------------------------------
	// Check if the number of elements is even, if not, store the last element
	// int a variable and remove it to make it even.
	// 나머지있니?
	bool hasLeftover = (v.size() % 2 != 0);
	int leftover = 0;
	if (hasLeftover) {
		leftover = v.back();
		v.pop_back(); // deletes the last element
	}
	//------------------------------ Code block 2 ------------------------------
	// Pair two numbers and arrange the large number first and the small number last.
	// e.g., Before: 2 5 3 7 1 9 => After: (5, 2) (7, 3) (9, 1)
	std::vector<std::pair<int, int> > pairs;
	for (size_t i = 0; i < v.size(); i += 2) { // i => 0, 2, 4, 6...
		if (v[i] < v[i + 1]) pairs.push_back(std::make_pair(v[i + 1], v[i]));
		else pairs.push_back(std::make_pair(v[i], v[i + 1]));
	}
	//------------------------------ Code block 3 ------------------------------
	std::vector<int> mainChain;
	// Filers out only the large numbers from the pairs and store them
	// in the `mainChain`.
	// e.g., Before: (5, 2) (7, 3) (9, 1) => After: 5, 7, 9
	for (size_t i = 0; i < pairs.size(); ++i) mainChain.push_back(pairs[i].first);
	//------------------------------ Code block 4 ------------------------------
	sortVector(mainChain); // Sorts the mainChain (winners) in ascending order
	                       // it doesn't have all numbers!

	//------------------------------ Code block 5 ------------------------------
	std::vector<int> pendingElements;
	std::vector<int> result = mainChain;
	std::vector<bool> pairUsed(pairs.size(), false);
	
	// `pairs` still has the first sort
	// result has numbers in ascending order
	// finds the smallest winner number and insert it's loser number in front of the result and exit the loop
	for (size_t j = 0; j < pairs.size(); ++j) {
		if (pairs[j].first == result[0]) {
			// insert(): pushes the provided number into the position in the container
			// insert(position, number);
			result.insert(result.begin(), pairs[j].second);
			pairUsed[j] = true;
			break; // for 루프 중단
		}
	}
	//------------------------------ Code block 6 ------------------------------
	// Stores the loser numbers of the winnder numbers sorted in ascending order
	// why? to mathematically minimize the number of comparisons in binary search
	for (size_t i = 1; i < mainChain.size(); ++i) {
		for (size_t j = 0; j < pairs.size(); ++j) {
			if (!pairUsed[j] && pairs[j].first == mainChain[i]) {
				pendingElements.push_back(pairs[j].second);
				pairUsed[j] = true;
				break;
			}
		}
	}

	//------------------------------ Code block 7 ------------------------------
	// Insert pending elements in the optimal Jacobstal order
	// to minimize the number of comparisons using binary search
	std::vector<int> jacob = generateJacobsthalSequence(pendingElements.size());
	size_t prevJacob = 0;
	for (size_t i = 0; i < jacob.size(); ++i) {
		size_t currentJacob = jacob[i];
		// Ensure currentJacob does not exceed the actual number of peding elements
		// to prevent 'out of bounds' errors.
		if (currentJacob > pendingElements.size()) currentJacob = pendingElements.size();

		for (size_t j = currentJacob; j > prevJacob; --j) {
			int val = pendingElements[j - 1];
			// lower_bound(): uses binary search, which has 0(log N) complexity.
			//                This means it can find the insertion point very efficiently,
			//                even as the sorted list grows larger.
			std::vector<int>::iterator it = std::lower_bound(result.begin(), result.end(), val);
			result.insert(it, val);
		}
		prevJacob = currentJacob;
		if (prevJacob >= pendingElements.size()) break;
	}

	// If the number of elements is odd, interts the leftover element int to its correct
	// position using binary search
	if (hasLeftover) {
		std::vector<int>::iterator it = std::lower_bound(result.begin(), result.end(), leftover);
		result.insert(it, leftover);
	}
	v = result; // sorting finished
}

void PmergeMe::sortDeque(std::deque<int>& d) {
	if (d.size() <= 1) return;

	bool hasLeftover = (d.size() % 2 != 0);
	int leftover = 0;
	if (hasLeftover) {
		leftover = d.back();
		d.pop_back();
	}

	std::deque<std::pair<int, int> > pairs;
	for (size_t i = 0; i < d.size(); i += 2) {
		if (d[i] < d[i + 1]) pairs.push_back(std::make_pair(d[i + 1], d[i]));
		else pairs.push_back(std::make_pair(d[i], d[i + 1]));
	}

	std::deque<int> mainChain;
	for (size_t i = 0; i < pairs.size(); ++i) mainChain.push_back(pairs[i].first);
	sortDeque(mainChain);

	// 4.
	std::deque<int> pendingElements;
	std::deque<int> result = mainChain;
	std::deque<bool> pairUsed(pairs.size(), false);

	for (size_t j = 0; j < pairs.size(); ++j) {
		if (pairs[j].first == result[0]) {
			result.insert(result.begin(), pairs[j].second);
			pairUsed[j] = true;
			break;
		}
	}

	for (size_t i = 1; i < mainChain.size(); ++i) {
		for (size_t j = 0; j < pairs.size(); ++j) {
			if (!pairUsed[j] && pairs[j].first == mainChain[i]) {
				pendingElements.push_back(pairs[j].second);
				pairUsed[j] = true;
				break;
			}
		}
	}

	std::vector<int> jacob = generateJacobsthalSequence(pendingElements.size());
	size_t prevJacob = 0;
	for (size_t i = 0; i < jacob.size(); ++i) {
		size_t currentJacob = jacob[i];
		if (currentJacob > pendingElements.size()) currentJacob = pendingElements.size();

		for (size_t j = currentJacob; j > prevJacob; --j) {
			int val = pendingElements[j - 1];
			std::deque<int>::iterator it = std::lower_bound(result.begin(), result.end(), val);
			result.insert(it, val);
		}
		prevJacob = currentJacob;
		if (prevJacob >= pendingElements.size()) break;
	}

	if (hasLeftover) {
		std::deque<int>::iterator it = std::lower_bound(result.begin(), result.end(), leftover);
		result.insert(it, leftover);
	}
	d = result;
}

void PmergeMe::execute(int argc, char* argv[]) {
	for (int i = 1; i < argc; ++i) {
		if (!isPositiveInteger(argv[i])) {
			std::cerr << "Error" << std::endl;
			exit(1);
		}
		int val = std::atoi(argv[i]);
		_vec.push_back(val);
		_deq.push_back(val);
	}

	std::cout << "Before: ";
	for (size_t i = 0; i < _vec.size(); ++i) std::cout << _vec[i] << " ";
	std::cout << std::endl;

	// clock(): Used to measure the time the CPU spends on a process after the program starts.
	//          Returns clock_t value
	clock_t start = clock();
	sortVector(_vec);
	_vecTime = static_cast<double>(clock() - start) / CLOCKS_PER_SEC * 1000000; // microsecond

	start = clock();
	sortDeque(_deq);
	_deqTime = static_cast<double>(clock() - start) / CLOCKS_PER_SEC * 1000000;

	std::cout << "After: ";
	for (size_t i = 0; i < _vec.size(); ++i) std::cout << _vec[i] << " ";
	std::cout << std::endl;

	std::cout << "Time to process a range of " << _vec.size() << " elements with std::vector : "
	<< std::fixed << std::setprecision(5) << _vecTime << " us" << std::endl;
	std::cout << "Time to process a range of " << _deq.size() << " elements with std::deque : "
	<< std::fixed << std::setprecision(5) << _deqTime << " us" << std::endl;
}
