/**
 * @file OverlayRoot.h
 * @author Bruno Leppe
 * @date 11/5/2026
 */

#ifndef LIGHTWEIGHTGUI_OVERLAYROOT_H
#define LIGHTWEIGHTGUI_OVERLAYROOT_H

#include "Widget.h"

namespace lw {

class OverlayRoot;

class OverlayRootRenderer : public IRenderer {
public:
    void Render(const Rect& rect) override {
        // Empty renderer: do nothing
    }
};

class OverlayRoot : public Widget {
public:
    OverlayRoot() {
        zIndex = 1000;
    }
    std::unique_ptr<IRenderer> CreateRenderer() override {
        return std::make_unique<OverlayRootRenderer>();
    }
};

} // namespace lw

#endif // LIGHTWEIGHTGUI_OVERLAYROOT_H
