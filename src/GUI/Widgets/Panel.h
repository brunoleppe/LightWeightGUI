/**
 * @file Panel.h
 * @author Bruno Leppe
 * @date 7/5/2026
 */

#ifndef LIGHTWEIGTHGUI_PANEL_H
#define LIGHTWEIGTHGUI_PANEL_H
#include "Widget.h"

namespace lw {
class Panel;

class PanelRenderer : public IRenderer {
    Panel* m_panel;

public:
    PanelRenderer(Panel* panel)
        : m_panel(panel) {
    }

    void Render(const Rect& rect) override;
};


class Panel : public Widget {
public:
    Panel() {
        name = "Panel_" + std::to_string(reinterpret_cast<long>(this));
    }

    std::unique_ptr<IRenderer> CreateRenderer() override {
        return std::make_unique<PanelRenderer>(this);
    }
};


inline void PanelRenderer::Render(const Rect& rect) {
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, Theme::Get().floatingCard.surface.color1);
}
} // llw

#endif //LIGHTWEIGTHGUI_PANEL_H
