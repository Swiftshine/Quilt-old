#pragma once

#include <string>

struct Translation {
	std::string shift_jis_bytes;
	std::string quilt_name;
	std::string utf8_string;
	std::string english_string;
	std::string comment;
	std::string parameter_resource;
};

struct GmkParamInfo {
	GmkParamInfo() { title = ""; data_type = ""; slot = -1; description = "";  }
	std::string title;
	std::string data_type;
	int slot;
	std::string description;
};
