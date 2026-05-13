#pragma once
#include <vector>
#include "RHI/Handles.h"
#include "RHI/Paint.h"
#include "RHI/Geometry.h"

namespace lw::rhi {
struct DrawCommand {
    GeometryHandle geo;
    Paint paint;
    Mat3x2 transform = Mat3x2::Identity();
};

struct DrawCommandList {
    std::vector<DrawCommand> commands;

    void Push(GeometryHandle geo, const Paint& paint,
              const Mat3x2& transform = Mat3x2::Identity()) {
        commands.push_back({geo, paint, transform});
    }

    void Clear() { commands.clear(); }
    bool Empty() const { return commands.empty(); }
};
} // namespace lw::rhi
