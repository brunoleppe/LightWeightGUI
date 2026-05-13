#pragma once
#include <span>
#include "RHI/Handles.h"
#include "RHI/Geometry.h"
#include "RHI/DrawCommand.h"
#include "Types.h"

namespace lw::rhi {
struct IRenderDevice {
    virtual ~IRenderDevice() = default;

    virtual GeometryHandle CreateVertexBuffer(std::span<const Vertex> vertices) = 0;
    virtual void UpdateVertexBuffer(GeometryHandle handle, std::span<const Vertex> vertices) = 0;
    virtual void DestroyBuffer(GeometryHandle handle) = 0;

    virtual TextureHandle CreateTexture(int w, int h, std::span<const uint8_t> pixels) = 0;
    virtual void DestroyTexture(TextureHandle handle) = 0;

    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;
    virtual void SetClipRect(const lw::Rect& rect) = 0;
    virtual void ClearClipRect() = 0;
    virtual void Submit(const DrawCommandList& list) = 0;
};
} // namespace lw::rhi
