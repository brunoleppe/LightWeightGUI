/**
 * @file Types.h
 * @author Bruno Leppe
 * @date 7/5/2026
 */

#ifndef LIGHTWEIGHTGUI_TYPES_H
#define LIGHTWEIGHTGUI_TYPES_H

#include <optional>

namespace lw {

struct Rect {
    int x, y, width, height;

    bool operator==(const Rect& o) const {
        return x == o.x && y == o.y && width == o.width && height == o.height;
    }
};

struct ColorBase {
    int r, g, b, a;

    bool operator==(const ColorBase& o) const {
        return r == o.r && g == o.g && b == o.b && a == o.a;
    }
};

using LwColor = std::optional<ColorBase>;

struct Size {
    int width, height;
};

enum class InteractionState {
    Normal,
    Hovered,
    Pressed,
    Disabled
};

} // namespace lw

#endif // LIGHTWEIGHTGUI_TYPES_H