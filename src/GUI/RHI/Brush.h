#pragma once
#include <variant>
#include "RHI/Handles.h"
#include "RHI/Geometry.h"

namespace lw::rhi {
struct SolidBrush {
    Color color;
};

struct GradientBrush {
    Color colorA, colorB;
    Vec2 ptA, ptB;
};

struct ImageBrush {
    TextureHandle texture;
    FloatRect srcRect;
};

using Brush = std::variant<SolidBrush, GradientBrush, ImageBrush>;
} // namespace lw::rhi
