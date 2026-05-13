/**
 * @file InputSystem.h
 * @author Bruno Leppe
 * @date 7/5/2026
 */

#ifndef LIGHTWEIGHTGUI_INPUTSYSTEM_H
#define LIGHTWEIGHTGUI_INPUTSYSTEM_H

#include "Types.h"

namespace lw {
class Widget;

class InputSystem {
    Widget* m_capturedWidget{nullptr};
    Widget* m_hoveredWidget{nullptr};

    Rect GetIntersection(const Rect& r1, const Rect& r2);
    Widget* HitTest(Widget* widget, int x, int y, const Rect& parentAbsRect, const Rect& currentClip);
    void SetHovered(Widget* widget);
    void SetCaptured(Widget* widget);

public:
    void Process(Widget* overlay, Widget* root, const InputState& input, const Rect& rootRect);
};
} // namespace lw

#endif // LIGHTWEIGHTGUI_INPUTSYSTEM_H
