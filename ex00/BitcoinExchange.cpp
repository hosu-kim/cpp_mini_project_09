#include "BitcoinExchange.hpp"

// ========================= ORTHODOX CANONICAL FORM =========================
// Assigns the values to the map container, _data
BitcoinExchange::BitcoinExchange() {
	std::ifstream file("data.csv");
	if (!file.is_open()) {
		std::cerr << "Error: could not open data.csv" << std::endl;
		return;
	}
	std::string line;
	std::getline(file, line); // skips the header: "date,exchange_rate"

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
	if (this != &other) this->_data = other._data;
	return *this;
}

BitcoinExchange::~BitcoinExchange() {}


// ===================== PRIVATE MEMBER FUNCTIONS =====================

// Validates if the date format is YYYY-MM-DD and, the dates are appropriate in "input.txt".
bool BitcoinExchange::isValidDate(const std::string& date) const {
	// 2026-03-13 => if !10 characters, if 4th char != '-', if 7th char != '-', return false
	if (date.length() != 10 || date[4] != '-' || date[7] != '-') return false;
	
	// substr(startIndex, charsToCopy)
	// |2026|-03-13
	int year = std::atoi(date.substr(0, 4).c_str());
	// 2026-|03|-13
	int month = std::atoi(date.substr(5, 2).c_str());
	// 2026-03-|13|
	int day = std::atoi(date.substr(8, 2).c_str());

	// Month must be the one between 1 and 12
	if (month < 1 || month > 12) return false;

	//                   Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
	int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	// Leap year calculation:
	// Divisible by 4, but not by 100 unless also divisible by 400
	bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
	if (month == 2 && isLeap) daysInMonth[1] = 29;

	// Depending on the given months the days, validates `day` if it's in the range of daysInMonth
	if (day < 1 || day > daysInMonth[month - 1])
		return false;

	return true;
}

// checks values(keys) in the "input.txt" and returns true if valid or false if invalid
// input.txt: e.g., "2011-01-03 | 3"
bool BitcoinExchange::isValidValue(const float value) const {
	// DOC: A valid value must be either a float or a positive integer, between 0 and 1000.
	// e.g., if '-1' is given, false
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
	// std::map::lower_bound(key): Points key(date) in the database
	//                If the exact key(date) is not found, points to the next greater element
	//                e.g.,
	//                  - Database data: `2026-01-01`, `2026-01-03`, `2026-01-05`
	//                  - input: `2026-01-04`
	//                    => the key is not found.
	//                    => returns the next greater iterator points to `2026-01-05`
	std::map<std::string, float>::iterator it = _data.lower_bound(date);
	// 1. Checks 'it != end()' first to avoid accessing invalid memory => using '->' (ootherwise crash!)
	// 2. Then check 'it->first == date' for an exact match
	if (it != _data.end() && it->first == date)
		return it->second; // returns the value of the exact date found in "data.csv".
	
	// Exact date is not found in the database
	// begin(): the first iterator in the map를 만나면 아래 코드는 전부 무시하고 다시 루프의 처음으로 돌아간다.
	if (it == _data.begin()) return -1; // no earlier date exists
	// DOC: If the date used in the input does not exist in your DB then you
	//      must use the closest date contained in your DB.
	// --it => why? according to the doc, it need to move back to the nearest past date
	//         e.g, 2026-01-05 => --it => 2026-01-03
	--it;
	return it->second; // returns value
}

// ==================== PUBLIC MEMBER FUNCTION ====================
// Reads data.csv and input.txt, extract necessary values, performs calculations,
// and prints to the console.
void BitcoinExchange::execute(const std::string& filename) {
	std::ifstream input_txt(filename.c_str()); // for is_open()
	if (!input_txt.is_open()) {
		std::cerr << "Error: could not open input.txt" << std::endl;
		return;
	}

	std::string line;
	std::getline(input_txt, line); // skips the header: "date | value"

	while (std::getline(input_txt, line)) {
		size_t delim = line.find(" | "); // includes spaces
		if (delim == std::string::npos) { // if '|' not found
			std::cout << "Error: bad input => " << line << std::endl;
			// continue: skips the code below and go to the beginning of the next loop
			continue;
		}

		// Before: "2011-01-03 | 3" => After: "2011-01-03"
		std::string date = line.substr(0, delim);
		// now, in the line, only " | 3" left by the previous code
		// substr(delim + 3)
		float value = static_cast<float>(atof(line.substr(delim + 3).c_str()));

		if (!isValidDate(date)) {
			std::cout << "Error: bad input => " << date << std::endl;
			continue;
		}

		if (!isValidValue(value)) {
			continue;
		}

		float rate = getExchangeRate(date);
		if (rate == -1) { // no earlier date exists when the date is not found in the database
			std::cout << "Error: no historical data => " << date << std::endl;
		} else { // This is Normal operation
			std::cout << date << " => " << value << " = " << (value * rate) << std::endl;
		}
	}
}
