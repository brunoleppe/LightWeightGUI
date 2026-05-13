#include "RHI/Raylib/RaylibTextBackend.h"
#include "raylib.h"

namespace lw::rhi {
RaylibTextBackend::~RaylibTextBackend() {
    for (auto& [h, e] : m_fonts)
        ::UnloadFont(e.font);
}

FontHandle RaylibTextBackend::LoadFont(const std::string& path, int size) {
    FontEntry e;
    e.font = LoadFontEx(path.c_str(), size, nullptr, 0);
    e.size = static_cast<float>(size);
    e.spacing = 1.f;
    FontHandle h = m_next++;
    m_fonts[h] = e;
    return h;
}

void RaylibTextBackend::UnloadFont(FontHandle handle) {
    if (auto it = m_fonts.find(handle); it != m_fonts.end()) {
        ::UnloadFont(it->second.font);
        m_fonts.erase(it);
    }
}

Vec2 RaylibTextBackend::MeasureText(FontHandle handle, const std::string& text) {
    auto it = m_fonts.find(handle);
    if (it == m_fonts.end()) return {};
    Vector2 s = MeasureTextEx(it->second.font, text.c_str(), it->second.size, it->second.spacing);
    return {s.x, s.y};
}

void RaylibTextBackend::DrawText(const std::string& text, Vec2 pos,
                                 FontHandle handle, const Paint& paint,
                                 ICanvas2D& /*canvas*/) {
    auto it = m_fonts.find(handle);
    if (it == m_fonts.end()) return;

    ::Color col = {255, 255, 255, 255};
    if (auto* s = std::get_if<SolidBrush>(&paint.fill))
        col = {s->color.r, s->color.g, s->color.b, s->color.a};
    col.a = static_cast<uint8_t>(col.a * paint.opacity);

    ::DrawTextEx(it->second.font, text.c_str(), {pos.x, pos.y},
                 it->second.size, it->second.spacing, col);
}
} // namespace lw::rhi
