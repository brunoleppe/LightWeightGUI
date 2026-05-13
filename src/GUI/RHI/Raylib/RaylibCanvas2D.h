#pragma once
#include "RHI/ICanvas2D.h"
#include "RHI/Raylib/RaylibRenderDevice.h"

namespace lw::rhi {
class RaylibCanvas2D final : public ICanvas2D {
    RaylibRenderDevice& m_device;
    DrawCommandList m_cmdList;

public:
    explicit RaylibCanvas2D(RaylibRenderDevice& device)
        : m_device(device) {
    }

    GeometryHandle BuildRoundedRect(const RoundedRect& shape) override;
    GeometryHandle BuildCircle(const Circle& shape) override;
    GeometryHandle BuildPath(const Path& path) override;
    void ReleaseGeometry(GeometryHandle handle) override;

    void Draw(GeometryHandle geo, const Paint& paint,
              const Mat3x2& transform = Mat3x2::Identity()) override;

    GeometryHandle CreateRawBuffer(std::span<const Vertex> vertices) override;
    void DrawRaw(GeometryHandle geo, const Paint& paint,
                 PrimitiveType primitive,
                 const Mat3x2& transform = Mat3x2::Identity()) override;

    DrawCommandList& GetCommandList() override { return m_cmdList; }
    void ClearCommandList() override { m_cmdList.Clear(); }
};
} // namespace lw::rhi
