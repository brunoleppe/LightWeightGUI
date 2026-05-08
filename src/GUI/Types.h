/**
 * @file Types.h
 * @author Bruno Leppe
 * @date 7/5/2026
 */

#ifndef LIGHTWEIGHTGUI_TYPES_H
#define LIGHTWEIGHTGUI_TYPES_H

#include "raylib.h"

namespace lw {

struct Rect {
    int x, y, width, height;

    bool operator==(const Rect& o) const {
        return x == o.x && y == o.y && width == o.width && height == o.height;
    }
};

struct LwColor : Color{

    LwColor& operator=(const Color& c) {
        r = c.r;
        g = c.g;
        b = c.b;
        a = c.a;
        return *this;
    }
    LwColor& operator=(const LwColor& c) {
        r = c.r;
        g = c.g;
        b = c.b;
        a = c.a;
        return *this;
    }

    bool operator==(const LwColor& o) const {
        return r == o.r && g == o.g && b == o.b && a == o.a;
    }
};


struct Size {
    int width, height;
};

enum class InteractionState {
    Normal,
    Hovered,
    Pressed,
    Disabled
};

struct InputState {
    int mouseX, mouseY;
    bool mouseDown;
    bool mousePressed;
    bool mouseReleased;
};

} // namespace lw

#endif // LIGHTWEIGHTGUI_TYPES_H