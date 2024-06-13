#include "quilt.h"

extern Camera camera;

Editor::Editor() { 
	window		= nullptr;
	running		= false;
	open		= false;
	saved		= false;
	dockSetup	= false;

	renderWalls = true;
	renderPaths = true;
	renderGimmicks = true;
	renderEnemies = true;
	renderGrid = false;

	selectedNode = nullptr;

	cachedParamName = "";

	selectedFileIndex = 0;
}

Editor::~Editor() {
	ClearMapdata();

	cachedParams.clear();
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

	glfwSetScrollCallback(window, Editor::ScrollCallback);

	// init ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	
	// using the docking branch
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");


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

void Editor::Run() {
	static bool first_time = true;
	while (running && !glfwWindowShouldClose(window)) {
		// editor
		UpdateCamera();

		// rendering
		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);



		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		

		HandleMenu();

		HandleTabs();

		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// swap buffers
		glfwSwapBuffers(window);
		// handle events
		glfwPollEvents();
	}
}


void Editor::SetupFile() {
	if (selectedFile.empty()) {
		return;
	}

	std::string filename = folderPath + selectedFile + ".mapbin";
	
	open = true;

	gmkNodes.clear();
	ClearSelectedNode();
	ClearMapdata();
	ClearNodes();


	std::fstream file(filename , std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		printf("Warning - failed to open file %s. The file's contents will not be rendered.\n", filename.c_str());
		return;
	}

	Mapdata::Mapbin::Header mapHeader;
	file.read((char*)&mapHeader, sizeof(mapHeader));

	/* walls */
	file.seekg(Swap32(mapHeader.wallOffset));
	for (int i = 0; i < Swap32(mapHeader.wallCount); i++) {
		Colbin::Entry entry;
		file.read((char*)&entry, sizeof(entry));
		walls.push_back(entry);
	}

	/* gimmicks */
	file.seekg(Swap32(mapHeader.gimmickOffset));
	
	for (int i = 0; i < Swap32(mapHeader.gimmickCount); i++) {
		Mapdata::Mapbin::Gimmick gimmick;
		file.read((char*)&gimmick, sizeof(gimmick));
		gimmicks.push_back(gimmick);

		GmkNode node;
		node.Configure(&gimmick);
		gmkNodes.push_back(node);
	}

	/* paths */
	file.seekg(Swap32(mapHeader.pathOffset));

	for (int i = 0; i < Swap32(mapHeader.pathCount); i++) {
		Mapdata::Mapbin::Path path;
		file.read((char*)&path, sizeof(path));
		std::vector<Vec2f> line;

		for (int j = 0; j < Swap32(path.numPoints); j++) {
			Vec2f point;
			file.read((char*)&point, sizeof(point));
			line.push_back(point);
		}

		paths.push_back(path);
		lines.push_back(line);
	}

	file.close();

	filename = folderPath + selectedFile + ".enbin";
	file.open(filename, std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		printf("Warning - failed to open file %s. The file's contents will not be rendered.\n", filename.c_str());
		return;
	}
	Mapdata::Enbin::Header enHeader;

	file.read((char*)&enHeader, sizeof(enHeader));

	// there doesnt seem to be an offset defined by the header
	file.seekg(sizeof(enHeader));

	for (int i = 0; i < Swap32(enHeader.numEntries); i++) {
		Mapdata::Enbin::EnemyEntry enemy;
		file.read((char*)&enemy, sizeof(enemy));
		enemies.push_back(enemy);

		EnNode node;
		node.Configure(&enemy);
		enNodes.push_back(node);
	}

	

	file.close();
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

void Editor::ClearMapdata() {
	walls.clear();
	paths.clear();
	for (auto line : lines) { line.clear(); }
	lines.clear();
	gimmicks.clear();
	enemies.clear();
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
}

void Editor::SetSelectedNode(NodeBase* node) {
	if (!node) return;
	ClearSelectedNode();
	selectedNode = node;
	selectedNode->isSelected = true;
}

void Editor::ClearSelectedNode() {
	if (!selectedNode) return;
	selectedNode->isSelected = false;
	selectedNode = nullptr;
}

NodeBase* Editor::GetSelectedNode() { return selectedNode;  }

void Editor::ClearNodes() {
	gmkNodes.clear();
	enNodes.clear();
}

void Editor::SaveFile() {


	
}