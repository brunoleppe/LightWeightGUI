#pragma once
#include <span>
#include "RHI/Handles.h"
#include "RHI/Geometry.h"
#include "RHI/Paint.h"
#include "RHI/DrawCommand.h"

namespace lw::rhi {
enum class PrimitiveType { Triangles, Lines, Points };

struct ICanvas2D {
    virtual ~ICanvas2D() = default;

    // Retained geometry: tessellate once, cache on GPU/CPU, return handle.
    // Pass local-space bounds (origin at 0,0); use transform in Draw() for positioning.
    virtual GeometryHandle BuildRoundedRect(const RoundedRect& shape) = 0;
    virtual GeometryHandle BuildCircle(const Circle& shape) = 0;
    virtual GeometryHandle BuildPath(const Path& path) = 0;
    virtual void ReleaseGeometry(GeometryHandle handle) = 0;

    // Emit a DrawCommand into the internal list.
    virtual void Draw(GeometryHandle geo, const Paint& paint,
                      const Mat3x2& transform = Mat3x2::Identity()) = 0;

    // Expert mode: raw vertex buffer + explicit primitive type.
    virtual GeometryHandle CreateRawBuffer(std::span<const Vertex> vertices) = 0;
    virtual void DrawRaw(GeometryHandle geo, const Paint& paint,
                         PrimitiveType primitive,
                         const Mat3x2& transform = Mat3x2::Identity()) = 0;

    virtual DrawCommandList& GetCommandList() = 0;
    virtual void ClearCommandList() = 0;
};
} // namespace lw::rhi
