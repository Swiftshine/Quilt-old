#include "parse.h"
#include "quilt.h"

Parser parser = Parser();

Parser::Parser() {
	gmkParams.clear();
	valid = false;
}

Parser::~Parser() { Close();  }


// trims whitespace from either side of a string
std::string trim(const std::string& str) {
	size_t first = str.find_first_not_of(" \n\r\t");
	size_t last = str.find_last_not_of(" \n\r\t");
	return (first == std::string::npos || last == std::string::npos) ? "" : str.substr(first, last - first + 1);
}

std::vector<GMKParameter>* Parser::OpenGMK(std::string name) {
	Close();

	std::string filepath = "res/gimmick_setting/" + name + ".gmk";
	if (!fs::exists(filepath)) return nullptr;

	std::ifstream file(filepath);
	std::string line;
	std::string currentDescription = "'";

	while (std::getline(file, line)) {
		try {
			// ignore comments and empty lines
			if (line.empty() || '#' == line[0]) {
				continue;
			}



			// get descriptions if they exist
			if ('$' == line[0]) {
				currentDescription = line.substr(1);
				if (!currentDescription.empty() && currentDescription[0] == ' ') {
					currentDescription = currentDescription.substr(1);
				}
				continue;
			}


			std::istringstream iss(line);
			GMKParameter param;

			param.description = currentDescription;
			currentDescription.clear();

			std::string slotStr;
			std::getline(iss, param.name, ':');
			std::getline(iss, param.dataType, ':');
			std::getline(iss, slotStr);

			param.name = trim(param.name);
			param.dataType = trim(param.dataType);
			param.slot = std::stoi(trim(slotStr));

			gmkParams.push_back(param);

		} catch (int exception) {
			Close();
			return nullptr;
		}
	}

	valid = true;
	return &gmkParams;
}

void Parser::Close() { valid = false; file.close(); gmkParams.clear();  }
