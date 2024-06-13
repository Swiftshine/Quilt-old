#include <cstdio>

#include "quilt.h"

static Editor editor;

int main(int argc, char** argv) {
	if (!editor.Setup()) {
		printf("Error - failed to set up Quilt\n");
		editor.Cleanup();
		return -1;
	}

	editor.Run();
	editor.Cleanup();
	printf("Program ended succesfully\n");
	return 0;
}