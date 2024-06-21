#include "quilt.h"
#include "editor.h"

int main() {
    if (!Editor::Instance()->Setup()) {
        Quilt::Debug::Error("Failed to set up Quilt.");
        Editor::Instance()->Cleanup();
        return 1;
    }

    Editor::Instance()->Run();
    Editor::Instance()->Cleanup();

    return 0;
}