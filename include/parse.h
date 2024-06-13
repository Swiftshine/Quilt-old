#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

#include "types.h"

namespace fs = std::filesystem;

struct GMKParameter {
	std::string description;
	std::string name;
	std::string dataType;
	int slot = 1;
};

struct ENParameter {
	std::string description;
	std::string name;
	std::string dataType;
	// todo - finish this
};

class Parser {
public:
	Parser();
	~Parser();

	std::vector<GMKParameter>* OpenGMK(std::string filename);
	// std::vector<ENParameter>* OpenEN(std::string filename);
	void Close();

private:
	std::ifstream file;
	bool valid;
	std::vector<GMKParameter> gmkParams;
};

extern Parser parser;