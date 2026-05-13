#pragma once
#include <unordered_map>
#include <vector>
#include "RHI/IRenderDevice.h"

namespace lw::rhi {
class RaylibRenderDevice final : public IRenderDevice {
    struct CpuBuffer {
        std::vector<Vertex> vertices;
    };

    struct GpuTexture {
        unsigned int id;
        int w, h;
    };

    std::unordered_map<GeometryHandle, CpuBuffer> m_buffers;
    std::unordered_map<TextureHandle, GpuTexture> m_textures;
    GeometryHandle m_nextGeo = 1;
    TextureHandle m_nextTex = 1;

public:
    GeometryHandle CreateVertexBuffer(std::span<const Vertex> vertices) override;
    void UpdateVertexBuffer(GeometryHandle handle, std::span<const Vertex> vertices) override;
    void DestroyBuffer(GeometryHandle handle) override;

    TextureHandle CreateTexture(int w, int h, std::span<const uint8_t> pixels) override;
    void DestroyTexture(TextureHandle handle) override;

    void BeginFrame() override {
    }

    void EndFrame() override {
    }

    void SetClipRect(const lw::Rect& rect) override;
    void ClearClipRect() override;
    void Submit(const DrawCommandList& list) override;
};
} // namespace lw::rhi
