#include "quilt.h"

extern Camera camera;

Editor::Editor() { 
	window		= nullptr;
	running		= false;
	open		= false;
	saved		= false;
	dockSetup	= false;
	font = nullptr;

	renderWalls = true;
	renderActionPoints = true;
	renderPaths = true;
	renderGimmicks = true;
	renderControllers = true;
	renderEnemies = true;
	renderGrid = false;

	selectedNode = nullptr;

	cachedParamName = "";

	parameterXMLContents = "";
	translationXMLContents = "";

	selectedFileIndex = 0;
}

Editor::~Editor() {
	ClearMapdata();
	ClearNodes();
	translations.clear();
	commonGimmickNames.clear();
	rawCommonGimmickNames.clear();
}


bool Editor::Setup() {
	// setup glfw
	glfwInit();

	// we're using OpenGL 3.3 CORE
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// the size the window starts out with doesnt really matter since it'll be maximised
	window = glfwCreateWindow(800, 800, "Quilt", NULL, NULL);
	if (!window) {
		printf("Error - failed to create GLFW window\n");
		glfwTerminate();
		return false;
	}

	// introduce window to current context
	glfwMakeContextCurrent(window);

	// configure OpenGL with GLAD
	gladLoadGL();
	glViewport(0, 0, 800, 800);
	glfwMaximizeWindow(window);

	// init ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	
	// add default font
	io.Fonts->AddFontDefault();
	// add consolas
	font = io.Fonts->AddFontFromFileTTF("res/font/Consolas.ttf", FONT_SIZE, NULL, io.Fonts->GetGlyphRangesDefault());

	// using the docking branch
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	LoadTranslations();
	LoadParameters();

	running = true;
	return true;
}

void Editor::Cleanup() {
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void Editor::ClearMapdata() {
	walls.clear();
	dataSegLabels.clear();
	paths.clear();
	controllers.clear();
	for (auto line : lines) { line.clear(); }
	lines.clear();
	commonGimmicks.clear();
	gimmicks.clear();
	enemies.clear();
	rawCommonGimmickNames.clear();
}

void Editor::Run() {
	while (running && !glfwWindowShouldClose(window)) {
		// rendering
		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		if (font) ImGui::PushFont(font);

		HandleMenu();
		HandleTabs();

		if (font) ImGui::PopFont();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// swap buffers
		glfwSwapBuffers(window);
		// handle events
		glfwPollEvents();
	}
}



void Editor::RenderFile() {
	if (!open) return;

	/* render walls */
	if (renderWalls) {
		for (int i = 0; i < walls.size(); i++) {
			float x1 = SwapF32(walls[i].point1.x) * camera.zoom + camera.x;
			float y1 = camera.h - (SwapF32(walls[i].point1.y) * camera.zoom) + camera.y;

			float x2 = SwapF32(walls[i].point2.x) * camera.zoom + camera.x;
			float y2 = camera.h - (SwapF32(walls[i].point2.y) * camera.zoom) + camera.y;

			DrawLine(Vec2f(x1, y1), Vec2f(x2, y2), WALL_COLOR);
		}
	}

	/* render data segs */
	if (renderActionPoints) {
		for (int i = 0; i < dataSegLabels.size(); i++) {
			float x1 = SwapF32(dataSegLabels[i].point1.x) * camera.zoom + camera.x;
			float y1 = camera.h - (SwapF32(dataSegLabels[i].point1.y) * camera.zoom) + camera.y;

			float x2 = SwapF32(dataSegLabels[i].point2.x) * camera.zoom + camera.x;
			float y2 = camera.h - (SwapF32(dataSegLabels[i].point2.y) * camera.zoom) + camera.y;

			DrawLine(Vec2f(x1, y1), Vec2f(x2, y2), RGBA(0xFF0000FF));
		}
	}

	/* render paths */
	if (renderPaths) {
		for (int i = 0; i < paths.size(); i++) {
			for (int j = 0; j < lines[i].size() - 1; j++) {
				float x1 = SwapF32(lines[i][j].x) * camera.zoom + camera.x;
				float y1 = camera.h - (SwapF32(lines[i][j].y) * camera.zoom) + camera.y;
				float x2 = SwapF32(lines[i][j + 1].x) * camera.zoom + camera.x;
				float y2 = camera.h - (SwapF32(lines[i][j + 1].y) * camera.zoom) + camera.y;

				DrawLine(Vec2f(x1, y1), Vec2f(x2, y2), PATH_COLOR);
			}
		}
	}
}

void Editor::UpdateNodes() {
	/* gimmick nodes*/
	if (renderGimmicks) {
		for (int i = 0; i < gmkNodes.size(); i++) {
			GmkNode* node = &gmkNodes[i];
			node->Update();
			if (node->RectClick()) SetSelectedNode(node);
		}
	}

	/* enemy nodes */
	if (renderEnemies) {
		for (int i = 0; i < enNodes.size(); i++) {
			EnNode* node = &enNodes[i];
			node->Update();
			if (node->RectClick()) SetSelectedNode(node);
		}
	}

	/* controller nodes */
	if (renderControllers) {
		for (int i = 0; i < contNodes.size(); i++) {
			ContNode* node = &contNodes[i];
			node->Update();
			if (node->RectClick()) SetSelectedNode(node);
		}
	}

	/* common gimmick nodes */
	if (renderGimmicks) {
		for (int i = 0; i < cmnGmkNodes.size(); i++) {
			CmnGmkNode* node = &cmnGmkNodes[i];
			node->Update();
			if (node->RectClick()) SetSelectedNode(node);
		}
	}
}

void Editor::SetSelectedNode(NodeBase* node) {
	if (!node) return;
	ClearSelectedNode();
	selectedNode = node;
	selectedNode->isSelected = true;
	selectedNode->paramsShown = false;
}

void Editor::ClearSelectedNode() {
	if (!selectedNode) return;
	selectedNode->isSelected = false;
	selectedNode->paramsShown = false;
	selectedNode = nullptr;
}

NodeBase* Editor::GetSelectedNode() { return selectedNode;  }

void Editor::ClearNodes() {
	gmkNodes.clear();
	enNodes.clear();
	contNodes.clear();
	cmnGmkNodes.clear();
}

void Editor::LoadParameters() {
	if (parameterXMLContents.empty()) {
		// read file into string
		std::ifstream file("res/parameters.xml");
		if (!file.is_open()) {
			printf("Error - failed to load parameters.xml!\n");
			return;
		}
		std::stringstream buf;
		buf << file.rdbuf();
		file.close();
		parameterXMLContents = buf.str();

		gmkParams.clear();
		contParams.clear();
		cmnGmkParams.clear();
		cachedParamName.clear();
	}

	try {
		// parse XML
		rapidxml::xml_document<> doc;
		doc.parse<0>(&parameterXMLContents[0]);

		// get root node
		rapidxml::xml_node<>* root = doc.first_node("root");
		if (!root) {
			printf("Error - root node 'root' not found!\n");
			return;
		}

		// load gimmicks
		rapidxml::xml_node<>* gimmicks_node = root->first_node("gimmicks");
		if (gimmicks_node) {
			for (rapidxml::xml_node<>* gimmick_node = gimmicks_node->first_node("gimmick"); gimmick_node; gimmick_node = gimmick_node->next_sibling("gimmick")) {
				std::string gimmick_name = gimmick_node->first_node("name")->value();
				std::vector<GmkParamInfo> params;

				rapidxml::xml_node<>* parameters_node = gimmick_node->first_node("parameters");
				if (parameters_node) {
					for (rapidxml::xml_node<>* parameter_node = parameters_node->first_node("parameter"); parameter_node; parameter_node = parameter_node->next_sibling("parameter")) {
						GmkParamInfo info;
						info.title = parameter_node->first_node("title")->value();
						info.data_type = parameter_node->first_node("data_type")->value();
						info.slot = std::stoi(parameter_node->first_node("slot")->value());
						info.description = parameter_node->first_node("description")->value();

						if ("dropdown_int" == info.data_type) {
							rapidxml::xml_node<>* options_node = parameter_node->first_node("options");
							if (options_node) {
								for (rapidxml::xml_node<>* option_node = options_node->first_node("option"); option_node; option_node = option_node->next_sibling("option")) {
									int value = std::stoi(option_node->first_node("value")->value());
									std::string label = option_node->first_node("label")->value();
									info.dropdown_options.emplace_back(value, label);
								}
							}
						}
						params.push_back(info);
					}
				}
				gmkParams.emplace_back(gimmick_name, params);
			}
		}

		// load controllers
		rapidxml::xml_node<>* controllers_node = root->first_node("controllers");
		if (controllers_node) {
			for (rapidxml::xml_node<>* controller_node = controllers_node->first_node("controller"); controller_node; controller_node = controller_node->next_sibling("controller")) {
				std::string controller_name = controller_node->first_node("name")->value();
				std::vector<GmkParamInfo> params;

				rapidxml::xml_node<>* parameters_node = controller_node->first_node("parameters");
				if (parameters_node) {
					for (rapidxml::xml_node<>* parameter_node = parameters_node->first_node("parameter"); parameter_node; parameter_node = parameter_node->next_sibling("parameter")) {
						GmkParamInfo info;
						info.title = parameter_node->first_node("title")->value();
						info.data_type = parameter_node->first_node("data_type")->value();
						info.slot = std::stoi(parameter_node->first_node("slot")->value());
						info.description = parameter_node->first_node("description")->value();

						if ("dropdown_int" == info.data_type) {
							rapidxml::xml_node<>* options_node = parameter_node->first_node("options");
							if (options_node) {
								for (rapidxml::xml_node<>* option_node = options_node->first_node("option"); option_node; option_node = option_node->next_sibling("option")) {
									int value = std::stoi(option_node->first_node("value")->value());
									std::string label = option_node->first_node("label")->value();
									info.dropdown_options.emplace_back(value, label);
								}
							}
						}

						params.push_back(info);
					}
				}
				contParams.emplace_back(controller_name, params);
			}
		}

		// load common gimmicks
		rapidxml::xml_node<>* common_gimmicks_node = root->first_node("common_gimmicks");
		if (common_gimmicks_node) {
			for (rapidxml::xml_node<>* common_gimmick_node = common_gimmicks_node->first_node("common_gimmick"); common_gimmick_node; common_gimmick_node = common_gimmick_node->next_sibling("common_gimmick")) {
				std::string quilt_identifier = common_gimmick_node->first_node("quilt_identifier")->value();
				if (!GetTranslationByQID(quilt_identifier)) {
					printf("Warning: Quilt ID '%s' not found in translations.xml. Skipping...\n", quilt_identifier.c_str());
					continue;
				}

				std::vector<CmnGmkParamInfo> params;

				rapidxml::xml_node<>* parameters_node = common_gimmick_node->first_node("parameters");
				if (parameters_node) {
					for (rapidxml::xml_node<>* parameter_node = parameters_node->first_node("parameter"); parameter_node; parameter_node = parameter_node->next_sibling("parameter")) {
						CmnGmkParamInfo info;
						info.quilt_identifier = quilt_identifier;
						info.title = parameter_node->first_node("title")->value();
						info.data_type = parameter_node->first_node("data_type")->value();
						info.slot = std::stoi(parameter_node->first_node("slot")->value());
						info.description = parameter_node->first_node("description")->value();

						if ("dropdown_int" == info.data_type) {
							rapidxml::xml_node<>* options_node = parameter_node->first_node("options");
							if (options_node) {
								for (rapidxml::xml_node<>* option_node = options_node->first_node("option"); option_node; option_node = option_node->next_sibling("option")) {
									int value = std::stoi(option_node->first_node("value")->value());
									std::string label = option_node->first_node("label")->value();
									info.dropdown_options.emplace_back(value, label);
								}
							}
						}

						params.push_back(info);
					}
				}
				cmnGmkParams.emplace_back(quilt_identifier, params);
			}
		}
	}
	catch (const std::exception& e) {
		printf("Error loading parameters: %s\n", e.what());
		return;
	}
}


void Editor::ReloadParameters() {
	parameterXMLContents.clear();
	LoadParameters();
}

void Editor::LoadTranslations() {
	if (translationXMLContents.empty()) {
		// read file into string
		std::ifstream file("res/translations.xml");
		if (!file.is_open()) {
			printf("Error - failed to load translations.xml!\n");
			return;
		}

		std::stringstream buf;
		buf << file.rdbuf();
		file.close();
		translationXMLContents = buf.str();

		translations.clear();
	}

	try {
		// parse xml
		rapidxml::xml_document<> doc;
		doc.parse<0>(&translationXMLContents[0]);
		
		// get root node
		rapidxml::xml_node<>* root = doc.first_node("root");
		if (!root) {
			printf("Error - root node 'root' not found!\n");
			return;
		}

		// load translations
		for (rapidxml::xml_node<>* translation_node = root->first_node("translation"); translation_node; translation_node = translation_node->next_sibling("translation")) {
			Translation translation;

			translation.shift_jis_bytes = translation_node->first_node("shift_jis_bytes")->value();
			if (translation.shift_jis_bytes.empty()) continue;
			std::string::iterator end_pos = std::remove(translation.shift_jis_bytes.begin(), translation.shift_jis_bytes.end(), ' ');
			translation.shift_jis_bytes.erase(end_pos, translation.shift_jis_bytes.end());
			if (GetTranslationByHex(translation.shift_jis_bytes)) continue;
			translation.english_string = translation_node->first_node("english_string")->value();
			translation.quilt_identifier = translation_node->first_node("quilt_identifier")->value();

			translations[translation.shift_jis_bytes] = translation;
		}

	}
	catch (const std::exception& e) {
		printf("Error loading translations: %s\n", e.what());
		return;
	}
}

void Editor::ReloadTranslations() {
	translationXMLContents.clear();
	LoadTranslations();
	SetupCommonGimmickNames();

	for (int i = 0; i < cmnGmkNodes.size(); i++) {
		CmnGmkNode* node = &cmnGmkNodes[i];
		node->name = commonGimmickNames[node->nameIndex];
	}
}

Translation* Editor::GetTranslationByHex(std::string hex) {
	Translation* result = nullptr;
	
	auto it = translations.find(hex);
	if (it != translations.end()) {
		result = &it->second;
	}

	return result;
}

Translation* Editor::GetTranslationByQID(std::string qid) {
	for (auto& pair : translations) {
		if (pair.second.quilt_identifier == qid) {
			return &pair.second;
		}
	}
	return nullptr;
}

std::string Editor::GetQIDByName(const std::string& name) {
	std::string search_name = name;

	if ("0x" == name.substr(0, 2)) search_name = name.substr(2);

	for (const auto& pair : translations) {
		const Translation& translation = pair.second;

		if (pair.first == search_name || translation.english_string == search_name) {
			return translation.quilt_identifier;
		}

	}

	return "";
}

std::vector<char> Editor::HexToBytes(std::string hex) {
	std::vector<char> bytes;

	for (auto i = 0; i < hex.length(); i++) {
		std::string byteString = hex.substr(i, 2);
		char byte = (char)strtol(byteString.c_str(), NULL, 16);
		bytes.push_back(byte);
	}

	return bytes;
}

std::string Editor::BytesToHex(char* bytes, size_t len) {
	std::stringstream hexStream;

	for (auto i = 0; i < len; i++) {
		hexStream << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << (static_cast<unsigned int>(bytes[i]) & 0xFF);
	}

	return hexStream.str();
}


void Editor::SetupCommonGimmickNames() {
	commonGimmickNames.clear();
	// i'm using an unordered set here because
	// the original string order MUST be preserved
	std::unordered_set<std::string> unique_names;

	for (const auto& common : commonGimmicks) {
		// get translation for the current common gimmick
		int index = Swap32(common.nameIndex);
		int count = rawCommonGimmickNames.size();

		if (index < 0 || index >= count) continue;

		std::string raw = rawCommonGimmickNames[index];
		std::string hex = BytesToHex(raw.data(), raw.length());
		Translation* translation = GetTranslationByHex(hex);

		std::string name;
		// check if there's a translation entry AND an english translation
		if (translation && !translation->english_string.empty()) {
			name = translation->english_string;
		}
		else {
			// just display the raw bytes
			name = "0x" + hex;
		}

		// add name to the vector if it is unique
		if (unique_names.find(name) == unique_names.end()) {
			unique_names.insert(name);
			commonGimmickNames.push_back(name);
		}
	}
}
