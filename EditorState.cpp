#include "EditorState.h"

using namespace editor;

EditorState::EditorState() {
    isActive = false;
}

void EditorState::initialise() {
    isActive = true;
}

void EditorState::uninitialise() {
    isActive = false;
}