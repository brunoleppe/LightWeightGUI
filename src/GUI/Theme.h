//
// Created by bruno on 11/5/26.
//

#ifndef LIGHTWEIGTHGUI_THEME_H
#define LIGHTWEIGTHGUI_THEME_H
#include <vector>

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

struct AlphaMap {
    std::vector<float> map{ 0.0f, 0.05f, 0.07f, 0.08f, 0.09f, 0.11f, 0.12f, 0.14f, 0.15f, 0.16f };
};

struct ShadowOffsetMap {
    std::vector<int> yOffsetMap { 0, 2, 4, 6, 8, 10, 12, 16, 20, 24 };
};

struct Theme {
    // Material presets
    ButtonMaterial primaryButton{};
    ButtonMaterial secondaryButton{};

    PanelMaterial windowBackground{{FillType::Solid, {0x24, 0x24, 0x24, 255}}};
    PanelMaterial floatingCard{
        {FillType::Solid, {0x24, 0x24, 0x24, 255}},
        0.1f};
    AlphaMap alphaMap;
    ShadowOffsetMap shadowOffsetMap;

    float GetAlpha(int elevationIndex) const {
        if (elevationIndex < 0 || elevationIndex >= alphaMap.map.size()) {
            return alphaMap.map.back();
        }
        return alphaMap.map[elevationIndex];
    }
    float GetShadowOffset(int elevationIndex) const {
        if (elevationIndex < 0 || elevationIndex >= shadowOffsetMap.yOffsetMap.size()) {
            return shadowOffsetMap.yOffsetMap.back();
        }
        return shadowOffsetMap.yOffsetMap[elevationIndex];
    }

    static Theme& Get() {
        static Theme instance;
        return instance;
    }
};
} // lw

#endif //LIGHTWEIGTHGUI_THEME_H
