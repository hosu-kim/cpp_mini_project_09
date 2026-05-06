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
//   false:
//     1. string is empty
//     2. non-digit character
//     3. 
bool PmergeMe::isPositiveInteger(const std::string& s) {

	// checks if the string is empty
	if (s.empty()) return false;
	// checks if the string has any non-digit character
	for (size_t i = 0; i < s.length(); ++i) {
		if (!std::isdigit(s[i])) return false;
	}

	// checks if converted number overflows or negative integer
	long long val = std::atoll(s.c_str());
	if (val > INT_MAX || val < 0) return false;

	return true;
}

/* Generates a Jacobsthal sequence where the values continues to grow
   until the last element reaches or exceeds the given value n.
*/
// Implementation of Jacobsthal Sequence: J_n = J_{n-1} + 2 * J_{n-2}
// => the next number is 'the previous number + 2 * the number before the previous one'
// e.g., 1 3 5 11 21...
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
// => This algorithm uses a Merge-Insertion method(Ford-Johnson Algorithm) that recursively sorts
// the winners and then inserts the losers back into their appropriate positions.
void PmergeMe::sortVector(std::vector<int>& v) {
	// if no element or only one element
	if (v.size() <= 1) return;

	//------------------------------ Code block 1 ------------------------------
	// Check if the number of elements is even, if not, store the last element
	// int a variable and remove it to make it even.
	// 쌍으로 묶어 비교하기 때문에 홀수면 마지막 원소는 잠시 빼두고 알고리즘 시작
	// 나머지있니?
	bool hasLeftover = (v.size() % 2 != 0);
	int leftover = 0;
	if (hasLeftover) {
		leftover = v.back(); // 마지막 원소의 값을 가져온다
		v.pop_back(); // deletes the last element
	}
	//------------------------------ Code block 2 ------------------------------
	// Pair two numbers and arrange the large number first and the small number last.
	// e.g., Before: 2 5 3 7 1 9 => After: (5, 2) (7, 3) (9, 1)
	std::vector<std::pair<int, int> > pairs;
	for (size_t i = 0; i < v.size(); i += 2) { // i => 0, 2, 4, 6...
		if (v[i] < v[i + 1]) pairs.push_back(std::make_pair(v[i + 1], v[i])); // {big, small}
		else pairs.push_back(std::make_pair(v[i], v[i + 1])); // {big, small}
	}
	//------------------------------ Code block 3 ------------------------------
	std::vector<int> mainChain;
	// Filers out only the large numbers from the pairs and store them
	// in the `mainChain`.
	// e.g., Before: (5, 2) (7, 3) (9, 1) => After: 5, 7, 9
	for (size_t i = 0; i < pairs.size(); ++i) mainChain.push_back(pairs[i].first); // 다시 승자들을 분류한다.
	//------------------------------ Code block 4 ------------------------------
	sortVector(mainChain); // Sorts the mainChain (winners) in ascending order
	                       // it doesn't have all numbers!

	//------------------------------ Code block 5 ------------------------------
	std::vector<int> pendingElements;
	std::vector<int> result = mainChain; // has winners in ascending order
	std::vector<bool> pairUsed(pairs.size(), false); // pairs.size()만큼의 칸을 만들고 false로 채운다.
	
	// `pairs` still has the first sort
	// result has numbers in ascending order
	// finds the smallest winner number and insert it's loser number in front of the result and exit the loop
	// for: iterates pairs
	// 가장 작은 승자의 짝궁을 찾고 그냥 맨 앞에 꽂아넣는 코드.
	for (size_t j = 0; j < pairs.size(); ++j) {
		if (pairs[j].first == result[0]) { // 가장 작은 winner 있는 짝을 pairs에서 찾았다면
			// insert(): pushes the provided number into the position in the container
			// insert(position, number);
			// begin(): 컨데이너의 첫번째 원소
			// 가장 작은 승자의 패자는 가장 작은 그룹에 속할 확률이 
			// 매우 높으니 result의 맨 앞에 끼워 넣어 준다.
			result.insert(result.begin(), pairs[j].second);
			pairUsed[j] = true;
			break; // for 루프 중단
		}
	}
	//------------------------------ Code block 6 ------------------------------
	/* Lines up the losers follwing the winners in order. => pendingElements.
	   오름차순으로 정렬된 승자들의 순서에 맞춰서 짝궁인 패자들을 pendingElements에 저장하여 줄 세운다.
	   why? to mathematically minimize the number of comparisons in binary search
	    */
	// iterates mainChain (winners in ascending order)
	// 앞선 코드에서 가장 작은 승자의 짝꿍을 result에 꽂아 넣었기 때문에 인덱스를 1부터 시작한다.
	for (size_t i = 1; i < mainChain.size(); ++i) {
		// iterates pairs: (winner, loser), (winner, loser)...
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
	// 앞에 하나 빼고 패자들을 모아 놓은 컨테이너의 크기로 야콥스탈 배열을 생성한다.
	std::vector<int> jacob = generateJacobsthalSequence(pendingElements.size());
	size_t prevJacob = 0;
	for (size_t i = 0; i < jacob.size(); ++i) {
		size_t currentJacob = jacob[i];
		// Ensure currentJacob does not exceed the actual number of peding elements
		// to prevent 'out of bounds' errors.
		// 현재 야콥스탈 배열의 수가 패자 모음의 수들보다 넘지 않도록 최대치를 조정한다.
		if (currentJacob > pendingElements.size()) currentJacob = pendingElements.size();

		// 야콥스탈 수열을 써서 패자들을 result에 역순으로 삽입한다.
		for (size_t j = currentJacob; j > prevJacob; --j) {
			int val = pendingElements[j - 1];
			// lower_bound(): uses binary search, which has 0(log N) complexity.
			//                This means it can find the insertion point very efficiently,
			//                even as the sorted list grows larger.
			//                타겟 이상의 값이 처음 나오는 위치 찾기
			//                왜 바이너리 탐색이라고 하냐면 매 단계마다
			//                선택지를 왼쪽 절반으로 갈 것인가, 오른쪽 절반으로 갈 것인가 둘로 나누기 때문이다.
			// 1. 비교횟수를 최소화하기 위해 사용
			// 2. 승자들을 가지고 있는 result에 패자들을 삽입하기 위해 사용
			std::vector<int>::iterator it = std::lower_bound(result.begin(), result.end(), val);
			result.insert(it, val); // it: 찾은 메모리 위치, val: 꽂아 넣을 값
		}
		prevJacob = currentJacob;
		if (prevJacob >= pendingElements.size()) break; // 패자들을 다 result에 꽂아 넣었으면 끝낸다.
	}

	// If the number of elements is odd, interts the leftover element int to its correct
	// position using binary search
	// 알고리즘의 첫 단계가 두 개씩 짝짓는 것이기 때문에 홀수일 때 빼둔 마지막 원소는 마지막에 처리해준다.
	// 데이터가 홀수라서 남은 수가 있다면 이진 탐색을 사용해서 적절한 위치에 넣는다.
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

/* std::vector와 std::deque의 차이
     vector: 요소들이 하나의 연속된 메모리로 이루어져 있어 접근 속도는 빠르지만,
             삽입할 때 뒤에 있는 데이터들을 하나 씩 뒤로 밀어야 해서 삽입 비용이 크다.
     deque: 요소들이 여러 개의 작은 덩어리로 나뉘어 연결되어 있어 접근 속도는 비교적 느리지만, 삽입 속도가 비교적 빠르다.
	        * 얼마나 큰 덩어리로 묶을 것인가는 컴파일러(STL 내부 구현체)가 자동으로 결정함.
            * 덩어리들의 메모리 주소도 자동으로 결정이됨.
*/