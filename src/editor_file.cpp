#include "editor.h"
#include "node.h"

void Editor::OpenFolder() {
	nfdchar_t* outpath = nullptr;
	nfdresult_t result = NFD_PickFolder(nullptr, &outpath);

	if (NFD_OKAY == result) {
		// picked a folder successfully
		ClearFilenames();
		folder_path = outpath;
		free(outpath);
	}
	else if (NFD_CANCEL == result) {
		// user cancelled
		free(outpath);
		return;
	}
	else {
		Quilt::Debug::Error("NFD failed to open a folder. Details: ", NFD_GetError());
		free(outpath);
		return;
	}
	

	selected_file_index = 0;
	selected_file = "";

	folder_name = folder_path.substr(folder_path.find_last_of("/\\") + 1);
	folder_path += '/';

	// we are using an ORDERED set
	std::set<std::string> unique;

	for (const auto& entry : fs::directory_iterator(folder_path)) {
		if (!entry.is_regular_file()) continue;

		fs::path path = entry.path();

		std::string extension = path.extension().string();

		if (!(".mapbin" == extension || ".enbin" == extension)) continue;

		unique.insert(path.stem().string());
	}

	filenames.assign(unique.begin(), unique.end());
	if (!filenames.empty()) {
		SetupFile();
	}
}

void Editor::SaveFile() {
	Quilt::Debug::Print("FILE SAVING IS NOT IMPLEMENTED YET");
	return;
}

void Editor::SetupFile() {
	std::string name = folder_path + filenames[selected_file_index] + ".mapbin";

	std::fstream file(name, std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		Quilt::Debug::Error("Failed to open file ", filenames[selected_file_index] + ".mapbin");
		return;
	}

	ClearNodes();
	ClearMapdata();

	file.read((char*)mapHeader, sizeof(Mapdata::Mapbin::Header));

	/* walls */
	file.seekg(Swap32(mapHeader->wallOffs));

	for (auto i = 0; i < Swap32(mapHeader->wallCount); i++) {
		Colbin::Entry wall;
		file.read((char*)&wall, sizeof(wall));
		nodes_wall.emplace_back(wall);
	}

	file.seekg(Swap32(mapHeader->labeledWallOffs));

	for (auto i = 0; i < Swap32(mapHeader->labeledWallCount); i++) {
		Mapdata::Mapbin::LabeledWall wall;
		file.read((char*)&wall, sizeof(wall));
		nodes_labeledWall.emplace_back(wall);
	}

	/* common gimmicks */
	file.seekg(Swap32(mapHeader->commonGmkOffs));
	for (auto i = 0; i < Swap32(mapHeader->commonGmkCount); i++) {
		Mapdata::Mapbin::CommonGimmick cmnGmk;
		file.read((char*)&cmnGmk, sizeof(cmnGmk));
		nodes_cmnGmk.emplace_back(cmnGmk);
	}

	/* gimmicks */
	file.seekg(Swap32(mapHeader->gmkOffs));
	
	for (auto i = 0; i < Swap32(mapHeader->gmkCount); i++) {
		Mapdata::Mapbin::Gimmick gmk;
		file.read((char*)&gmk, sizeof(gmk));
		if ("NONE" == std::string(gmk.name)) continue;
		nodes_gmk.emplace_back(gmk);
	}


	/* paths */
	file.seekg(Swap32(mapHeader->pathOffs));

	for (auto i = 0; i < Swap32(mapHeader->pathCount); i++) {
		Mapdata::Mapbin::Path path;
		file.read((char*)&path, sizeof(path));


		std::vector<Vec2f> line;

		for (auto j = 0; j < Swap32(path.numPoints); j++) {
			Vec2f point;
			file.read((char*)&point, sizeof(point));
			line.push_back(point);
		}

		nodes_path.emplace_back(path, line);
	}

	/* race course info */
	file.seekg(Swap32(mapHeader->courseInfoOffs));

	for (auto i = 0; i < Swap32(mapHeader->courseInfoCount); i++) {
		Mapdata::Mapbin::RaceCourseInfo rcinfo;
		file.read((char*)&rcinfo, sizeof(rcinfo));
		nodes_rcinfo.emplace_back(rcinfo);
	}

	/* zones */
	file.seekg(Swap32(mapHeader->zoneOffs));

	for (auto i = 0; i < Swap32(mapHeader->zoneCount); i++) {
		Mapdata::Mapbin::Zone zone;
		file.read((char*)&zone, sizeof(zone));
		nodes_zone.emplace_back(zone);
	}


	/* common gimmick names */
	file.seekg(Swap32(mapHeader->commonGmkNamesOffs));

	u32 num = 0;
	file.read((char*)&num, 4);

	for (auto i = 0; i < Swap32(num); i++) {
		char bytes[0x20];
		file.read(bytes, sizeof(bytes));
		bytes_cmnGmk.push_back(bytes);
	}
	open = true;
	file.close();

	/* enemies are stored in a separate file fsr */
	name = folder_path + filenames[selected_file_index] + ".enbin";
	file.open(name, std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		Quilt::Debug::Error("Failed to open file ", filenames[selected_file_index] + ".enbin");
		return;
	}

	file.read((char*)enHeader, sizeof(Mapdata::Enbin::Header));

	for (auto i = 0; i < Swap32(enHeader->numEntries); i++) {
		Mapdata::Enbin::EnemyEntry enemy;
		file.read((char*)&enemy, sizeof(enemy));
		nodes_enemy.emplace_back(enemy);
	}

	// todo - handle whatever is at the footer
	file.close();
}

void Editor::ClearFilenames() {
	folder_path.clear();
	folder_name.clear();
	selected_file.clear();
	filenames.clear();
	selected_file_index = 0;
}