#include "BitcoinExchange.hpp"

// ================ PRIVATE MEMBER FUNCTIONS ================
bool BitcoinExchange::isValidDate(const std::string& date) const {
	// 2026-03-13 => 10 characters, 4th char: '-', 7th char: '-'
	if (date.length() != 10 || date[4] != '-' || date[7] != '-') return false;
	
	// substr(index, charsToCopy)
	// |2026|-03-13
	int year = std::atoi(date.substr(0, 4).c_str());
	// 2026-|03|-13
	int month = std::atoi(date.substr(5, 2).c_str());
	// 2026-03-|13|
	int day = std::atoi(date.substr(8, 2).c_str());

	// Month must be the one between January and December
	if (month < 1 || month > 12) return false;

	//                   Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
	int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	// Leap year calculation:
	// Divisible by 4, but not by 100 unless also divisible by 400
	bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
	if (month == 2 && isLeap) daysInMonth[1] = 29;

	// Depending on the given monthm the days must be between 1 and 28/31
	if (day < 1 || day > daysInMonth[month - 1])
		return false;

	return true;
}

bool BitcoinExchange::isValidValue(const float value) const {
	// DOC: A valid value must be either a float or a positive integer, between 0 and 1000.
	// e.g., if '-1' is given
	if (value < 0) {
		std::cout << "Error: not a positive number." << std::endl;
		return false;
	}
	if (value > 1000) {
		std::cout << "Error: too large a number." << std::endl;
		return false;
	}
	return true;
}

float BitcoinExchange::getExchangeRate(const std::string& date) {
	// lower_bound(key): points date(key) in the database
	//                If the exact date(key) is not found, points to the next greater element
	//                (the first one that exceeds it)
	//                e.g.,
	//                - Database data: `2026-01-01`, `2026-01-03`, `2026-01-05`
	//                - input: `2026-01-04`
	//                => returns an iterator points to `2026-01-05`
	std::map<std::string, float>::iterator it = _data.lower_bound(date);
	// 1. Checks 'it != end()' first to avoid accessing invalid memory => using '->'
	// 2. Then check 'it->first == date' for an exact match
	if (it != _data.end() && it->first == date)
		return it->second; // Case 1: exact date found in datebase.
	
	// begin(): the first iterator in the map
	if (it == _data.begin()) return -1; // no earlier date exists in the database
	// DOC: If the date used in the input does not exist in your DB then you
	//      must use the closest date contained in your DB.
	// --it => moves back to the nearest past date
	--it;
	return it->second; // returns value
}
//==============================================================================
// ORTHODOX CANONICAL FORM
BitcoinExchange::BitcoinExchange() {
	std::ifstream file("data.csv");
	if (!file.is_open()) {
		std::cerr << "Error: could not open data.csv" << std::endl;
		return;
	}
	std::string line;
	std::getline(file, line); // skips the header: "date | value"

	while (std::getline(file, line)) {
		size_t delim = line.find(',');
		if (delim != std::string::npos) {
			std::string date = line.substr(0, delim);
			float rate = static_cast<float>(atof(line.substr(delim + 1).c_str()));
			_data[date] = rate;
		}
	}
	file.close();
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) { *this = other; }

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
	if (this !=  &other) this->_data = other._data;
	return *this;
}

BitcoinExchange::~BitcoinExchange() {}
//==============================================================================

void BitcoinExchange::execute(const std::string& filename) {
	std::ifstream file(filename.c_str());
	if (!file.is_open()) {
		std::cerr << "Error: could not open input.txt" << std::endl;
		return;
	}

	std::string line;
	std::getline(file, line); // skips the header: "date | value"

	while (std::getline(file, line)) {
		size_t delim = line.find(" | ");
		if (delim == std::string::npos) {
			std::cout << "Error: bad input => " << line << std::endl;
			// continue를 만나면 아래 코드는 전부 무시하고 다시 루프의 처음으로 돌아간다.
			continue;
		}

		std::string date = line.substr(0, delim);
		float value = static_cast<float>(atof(line.substr(delim + 3).c_str()));

		if (!isValidDate(date)) {
			std::cout << "Error: bad input => " << date << std::endl;
			continue;
		}

		if (!isValidValue(value)) {
			continue;
		}

		float rate = getExchangeRate(date);
		if (rate == -1) {
			std::cout << "Error: no historical data => " << date << std::endl;
		} else {
			std::cout << date << " => " << value << " = " << (value * rate) << std::endl;
		}
	}
}