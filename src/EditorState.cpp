#include "EditorState.h"

using namespace editor;

EditorState::EditorState() {
    isActive = false;
}

void EditorState::initialise() {
    camX = 0;
    camY = 0;
    camZoom = 0.7f;
    isActive = true;
}

void EditorState::uninitialise() {
    isActive = false;
}