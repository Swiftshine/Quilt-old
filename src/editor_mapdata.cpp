#include "editor.h"

void Editor::ClearMapdata() {
	bytes_cmnGmk.clear();
}

void Editor::RenderMapdata() {
	if (showBounds)
		Quilt::Util::DrawRect(mapHeader->boundsStart.GetSwap(), mapHeader->boundsEnd.GetSwap(), 0x777777FF);
}