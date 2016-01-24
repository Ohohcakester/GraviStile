#ifndef TOOL_STATE_H_
#define TOOL_STATE_H_

namespace editor {
    enum eTool {
        tool_none,
        tool_spinConnection,
        tool_switchConnection,
        tool_leftTiles,
        tool_rightTiles,
        tool_move,
        tool_placePlatform,
        tool_placeDoor,
        tool_placePlayer,
        tool_quitConfirmation,
    };

    class ToolState {
    public:
        int state;
    };
}


#endif