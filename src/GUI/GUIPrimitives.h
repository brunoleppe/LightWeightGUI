//
// Created by bruno on 5/5/26.
//

#ifndef LIGHTWEIGHTGUI_GUIPRIMITIVES_H
#define LIGHTWEIGHTGUI_GUIPRIMITIVES_H

namespace lw {
struct Rect {
    float x, y, w, h;
};

struct Transform {
    Rect bounds{};
    int zIndex = 0;
};

struct Color {
    float r, g, b, a;
};
}

#endif //LIGHTWEIGHTGUI_GUIPRIMITIVES_H
