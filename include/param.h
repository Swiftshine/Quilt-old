#pragma once

#include <string>

using IntLabelPair = std::pair<int, std::string>;

struct Translation {
	std::string hex;
	std::string qid;
	std::string name; // english ver
};

struct Param {
	std::string title;
	std::string owner_type;
	std::string data_type;
	int slot;
	std::string description;
	std::vector<IntLabelPair> int_dropdown;
};

struct EnemyTranslation {
	std::string id;
	std::string name;
	std::string note;
};