#pragma once

#include <string>

using IntLabelPair = std::pair<int, std::string>;

struct Translation {
	std::string hex;
	std::string qid;
	std::string name; // english ver
};

struct GmkParam {
	std::string title;
	std::string type;
	int slot;
	std::string description;
	std::vector<IntLabelPair> int_dropdown;
};