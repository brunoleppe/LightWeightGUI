#include "RHI/Raylib/RaylibRenderDevice.h"
#include "raylib.h"
#include "rlgl.h"
#include <algorithm>
#include <cmath>

namespace lw::rhi {
// ---- helpers ---------------------------------------------------------------

static Color SampleBrush(const Brush& brush, Vec2 pos, float opacity) {
    Color out = {255, 255, 255, 255};

    if (auto* s = std::get_if<SolidBrush>(&brush)) {
        out = s->color;
    } else if (auto* g = std::get_if<GradientBrush>(&brush)) {
        Vec2 dir = {g->ptB.x - g->ptA.x, g->ptB.y - g->ptA.y};
        float len2 = dir.x * dir.x + dir.y * dir.y;
        float t = 0.f;
        if (len2 > 0.f)
            t = std::clamp(((pos.x - g->ptA.x) * dir.x + (pos.y - g->ptA.y) * dir.y) / len2, 0.f, 1.f);
        out.r = static_cast<uint8_t>(g->colorA.r * (1.f - t) + g->colorB.r * t);
        out.g = static_cast<uint8_t>(g->colorA.g * (1.f - t) + g->colorB.g * t);
        out.b = static_cast<uint8_t>(g->colorA.b * (1.f - t) + g->colorB.b * t);
        out.a = static_cast<uint8_t>(g->colorA.a * (1.f - t) + g->colorB.a * t);
    }

    out.a = static_cast<uint8_t>(out.a * opacity);
    return out;
}

// ---- buffer management -----------------------------------------------------

GeometryHandle RaylibRenderDevice::CreateVertexBuffer(std::span<const Vertex> verts) {
    GeometryHandle h = m_nextGeo++;
    m_buffers[h] = CpuBuffer{std::vector<Vertex>(verts.begin(), verts.end())};
    return h;
}

void RaylibRenderDevice::UpdateVertexBuffer(GeometryHandle h, std::span<const Vertex> verts) {
    if (auto it = m_buffers.find(h); it != m_buffers.end())
        it->second.vertices.assign(verts.begin(), verts.end());
}

void RaylibRenderDevice::DestroyBuffer(GeometryHandle h) {
    m_buffers.erase(h);
}

// ---- texture management ----------------------------------------------------

TextureHandle RaylibRenderDevice::CreateTexture(int w, int h, std::span<const uint8_t> pixels) {
    Image img{};
    img.width = w;
    img.height = h;
    img.mipmaps = 1;
    img.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    img.data = static_cast<void*>(const_cast<uint8_t*>(pixels.data()));
    Texture2D tex = LoadTextureFromImage(img);
    TextureHandle handle = m_nextTex++;
    m_textures[handle] = {tex.id, w, h};
    return handle;
}

void RaylibRenderDevice::DestroyTexture(TextureHandle h) {
    if (auto it = m_textures.find(h); it != m_textures.end()) {
        rlUnloadTexture(it->second.id);
        m_textures.erase(it);
    }
}

// ---- clip rect -------------------------------------------------------------

void RaylibRenderDevice::SetClipRect(const lw::Rect& r) {
    BeginScissorMode(r.x, r.y, r.width, r.height);
}

void RaylibRenderDevice::ClearClipRect() {
    EndScissorMode();
}

// ---- submit ----------------------------------------------------------------

void RaylibRenderDevice::Submit(const DrawCommandList& list) {
    for (const auto& cmd : list.commands) {
        auto it = m_buffers.find(cmd.geo);
        if (it == m_buffers.end()) continue;
        const auto& verts = it->second.vertices;
        if (verts.empty()) continue;

        unsigned int texId = rlGetTextureIdDefault();
        if (auto* img = std::get_if<ImageBrush>(&cmd.paint.fill))
            if (auto tit = m_textures.find(img->texture); tit != m_textures.end())
                texId = tit->second.id;

        bool isGradient = std::holds_alternative<GradientBrush>(cmd.paint.fill);

        rlSetTexture(texId);
        rlBegin(RL_TRIANGLES);

        if (!isGradient) {
            Color c = SampleBrush(cmd.paint.fill, {}, cmd.paint.opacity);
            rlColor4ub(c.r, c.g, c.b, c.a);
        }

        for (const auto& v : verts) {
            if (isGradient) {
                Color c = SampleBrush(cmd.paint.fill, v.pos, cmd.paint.opacity);
                rlColor4ub(c.r, c.g, c.b, c.a);
            }
            Vec2 p = cmd.transform.Apply(v.pos);
            rlTexCoord2f(v.uv.x, v.uv.y);
            rlVertex2f(p.x, p.y);
        }

        rlEnd();
        rlSetTexture(0);

        // TODO: stroke support
    }
}
} // namespace lw::rhi
