/**
 * @file Types.h
 * @author Bruno Leppe
 * @date 7/5/2026
 */

#ifndef LIGHTWEIGHTGUI_TYPES_H
#define LIGHTWEIGHTGUI_TYPES_H

#include <cstdlib>

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


    static LwColor GetRandomColor()
    {
        LwColor c;
        c.r = GetRandomValue(0, 255);
        c.g = GetRandomValue(0, 255);
        c.b = GetRandomValue(0, 255);
        c.a = 255;
        return c;
    }
private:
    static int GetRandomValue(int min, int max)
    {
       return (rand() % (max - min + 1)) + min;
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