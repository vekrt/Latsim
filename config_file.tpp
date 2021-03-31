#include "config_file.h"
#include <algorithm>
#include <utility>
#include <iomanip>

Config_file::Config_file(const std::string& filename) {
	read_from_file(filename);
}

void Config_file::read_from_file(const std::string& filename) {
	std::ifstream file(filename);

	if (file.fail()) {
		throw std::invalid_argument("[Config_file] Error cannot open the configuration file");
	}
	std::string line;
	while (getline(file, line)) {
		key_value kv = split_on(line, '=');
		this -> insert(kv);
	}
}

template<typename T> 
T Config_file::get(const std::string& key) const {
	auto kv = parameters.find(key);

	if (kv == parameters.end())
		throw std::out_of_range("[Config_file] Key not found \"" + key + "\"");
	
	T res;
	std::istringstream iss(kv -> second);
	if (!(iss >> std::setprecision(9) >> res))
		throw std::invalid_argument("Cannot convert the value \"" + kv -> second + "\"");

	return res;
}
		
void Config_file::print() const {
	std::cout << "Parameters: " << std::endl;
	for (auto const& [k, v] : parameters)
		std::cout << std::setw(10) << k << ": " << v << std::endl;
}

std::ostream& Config_file::show(std::ostream& out) const {
	for (auto const& [k, v] : parameters)
		out << std::setw(10) << k << ": " << v << std::endl;

	return out;
}

void Config_file::insert(const std::pair<std::string, std::string>& param) {
	auto kv = parameters.find(param.first);

	if (kv == parameters.end())
		parameters.insert(param);
	else {
		std::cout << "[Config_file] Overwriting key " + param.first + ": " + param.second
			  << std::endl;
		parameters.erase(kv);
		parameters.insert(param);
	}
}

key_value split_on(const std::string& str, const char& delim) {
	std::istringstream iss(str);
	std::string token;
	getline(iss, token, delim);
	const std::string key = trim(token);
	getline(iss, token, delim);
	const std::string value = uncomment(trim(token));
	
	return std::make_pair(key, value);
}

std::string trim(const std::string& input) {
	std::string str = input;
	str.erase(str.begin(), 
		  std::find_if(str.begin(), str.end(), [](const char& ch) {return !std::isspace(ch);}));
	str.erase(std::find_if(str.rbegin(), str.rend(), [](const char& ch) {return !std::isspace(ch);}).base(),
		  str.end());
	
	return str;
}

std::string uncomment(const std::string& input) {
	std::string str = input;

	return str.substr(0, str.find('#'));
}

std::ostream& operator<<(std::ostream& out, const Config_file& config_file) {
	return config_file.show(out);
}
