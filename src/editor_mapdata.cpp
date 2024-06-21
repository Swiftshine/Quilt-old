#include "editor.h"

void Editor::ClearMapdata() {
	bytes_cmnGmk.clear();
	walls.clear();

	paths.clear();
	lines_path.clear();
}

void Editor::RenderMapdata() {
	if (showBounds)
		Quilt::Util::DrawRect(mapHeader->boundsStart.GetSwap(), mapHeader->boundsEnd.GetSwap(), 0x777777FF);

	if (showWalls) {
		for (auto& wall : walls) {
			Quilt::Util::DrawLine(wall.point1.GetSwap(), wall.point2.GetSwap(), 0xFFFFFFFF);
		}
	}

	if (showPaths) {
		for (auto& line : lines_path) {
			Quilt::Util::DrawLines(line, 0x00FF7AFF);
		}
	}
}