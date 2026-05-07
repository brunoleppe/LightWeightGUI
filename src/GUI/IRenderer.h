/**
 * @file IRenderer.h
 * @author Bruno Leppe
 * @date 7/5/2026
 */

#ifndef LIGHTWEIGHTGUI_IRENDERER_H
#define LIGHTWEIGHTGUI_IRENDERER_H

#include <memory>

#include "Types.h"

namespace lw {

struct IRenderer {
    virtual ~IRenderer() = default;
    virtual void Refresh() = 0;
    virtual void Render(const Rect& rect) = 0;
};

struct IRenderable {
    virtual ~IRenderable() = default;
    virtual std::unique_ptr<IRenderer> CreateRenderer() = 0;
};

} // namespace lw

#endif // LIGHTWEIGHTGUI_IRENDERER_H
