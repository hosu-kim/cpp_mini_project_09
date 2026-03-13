/*
This exercise is about developing a program that calculates the value
of Bitcoin on specific dates by parsing a CSV database and an input.txt,
utilizing `std::map` to handle data storage and search.
*/

#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <map> // std::map
# include <string> // substr(), find(), string::npos, length(), size()...
# include <iostream>
# include <fstream>
# include <sstream>
# include <algorithm>

class BitcoinExchange {
	private:
		// std::map: Associative(연관) container; stores key-value pairs.
		//           Methods
		//           1. data["dataKey"] = dataValue;
		//           2. data.insert(std::make_pair(dataKey, dataValue));
		std::map<std::string, float> _data;

		bool isValidDate(const std::string& date) const;
		bool isValidValue(const float value) const;
		float getExchangeRate(const std::string& date);

	public:
		// Orthodox Canonical Form
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange& other);
		BitcoinExchange& operator=(const BitcoinExchange& other);
		~BitcoinExchange();
		// Member function
		void execute(const std::string& filename);
};

#endif