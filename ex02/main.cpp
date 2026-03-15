#include "PmergeMe.hpp"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Error" << std::endl;
		return 1;
	}
	PmergeMe pm;
	pm.execute(argc, argv);
	return 0;
}