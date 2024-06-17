#include "quilt.h"

void Editor::SetupFile() {
	if (selectedFile.empty()) return;
	// filename = "folder/file.mapbin"
	std::string filename = folderPath + selectedFile + ".mapbin";

	ClearSelectedNode();
	ClearNodes();
	ClearMapdata();

	std::fstream file;
	file.open(filename, std::ios::in | std::ios::binary);

	if (!file.is_open()) {
		printf("Warning - failed to open file %s. The file's contents will not be rendered.\n", filename.c_str());
		return;
	}

	file.read((char*)&mapHeader, sizeof(mapHeader));

	/* walls */
	file.seekg(Swap32(mapHeader.wallOffset));
	
	for (auto i = 0; i < Swap32(mapHeader.wallCount); i++) {
		Colbin::Entry wall;
		file.read((char*)&wall, sizeof(wall));
		walls.push_back(wall);
	}

	/* walls with labels */
	file.seekg(Swap32(mapHeader.dataSegLabelOffset));

	for (auto i = 0; i < Swap32(mapHeader.dataSegLabelCount); i++) {
		Mapdata::Mapbin::DataSegLabel dsl;
		file.read((char*)&dsl, sizeof(dsl));
		dataSegLabels.push_back(dsl);
	}

	/* common gimmicks */
	file.seekg(Swap32(mapHeader.commonGimmickOffset));

	for (auto i = 0; i < Swap32(mapHeader.commonGimmickCount); i++) {
		Mapdata::Mapbin::CommonGimmick cgmk;
		file.read((char*)&cgmk, sizeof(cgmk));
		commonGimmicks.push_back(cgmk);

		// configure a node too

		CmnGmkNode node;
		node.Configure(&cgmk);
		cmnGmkNodes.push_back(node);
	}

	/* regular gimmicks */
	file.seekg(Swap32(mapHeader.gimmickOffset));

	for (auto i = 0; i < Swap32(mapHeader.gimmickCount); i++) {
		Mapdata::Mapbin::Gimmick gmk;
		file.read((char*)&gmk, sizeof(gmk));

		// todo - ignore "NONE"

		gimmicks.push_back(gmk);

		// node config

		GmkNode node;
		node.Configure(&gmk);
		gmkNodes.push_back(node);
	}

	/* paths */
	file.seekg(Swap32(mapHeader.pathOffset));

	for (auto i = 0; i < Swap32(mapHeader.pathCount); i++) {
		Mapdata::Mapbin::Path path;
		file.read((char*)&path, sizeof(path));
		std::vector<Vec2f> line;

		for (auto j = 0; j < Swap32(path.numPoints); j++) {
			Vec2f point;
			file.read((char*)&point, sizeof(point));
			line.push_back(point);
		}

		paths.push_back(path);
		lines.push_back(line);
	}

	/* controllers */
	file.seekg(Swap32(mapHeader.controllerOffset));

	for (auto i = 0; i < Swap32(mapHeader.controllerCount); i++) {
		Mapdata::Mapbin::Controller controller;
		file.read((char*)&controller, sizeof(controller));
		// todo - ignore "NONE"
		controllers.push_back(controller);

		ContNode node;
		node.Configure(&controller);
		contNodes.push_back(node);
	}

	/* course info */
	file.seekg(Swap32(mapHeader.courseInfoOffset));

	for (auto i = 0; i < Swap32(mapHeader.courseInfoCount); i++) {
		Mapdata::Mapbin::CourseInfo cinfo;
		file.read((char*)&cinfo, sizeof(cinfo));
		// todo - ignore "NONE"
		courseInfos.push_back(cinfo);
	}

	/* misc information (structures without a fixed size */
	// -> common gimmick names
	file.seekg(Swap32(mapHeader.commonGimmickHeaderOffset));

	u32 num = 0;
	file.read((char*)&num, sizeof(u32));

	for (auto i = 0; i < Swap32(num); i++) {
		char name[0x20];
		file.read(name, sizeof(name));
		rawCommonGimmickNames.push_back(std::string(name));
	}

	// -> collision types
	file.seekg(Swap32(mapHeader.colbinTypeOffset));

	num = 0;
	file.read((char*)&num, sizeof(u32));

	for (auto i = 0; i < Swap32(num); i++) {
		char name[0x20];
		file.read(name, sizeof(name));
		colbinTypes.push_back(std::string(name));
	}

	// -> data seg label names
	num = 0;
	for (auto i = 0; i < Swap32(num); i++) {
		char name[0x20];
		file.read(name, sizeof(name));
		dataSegLabelNames.push_back(std::string(name));
	}

	file.close();

	// filename = "folder/file.enbin"

	filename = folderPath + selectedFile + ".enbin";
	file.open(filename, std::ios::in | std::ios::binary);

	if (!file.is_open()) {
		printf("Warning - failed to open file %s. The file's contents will not be rendered.\n", filename.c_str());
		return;
	}

	file.read((char*)&enHeader, sizeof(enHeader));

	for (auto i = 0; i < Swap32(enHeader.numEntries); i++) {
		Mapdata::Enbin::EnemyEntry enemy;
		file.read((char*)&enemy, sizeof(enemy));
		enemies.push_back(enemy);

		EnNode node;
		node.Configure(&enemy);
		enNodes.push_back(node);
	}

	num = 0;
	file.read((char*)&num, sizeof(u32));

	for (auto i = 0; i < Swap32(num); i++) {
		Mapdata::Enbin::FooterEntry footEntry;
		file.read((char*)&footEntry, sizeof(footEntry));
		enFooterEntries.push_back(footEntry);
	}
	file.close();

	SetupCommonGimmickNames();

	for (auto i = 0; i < cmnGmkNodes.size(); i++) {
		CmnGmkNode* node = &cmnGmkNodes[i];
		node->name = commonGimmickNames[node->nameIndex];
	}

	if (!open) open = true;
}

void Editor::SaveFile() {
	if (!open) return;
	// afaik there's never any padding between structs

	std::string filename = folderPath + "edit - " + selectedFile + ".mapbin";

	std::fstream file;
	file.open(filename , std::ios::out | std::ios::binary);
	if (!file.is_open()) {
		printf("failed to write mapbin\n");
		return;
	}

	file.write((char*)&mapHeader, sizeof(mapHeader));

	for (const auto& wall : walls) {
		file.write((char*)&wall, sizeof(wall));
	}

	for (const auto& dataSegLabel : dataSegLabels) {
		file.write((char*)&dataSegLabel, sizeof(dataSegLabel));
	}

	// endianness is only swapped when necessary

	for (const auto& node : cmnGmkNodes) {
		Mapdata::Mapbin::CommonGimmick cmnGmk;
		// afaik having a vector of certain structs
		// such as these isn't technically required
		// due to the node system already being in place,
		// but i won't remove them just yet

		cmnGmk.params = node.params;
		cmnGmk.nameIndex = Swap32(node.nameIndex);
		cmnGmk.position.x = SwapF32(node.position.x);
		cmnGmk.position.y = SwapF32(node.position.y);
		cmnGmk.position.z = 0.0f; // it was probably zero to begin with

		file.write((char*)&cmnGmk, sizeof(cmnGmk));
	}

	for (const auto& node : gmkNodes) {
		Mapdata::Mapbin::Gimmick gmk;
		char name[0x30] = { 0 };
		std::memcpy(name, node.name.data(), node.name.size());
		std::memcpy(gmk.name, name, 0x30);
		gmk.params = node.params;
		gmk.position.x = SwapF32(node.position.x);
		gmk.position.y = SwapF32(node.position.y);
		gmk.position.z = 0.0f;
		char _30[0x10] = { 0 };

		std::memcpy(_30, node._30, 0x10);
		std::memcpy(gmk._30, _30, 0x10);
		file.write((char*)&gmk, sizeof(gmk));
	}


	for (auto i = 0; i < paths.size(); i++) {
		auto& path = paths[i];

		file.write((char*)&path, sizeof(path));

		u32 numPoints = path.numPoints;

		for (auto j = 0; j < Swap32(numPoints); j++) {
			float x = lines[i][j].x;
			float y = lines[i][j].y;

			file.write((char*)&x, 4);
			file.write((char*)&y, 4);
		}
	}

	for (const auto& controller : controllers) {
		file.write((char*)&controller, sizeof(controller));
	}

	for (const auto& cinfo : courseInfos) {
		file.write((char*)&cinfo, sizeof(cinfo));
	}

	u32 num = Swap32(rawCommonGimmickNames.size());
	file.write((char*)&num, sizeof(u32));

	for (auto i = 0; i < Swap32(num); i++) {
		char name[0x20] = { 0 };
		std::memcpy(name, rawCommonGimmickNames[i].data(), rawCommonGimmickNames[i].size());
		file.write(name, sizeof(name));
	}

	num = Swap32(colbinTypes.size());
	file.write((char*)&num, sizeof(u32));

	for (auto i = 0; i < Swap32(num); i++) {
		char name[0x20] = { 0 };
		std::memcpy(name, colbinTypes[i].data(), colbinTypes[i].size());
		file.write(name, sizeof(name));
	}

	num = Swap32(dataSegLabelNames.size());
	file.write((char*)&num, sizeof(u32));

	for (auto i = 0; i < Swap32(num); i++) {
		char name[0x20] = { 0 };
		std::memcpy(name, dataSegLabelNames[i].data(), dataSegLabelNames[i].size());
		file.write(name, sizeof(name));
	}

	auto writePos = file.tellp();
	if (writePos % 0x20 != 0) {
		auto pad = 0x20 - (writePos % 0x20);
		const char c = 0;
		for (auto i = 0; i < pad; i++) {
			file.write(&c, 1);
		}
	}
	file.close();

	filename = folderPath + "edit - " + selectedFile + ".enbin";
	file.open(filename, std::ios::out | std::ios::binary);
	if (!file.is_open()) {
		printf("failed to write enbin\n");
		return;
	}

	file.write((char*)&enHeader, sizeof(enHeader));

	for (const auto& enemy : enemies) {
		file.write((char*)&enemy, sizeof(enemy));
	}

	num = Swap32(enFooterEntries.size());
	file.write((char*)&num, sizeof(u32));
	
	for (auto i = 0; i < Swap32(num); i++) {
		file.write((char*)&enFooterEntries[i], sizeof(Mapdata::Enbin::FooterEntry));
	}

	file.close();
}