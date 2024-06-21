#include "editor.h"

void Editor::LoadParams() {
	std::ifstream file("res/param/parameter.xml");
	if (!file.is_open()) {
		Quilt::Debug::Error("Failed to load parameter.xml!");
		return;
	}

	param_xml_contents.clear();
	params_general.clear();

	std::stringstream buf;
	buf << file.rdbuf();
	file.close();

	param_xml_contents = buf.str();

	// parse xml

	rapidxml::xml_document<>* doc = new rapidxml::xml_document<>;

	if (!doc) {
		Quilt::Debug::Error("Failed to create rapidxml doc.");
		return;
	}

	try {
		doc->parse<0>(param_xml_contents.data());

		// get root node
		rapidxml::xml_node<>* root = doc->first_node("root");

		if (!root) {
			Quilt::Debug::Error("Root node 'root' not found!");
			delete doc;
			return;
		}

		// go through gimmicks

		for (rapidxml::xml_node<>* gmk_xml_node = root->first_node("gimmick"); gmk_xml_node; gmk_xml_node = gmk_xml_node->next_sibling("gimmick")) {
			std::string gmk_name = gmk_xml_node->first_attribute("name")->value();
			std::string gmk_note = gmk_xml_node->first_attribute("note")->value();

			NameNotePair string_pair = std::make_pair(gmk_name, gmk_note);

			std::vector<GmkParam> params;
			
			for (rapidxml::xml_node<>* param_xml_node = gmk_xml_node->first_node("parameter"); param_xml_node; param_xml_node = param_xml_node->next_sibling("parameter")) {
				GmkParam param;
				param.title = param_xml_node->first_attribute("title")->value();
				param.type = param_xml_node->first_attribute("type")->value();
				param.slot = std::stoi(param_xml_node->first_attribute("slot")->value());
				param.description = param_xml_node->first_attribute("description")->value();

				if ("dropdown_int" == param.type) {
					rapidxml::xml_node<>* dropdown_node = param_xml_node->first_node("dropdown");

					for (rapidxml::xml_node<>* item = dropdown_node->first_node("item"); item; item = item->next_sibling("item")) {
						int val = std::stoi(item->first_attribute("value")->value());
						std::string label = item->first_attribute("label")->value();
						param.int_dropdown.emplace_back(val, label);
					}
				}

				params.push_back(param);
			}
			params_general.emplace_back(string_pair, params);
		}

		// go through zones
		for (rapidxml::xml_node<>* zone_xml_node = root->first_node("zone"); zone_xml_node; zone_xml_node = zone_xml_node->next_sibling("zone")) {
			std::string zone_name = zone_xml_node->first_attribute("name")->value();
			std::string zone_note = zone_xml_node->first_attribute("note")->value();

			NameNotePair string_pair = std::make_pair(zone_name, zone_note);

			std::vector<GmkParam> params;

			for (rapidxml::xml_node<>* param_xml_node = zone_xml_node->first_node("parameter"); param_xml_node; param_xml_node = param_xml_node->next_sibling("parameter")) {
				GmkParam param;
				param.title = param_xml_node->first_attribute("title")->value();
				param.type = param_xml_node->first_attribute("type")->value();
				param.slot = std::stoi(param_xml_node->first_attribute("slot")->value());
				param.description = param_xml_node->first_attribute("description")->value();

				if ("dropdown_int" == param.type) {
					rapidxml::xml_node<>* dropdown_node = param_xml_node->first_node("dropdown");

					for (rapidxml::xml_node<>* item = dropdown_node->first_node("item"); item; item = item->next_sibling("item")) {
						int val = std::stoi(item->first_attribute("value")->value());
						std::string label = item->first_attribute("label")->value();
						param.int_dropdown.emplace_back(val, label);
					}
				}

				params.push_back(param);
			}
			params_general.emplace_back(string_pair, params);
		}

		// go through common gimmicks
		for (rapidxml::xml_node<>* cmnGmk_xml_node = root->first_node("common"); cmnGmk_xml_node; cmnGmk_xml_node = cmnGmk_xml_node->next_sibling("common")) {
			std::string qid = cmnGmk_xml_node->first_attribute("qid")->value();
			std::string note = cmnGmk_xml_node->first_attribute("note")->value();

			QIDNotePair string_pair = std::make_pair(qid, note);

			std::vector<GmkParam> params;

			for (rapidxml::xml_node<>* param_xml_node = cmnGmk_xml_node->first_node("parameter"); param_xml_node; param_xml_node = param_xml_node->next_sibling("parameter")) {
				GmkParam param;
				param.title = param_xml_node->first_attribute("title")->value();
				param.type = param_xml_node->first_attribute("type")->value();
				param.slot = std::stoi(param_xml_node->first_attribute("slot")->value());
				param.description = param_xml_node->first_attribute("description")->value();

				if ("dropdown_int" == param.type) {
					rapidxml::xml_node<>* dropdown_node = param_xml_node->first_node("dropdown");

					for (rapidxml::xml_node<>* item = dropdown_node->first_node("item"); item; item = item->next_sibling("item")) {
						int val = std::stoi(item->first_attribute("value")->value());
						std::string label = item->first_attribute("label")->value();
						param.int_dropdown.emplace_back(val, label);
					}
				}

				params.push_back(param);
			}
			params_cmnGmk.emplace_back(string_pair, params);
		}

		delete doc;
	}
	catch (std::exception& e) {
		Quilt::Debug::Exception(e.what());
		delete doc;
	}
}

void Editor::LoadTranslations() {
	std::ifstream file("res/param/translate.xml");
	if (!file.is_open()) {
		Quilt::Debug::Error("Failed to load translate.xml!");
		return;
	}


	translation_xml_contents.clear();
	translations.clear();

	std::stringstream buf;
	buf << file.rdbuf();
	file.close();

	translation_xml_contents = buf.str();

	// parse xml


	rapidxml::xml_document<>* doc = new rapidxml::xml_document<>;
	if (!doc) {
		Quilt::Debug::Error("Failed to create rapidxml doc.");
		return;
	}

	try {
		doc->parse<0>(translation_xml_contents.data());

		// get root node
		rapidxml::xml_node<>* root = doc->first_node("root");

		if (!root) {
			Quilt::Debug::Error("Root node 'root' not found!");
			delete doc;
			return;
		}
		// go through translations

		for (rapidxml::xml_node<>* trans_xml_node = root->first_node("translation"); trans_xml_node; trans_xml_node = trans_xml_node->next_sibling("translation")) {
			Translation translation;
			translation.qid = trans_xml_node->first_attribute("qid")->value();
			if (translation.qid.empty()) continue;
			translation.hex = trans_xml_node->first_attribute("bytes")->value();
			std::string::iterator end_pos = std::remove(translation.hex.begin(), translation.hex.end(), ' ');
			translation.hex.erase(end_pos, translation.hex.end());
			translation.name = trans_xml_node->first_attribute("name")->value();
			translations.push_back(translation);
		}

		delete doc;
	}
	catch (std::exception& e) {
		Quilt::Debug::Exception(e.what());
		delete doc;
	}
}

void Editor::ClearParams() {
	params_general.clear();
	params_cmnGmk.clear();
}


void Editor::Param_CommonGimmick() {
	ImGui::Begin("Common Gimmick Parameters");
	CmnGmkNode* node = reinterpret_cast<CmnGmkNode*>(selected_node);

	int nIndex = Swap32(node->cmnGmk->nameIndex);
	std::string dummy = GetCommonGimmickNameFromIndex(nIndex);

	// todo - have some kind of window with every raw name to select from
	// (and a dropdown on THIS window to select from it)
	ImGui::InputText("Name", dummy.data(), 0x20);

	node->cmnGmk->position.Swap();
	ImGui::InputFloat("X Position", &node->cmnGmk->position.x, 0.1f, 1.0f);
	ImGui::InputFloat("Y Position", &node->cmnGmk->position.y, 0.1f, 1.0f);
	node->cmnGmk->position.Swap();

	// check for params
	bool found = false;

	std::vector<GmkParam> params;
	
	std::string qid = GetQIDFromIndex(nIndex);

	for (const auto& pair : params_cmnGmk) {
		if (pair.first.first != qid) continue;
		else {
			found = true;
			params = pair.second;
			break;
		}
	}

	if (!found) {
		ImGui::End();
		return;
	}

	for (const auto& param : params) {
		if ("string" == param.type) {
			ImGui::InputText(param.title.c_str(), node->cmnGmk->params.string1, 32);
			Quilt::Util::DrawTooltip(param.description);
		}

		else if ("dropdown_int" == param.type) {
			std::string cur_item;

			for (const auto& item : param.int_dropdown) {
				if (Swap32(node->cmnGmk->params.ints2[param.slot]) == item.first) {
					cur_item = item.second;
					break;
				}
			}

			if (ImGui::BeginCombo(param.title.c_str(), cur_item.c_str())) {
				for (const auto& item : param.int_dropdown) {
					bool selected = Swap32(node->cmnGmk->params.ints2[param.slot]) == item.first;

					if (ImGui::Selectable(item.second.c_str(), selected)) {
						node->cmnGmk->params.ints2[param.slot] = Swap32(item.first);
					}

					if (selected) ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();

				Quilt::Util::DrawTooltip(param.description);
			}
		}
	}
	ImGui::End();
}

void Editor::Param_Gimmick() {
	ImGui::Begin("Gimmick Parameters");
	GmkNode* node = reinterpret_cast<GmkNode*>(selected_node);
	ImGui::InputText("Name", node->gmk->name, 48);
	node->gmk->position.Swap();
	ImGui::InputFloat("X Position", &node->gmk->position.x, 0.1f, 1.0f);
	ImGui::InputFloat("Y Position", &node->gmk->position.y, 0.1f, 1.0f);
	node->gmk->position.Swap();


	// check for params
	bool found = false;

	std::vector<GmkParam> params;

	for (const auto& pair : params_general) {
		if (pair.first.first != std::string(node->gmk->name)) {
			continue;
		}
		else {
			found = true;
			params = pair.second;
			break;
		}
	}

	if (!found) {
		ImGui::End();
		return;
	}

	for (const auto& param : params) {
		if ("int" == param.type) {
			int val = Swap32(node->gmk->params.ints[param.slot]);
			ImGui::InputInt(param.title.c_str(), &val, 1, 5);
			node->gmk->params.ints[param.slot] = Swap32(val);

			Quilt::Util::DrawTooltip(param.description);
		}
		else if ("float" == param.type) {
			float val = SwapF32(node->gmk->params.floats[param.slot]);
			ImGui::InputFloat(param.title.c_str(), &val, 0.1f, 1.0f);
			node->gmk->params.floats[param.slot] = SwapF32(val);

			Quilt::Util::DrawTooltip(param.description);
		}
	}

	ImGui::End();
}


void Editor::Param_Zone() {
	ImGui::Begin("Zone Parameters");
	ZoneNode* node = reinterpret_cast<ZoneNode*>(selected_node);
	ImGui::InputText("Name", node->zone->name, 0x20);

	node->zone->boundsStart.Swap();
	node->zone->boundsEnd.Swap();

	ImGui::InputFloat("Start X", &node->zone->boundsStart.x, 0.1f, 1.0f);
	ImGui::InputFloat("Start Y", &node->zone->boundsStart.y, 0.1f, 1.0f);
	ImGui::InputFloat("End X", &node->zone->boundsEnd.x, 0.1f, 1.0f);
	ImGui::InputFloat("End Y", &node->zone->boundsEnd.y, 0.1f, 1.0f);

	node->zone->boundsStart.Swap();
	node->zone->boundsEnd.Swap();

	bool found = false;

	std::vector<GmkParam> params;

	for (const auto& pair : params_general) {
		if (pair.first.first != std::string(node->zone->name)) {
			continue;
		}
		else {
			found = true;
			params = pair.second;
			break;
		}
	}

	if (!found) {
		ImGui::End();
		return;
	}

	for (const auto& param : params) {
		if ("int" == param.type) {
			int val = Swap32(node->zone->params.ints[param.slot]);
			ImGui::InputInt(param.title.c_str(), &val, 1, 5);
			node->zone->params.ints[param.slot] = Swap32(val);

			Quilt::Util::DrawTooltip(param.description);
		}
		else if ("float" == param.type) {
			float val = SwapF32(node->zone->params.floats[param.slot]);
			ImGui::InputFloat(param.title.c_str(), &val, 0.1f, 1.0f);
			node->zone->params.floats[param.slot] = SwapF32(val);

			Quilt::Util::DrawTooltip(param.description);
		}
		else if ("string" == param.type) {
			ImGui::InputText(param.title.c_str(), node->zone->params.strings[param.slot], 0x20);
			Quilt::Util::DrawTooltip(param.description);
		}
		else if ("dropdown_int" == param.type) {
			std::string cur_item;

			for (const auto& item : param.int_dropdown) {
				if (Swap32(node->zone->params.ints[param.slot]) == item.first) {
					cur_item = item.second;
					break;
				}
			}

			if (ImGui::BeginCombo(param.title.c_str(), cur_item.c_str())) {
				for (const auto& item : param.int_dropdown) {
					bool selected = Swap32(node->zone->params.ints[param.slot]) == item.first;

					if (ImGui::Selectable(item.second.c_str(), selected)) {
						node->zone->params.ints[param.slot] = Swap32(item.first);
					}

					if (selected) ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();

				Quilt::Util::DrawTooltip(param.description);
			}
		}
	}

	ImGui::End();
}

std::string Editor::GetCommonGimmickNameFromIndex(size_t index) {
	if ((int)bytes_cmnGmk.size() - 1 <= (int)index) {
		// need to add one
		std::string name = query_selected_label;
		for (const auto& t : translations) {
			if (t.name == name) {
				bytes_cmnGmk.push_back(Quilt::Util::HexToBytes(t.hex));
			}
		}
	}
	
	for (const auto& t : translations) {
		std::string hex = Quilt::Util::BytesToHex(bytes_cmnGmk[index].data(), bytes_cmnGmk[index].length());
		if (t.hex == hex) {
			return t.name;
		}
	}
	
	// otherwise return the hex
	return Quilt::Util::BytesToHex(bytes_cmnGmk[index].data(), bytes_cmnGmk[index].length());
}

std::string Editor::GetQIDFromIndex(size_t index) {
	if ((int)bytes_cmnGmk.size() - 1 < (int)index) return "QID_NONE";
	for (const auto& translation : translations) {
		std::string hex = Quilt::Util::BytesToHex(bytes_cmnGmk[index].data(), bytes_cmnGmk[index].length());
		if (translation.hex == hex) {
			return translation.qid;
		}
	}

	// otherwise return the default
	return "QUILT_DEFAULT_QID";
}

size_t Editor::GetIndexFromHex(std::string hex) {
	std::string bytes = Quilt::Util::HexToBytes(hex);
	
	for (auto i = 0; i < bytes_cmnGmk.size(); i++) {
		if (bytes_cmnGmk[i] == bytes) {
			return i;
		}
	}

	// it needs to be added

	bytes_cmnGmk.push_back(bytes);
	return bytes_cmnGmk.size() - 1;
}

size_t Editor::GetIndexFromName(std::string name) {
	for (const auto& t : translations) {
		if (t.name == name) {
			return GetIndexFromHex(t.hex);
		}
	}

	// there isn't a translation entry
	// (this point in the function shouldn't be reached
	// at all because the names are being pulled from the
	// translation entries anyways)
	return 0;
}