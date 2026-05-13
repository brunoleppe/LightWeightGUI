#pragma once
#include <vector>
#include <cstdint>

namespace lw::rhi {
struct Color {
    uint8_t r, g, b, a;
};

struct Vec2 {
    float x = 0, y = 0;
};

struct Mat3x2 {
    float a = 1, b = 0;
    float c = 0, d = 1;
    float tx = 0, ty = 0;

    static constexpr Mat3x2 Identity() { return {}; }
    static constexpr Mat3x2 Translation(float x, float y) { return {1, 0, 0, 1, x, y}; }

    Vec2 Apply(Vec2 p) const { return {a * p.x + c * p.y + tx, b * p.x + d * p.y + ty}; }
};

struct Vertex {
    Vec2 pos;
    Vec2 uv;
    Color col;
};

struct FloatRect {
    float x, y, width, height;
};

struct RoundedRect {
    FloatRect bounds;
    float radii[4]; // TL, TR, BR, BL
};

struct Circle {
    Vec2 center;
    float radius;
};

enum class PathCmdType { MoveTo, LineTo, QuadTo, CubicTo, Close };

struct PathCmd {
    PathCmdType type;
    Vec2 p0, p1, p2;
};

struct Path {
    std::vector<PathCmd> cmds;

    void MoveTo(float x, float y) { cmds.push_back({PathCmdType::MoveTo, {x, y}}); }
    void LineTo(float x, float y) { cmds.push_back({PathCmdType::LineTo, {x, y}}); }

    void QuadTo(float cx, float cy, float x, float y) {
        cmds.push_back({PathCmdType::QuadTo, {cx, cy}, {x, y}});
    }

    void CubicTo(float c1x, float c1y, float c2x, float c2y, float x, float y) {
        cmds.push_back({PathCmdType::CubicTo, {c1x, c1y}, {c2x, c2y}, {x, y}});
    }

    void Close() { cmds.push_back({PathCmdType::Close}); }
};
} // namespace lw::rhi
