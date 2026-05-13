#pragma once
#include <string>
#include "RHI/Handles.h"
#include "RHI/Geometry.h"
#include "RHI/Paint.h"

namespace lw::rhi {
struct ICanvas2D;

struct ITextBackend {
    virtual ~ITextBackend() = default;

    virtual FontHandle LoadFont(const std::string& path, int size) = 0;
    virtual void UnloadFont(FontHandle handle) = 0;
    virtual Vec2 MeasureText(FontHandle handle, const std::string& text) = 0;

    // Implementations may draw directly (Raylib) or emit glyph geometry via canvas (Dawn).
    virtual void DrawText(const std::string& text, Vec2 pos,
                          FontHandle font, const Paint& paint,
                          ICanvas2D& canvas) = 0;
};
} // namespace lw::rhi
