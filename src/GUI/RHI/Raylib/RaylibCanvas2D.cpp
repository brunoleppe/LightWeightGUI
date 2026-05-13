#include "RHI/Raylib/RaylibCanvas2D.h"
#include <algorithm>
#include <cmath>

namespace lw::rhi {
static constexpr int CORNER_SEGS = 8;
static constexpr int CIRCLE_SEGS = 32;

// ---- tessellation ----------------------------------------------------------

// Generates a convex polygon outline for a rounded rect, then fan-triangulates
// from the centroid. Works correctly when any radius is 0.
static std::vector<Vertex> TessellateRoundedRect(const RoundedRect& rr) {
    const float x = rr.bounds.x, y = rr.bounds.y;
    const float w = rr.bounds.width, h = rr.bounds.height;
    const float maxR = std::min(w, h) * 0.5f;

    const float tl = std::min(rr.radii[0], maxR);
    const float tr = std::min(rr.radii[1], maxR);
    const float br = std::min(rr.radii[2], maxR);
    const float bl = std::min(rr.radii[3], maxR);

    // Corner: center position, radius, arc start angle
    struct Corner {
        float cx, cy, r, a0;
    };
    const Corner corners[4] = {
        {x + tl, y + tl, tl, PI},            // TL
        {x + w - tr, y + tr, tr, PI * 1.5f}, // TR
        {x + w - br, y + h - br, br, 0.f},   // BR
        {x + bl, y + h - bl, bl, PI * 0.5f}, // BL
    };

    std::vector<Vec2> outline;
    outline.reserve((CORNER_SEGS + 1) * 4);
    for (const auto& c : corners) {
        for (int i = 0; i <= CORNER_SEGS; ++i) {
            float a = c.a0 + (PI * 0.5f) * i / CORNER_SEGS;
            outline.push_back({c.cx + c.r * std::cos(a), c.cy + c.r * std::sin(a)});
        }
    }

    const float cx = x + w * 0.5f, cy = y + h * 0.5f;
    const int N = static_cast<int>(outline.size());

    std::vector<Vertex> verts;
    verts.reserve(N * 3);
    for (int i = 0; i < N; ++i) {
        const Vec2 a = outline[i], b = outline[(i + 1) % N];
        verts.push_back({{cx, cy}, {0.5f, 0.5f}, WHITE});
        verts.push_back({{a.x, a.y}, {0.f, 0.f}, WHITE});
        verts.push_back({{b.x, b.y}, {0.f, 0.f}, WHITE});
    }
    return verts;
}

static std::vector<Vertex> TessellateCircle(const Circle& c) {
    std::vector<Vertex> verts;
    verts.reserve(CIRCLE_SEGS * 3);
    for (int i = 0; i < CIRCLE_SEGS; ++i) {
        float a0 = 2.f * PI * i / CIRCLE_SEGS;
        float a1 = 2.f * PI * (i + 1) / CIRCLE_SEGS;
        verts.push_back({{c.center.x, c.center.y}, {0.5f, 0.5f}, WHITE});
        verts.push_back({{c.center.x + c.radius * std::cos(a0),
                          c.center.y + c.radius * std::sin(a0)},
                         {0.f, 0.f}, WHITE});
        verts.push_back({{c.center.x + c.radius * std::cos(a1),
                          c.center.y + c.radius * std::sin(a1)},
                         {0.f, 0.f}, WHITE});
    }
    return verts;
}

// ---- ICanvas2D impl --------------------------------------------------------

GeometryHandle RaylibCanvas2D::BuildRoundedRect(const RoundedRect& shape) {
    auto v = TessellateRoundedRect(shape);
    return m_device.CreateVertexBuffer(v);
}

GeometryHandle RaylibCanvas2D::BuildCircle(const Circle& shape) {
    auto v = TessellateCircle(shape);
    return m_device.CreateVertexBuffer(v);
}

GeometryHandle RaylibCanvas2D::BuildPath(const Path&) {
    // Path tessellation not yet implemented.
    return INVALID_HANDLE;
}

void RaylibCanvas2D::ReleaseGeometry(GeometryHandle handle) {
    m_device.DestroyBuffer(handle);
}

void RaylibCanvas2D::Draw(GeometryHandle geo, const Paint& paint, const Mat3x2& transform) {
    m_cmdList.Push(geo, paint, transform);
}

GeometryHandle RaylibCanvas2D::CreateRawBuffer(std::span<const Vertex> vertices) {
    return m_device.CreateVertexBuffer(vertices);
}

void RaylibCanvas2D::DrawRaw(GeometryHandle geo, const Paint& paint,
                             PrimitiveType /*primitive*/, const Mat3x2& transform) {
    m_cmdList.Push(geo, paint, transform);
}
} // namespace lw::rhi
