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
struct LwSize {
    int width, height;
    bool operator==(const LwSize& other) const {
        return width == other.width && height == other.height;
    }
};

struct Rect {
    int x, y, width, height;

    Rectangle AsRectangle() const {
        return {static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height)};
    }

    bool operator==(const Rect& o) const {
        return x == o.x && y == o.y && width == o.width && height == o.height;
    }
};

struct LwColor : Color {
    LwColor()
        : Color(0, 0, 0, 255) {
    }

    constexpr LwColor(int r, int g, int b, int a = 255)
        : Color(r, g, b, a) {
    }

    constexpr LwColor(const Color& c)
        : Color(c) {
    }
    constexpr LwColor(const LwColor& c)
        : Color(c) {
    }

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


    static LwColor GetRandomColor() {
        LwColor c;
        c.r = GetRandomValue(0, 255);
        c.g = GetRandomValue(0, 255);
        c.b = GetRandomValue(0, 255);
        c.a = 255;
        return c;
    }

    static const LwColor COLOR_LIGHTGRAY;
    static const LwColor COLOR_GRAY;
    static const LwColor COLOR_DARKGRAY;
    static const LwColor COLOR_YELLOW;
    static const LwColor COLOR_GOLD;
    static const LwColor COLOR_ORANGE;
    static const LwColor COLOR_PINK;
    static const LwColor COLOR_RED;
    static const LwColor COLOR_MAROON;
    static const LwColor COLOR_GREEN;
    static const LwColor COLOR_LIME;
    static const LwColor COLOR_DARKGREEN;
    static const LwColor COLOR_SKYBLUE;
    static const LwColor COLOR_BLUE;
    static const LwColor COLOR_DARKBLUE;
    static const LwColor COLOR_PURPLE;
    static const LwColor COLOR_VIOLET;
    static const LwColor COLOR_DARKPURPLE;
    static const LwColor COLOR_BEIGE;
    static const LwColor COLOR_BROWN;
    static const LwColor COLOR_DARKBROWN;
    static const LwColor COLOR_WHITE;
    static const LwColor COLOR_BLACK;
    static const LwColor COLOR_BLANK;
    static const LwColor COLOR_MAGENTA;
    static const LwColor COLOR_RAYWHITE;

private:
    static int GetRandomValue(int min, int max) {
        return (rand() % (max - min + 1)) + min;
    }
};

inline constexpr LwColor LwColor::COLOR_LIGHTGRAY{200, 200, 200, 255};
inline constexpr LwColor LwColor::COLOR_GRAY{130, 130, 130, 255};
inline constexpr LwColor LwColor::COLOR_DARKGRAY{80, 80, 80, 255};
inline constexpr LwColor LwColor::COLOR_YELLOW{253, 249, 0, 255};
inline constexpr LwColor LwColor::COLOR_GOLD{255, 203, 0, 255};
inline constexpr LwColor LwColor::COLOR_ORANGE{255, 161, 0, 255};
inline constexpr LwColor LwColor::COLOR_PINK{255, 109, 194, 255};
inline constexpr LwColor LwColor::COLOR_RED{230, 41, 55, 255};
inline constexpr LwColor LwColor::COLOR_MAROON{190, 33, 55, 255};
inline constexpr LwColor LwColor::COLOR_GREEN{0, 228, 48, 255};
inline constexpr LwColor LwColor::COLOR_LIME{0, 158, 47, 255};
inline constexpr LwColor LwColor::COLOR_DARKGREEN{0, 117, 44, 255};
inline constexpr LwColor LwColor::COLOR_SKYBLUE{102, 191, 255, 255};
inline constexpr LwColor LwColor::COLOR_BLUE{0, 121, 241, 255};
inline constexpr LwColor LwColor::COLOR_DARKBLUE{0, 82, 172, 255};
inline constexpr LwColor LwColor::COLOR_PURPLE{200, 122, 255, 255};
inline constexpr LwColor LwColor::COLOR_VIOLET{135, 60, 190, 255};
inline constexpr LwColor LwColor::COLOR_DARKPURPLE{112, 31, 126, 255};
inline constexpr LwColor LwColor::COLOR_BEIGE{211, 176, 131, 255};
inline constexpr LwColor LwColor::COLOR_BROWN{127, 106, 79, 255};
inline constexpr LwColor LwColor::COLOR_DARKBROWN{76, 63, 47, 255};
inline constexpr LwColor LwColor::COLOR_WHITE{255, 255, 255, 255};
inline constexpr LwColor LwColor::COLOR_BLACK{0, 0, 0, 255};
inline constexpr LwColor LwColor::COLOR_BLANK{0, 0, 0, 0};
inline constexpr LwColor LwColor::COLOR_MAGENTA{255, 0, 255, 255};
inline constexpr LwColor LwColor::COLOR_RAYWHITE{245, 245, 245, 255};

struct LwFont : Font {
    float fontSize;
    float spacing;
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
