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

    static Widget* HitTest(Widget* root, int x, int y);
    void SetHovered(Widget* widget);
    void SetCaptured(Widget* widget);

public:
    void Process(Widget* root, const InputState& input);
};

} // namespace lw

#endif // LIGHTWEIGHTGUI_INPUTSYSTEM_H