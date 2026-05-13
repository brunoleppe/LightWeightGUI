#pragma once
#include <unordered_map>
#include <string>
#include "RHI/ITextBackend.h"
#include "raylib.h"

namespace lw::rhi {
class RaylibTextBackend final : public ITextBackend {
    struct FontEntry {
        Font font;
        float size;
        float spacing;
    };

    std::unordered_map<FontHandle, FontEntry> m_fonts;
    FontHandle m_next = 1;

public:
    ~RaylibTextBackend() override;

    FontHandle LoadFont(const std::string& path, int size) override;
    void UnloadFont(FontHandle handle) override;
    Vec2 MeasureText(FontHandle handle, const std::string& text) override;
    void DrawText(const std::string& text, Vec2 pos,
                  FontHandle font, const Paint& paint,
                  ICanvas2D& canvas) override;
};
} // namespace lw::rhi
