#ifndef EDITOR_STATE_H_
#define EDITOR_STATE_H_

#include "ToolState.h"
#include "SelectionState.h"
#include "EditableLevelTemplate.h"

namespace editor {
    class EditorState {
    public:
        bool isActive = false;

        ToolState toolState;
        SelectionState selectionState;
        EditableLevelTemplate levelTemplate;

        int camX, camY;
        float camZoom;


        EditorState();
        void initialise();
        void uninitialise();
    };
}


#endif