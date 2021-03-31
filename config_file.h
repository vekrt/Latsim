#ifndef CONFIG_FILE_CLASS_H
#define CONFIG_FILE_CLASS_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>

typedef std::pair<std::string, std::string> key_value;

class Config_file {
	public:
		Config_file() = default;
		Config_file(const std::string& filename);
	
		template<typename T> T get(const std::string& key) const;
		void print() const;
		std::ostream& show(std::ostream& out) const;		
		void insert(const std::pair<std::string, std::string>& param);
		void read_from_file(const std::string& filename);
	private:
		std::map<std::string, std::string> parameters;
};

key_value split_on(const std::string& str, const char& delim); 
std::string trim(const std::string& str);
std::string uncomment(const std::string& str);

std::ostream& operator<<(std::ostream& out, const Config_file& config_file);

#endif //CONFIG_FILE_CLASS_H
