/**
 * @file Renderer.h
 * @author Bruno Leppe 
 * @date 8/5/2026
 */

#ifndef LIGHTWEIGTHGUI_RENDERER_H
#define LIGHTWEIGTHGUI_RENDERER_H
#include <stack>

#include "Types.h"

namespace lw {
class Widget;

class RenderSubsystem {
    struct RenderTask {
        Widget* widget;
        Rect parentAbsRect; // absolute rect of this widget's parent
        bool isExitNode;    // true if we only need to pop the clipping stack
    };

public:
    void Render(Widget* root, const Rect& rect);
};


} // lw

#endif //LIGHTWEIGTHGUI_RENDERER_H
