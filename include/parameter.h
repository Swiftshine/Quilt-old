#pragma once

#include <string>
#include <vector>

struct Translation {
	std::string quilt_identifier;
	std::string shift_jis_bytes;
	std::string english_string;
};

struct GmkParamInfo {
	GmkParamInfo() { slot = -1;  }
	std::string title;
	std::string data_type;
	int slot;
	std::string description;
	std::vector<std::pair<int, std::string>> dropdown_options;
};

struct CmnGmkParamInfo {
	CmnGmkParamInfo() { slot = -1; }
	std::string quilt_identifier;
	std::string title;
	std::string data_type;
	int slot;
	std::string description;
	std::vector<std::pair<int, std::string>> dropdown_options;
};