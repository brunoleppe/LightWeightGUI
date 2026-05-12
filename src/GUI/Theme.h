//
// Created by bruno on 11/5/26.
//

#ifndef LIGHTWEIGTHGUI_THEME_H
#define LIGHTWEIGTHGUI_THEME_H
#include "Types.h"

namespace lw {
enum class FillType { Solid, GradientVertical, GradientHorizontal };

struct SurfaceMaterial {
    FillType type = FillType::Solid;
    LwColor color1;
    LwColor color2;
};

struct ButtonMaterial {
    SurfaceMaterial normal;
    SurfaceMaterial hovered;
    SurfaceMaterial pressed;

    // Other physical traits
    LwColor textColor;
    int cornerRadius = 0;
    int borderThickness = 0;
    LwColor borderColor;
};

struct PanelMaterial {
    SurfaceMaterial surface;
    float cornerRadius = 0;
};

struct Theme {
    // Material presets
    ButtonMaterial primaryButton{};
    ButtonMaterial secondaryButton{};

    PanelMaterial windowBackground{{FillType::Solid, {0x1f, 0x1b, 0x24, 255}}};
    PanelMaterial floatingCard{{FillType::Solid, {0xff, 0xff, 0xff, 10}}, 0.1};

    static Theme& Get() {
        static Theme instance;
        return instance;
    }
};
} // lw

#endif //LIGHTWEIGTHGUI_THEME_H
