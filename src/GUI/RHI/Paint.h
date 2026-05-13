#pragma once
#include <optional>
#include "RHI/Brush.h"

namespace lw::rhi {
struct Paint {
    Brush fill = SolidBrush{{255, 255, 255, 255}};
    std::optional<Brush> stroke;
    float strokeWidth = 1.f;
    float opacity = 1.f;
};
} // namespace lw::rhi
