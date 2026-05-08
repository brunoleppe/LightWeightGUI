/**
 * @file Panel.h
 * @author Bruno Leppe
 * @date 7/5/2026
 */

#ifndef LIGHTWEIGTHGUI_PANEL_H
#define LIGHTWEIGTHGUI_PANEL_H
#include "ClippingStack.h"
#include "Widget.h"

namespace lw {
class Panel;

class PanelRenderer : public IRenderer {
    Panel* m_panel;

public:
    PanelRenderer(Panel* panel)
        : m_panel(panel) {
    }

    void Refresh() override;
    void Render(const Rect& rect) override;
};


class Panel : public Widget {
public:
    std::unique_ptr<IRenderer> CreateRenderer() override {
        return std::make_unique<PanelRenderer>(this);
    }
};

inline void PanelRenderer::Refresh() {
}

inline void PanelRenderer::Render(const Rect& rect) {
    Rect requested{rect.x + m_panel->transform->x, rect.y + m_panel->transform->y, m_panel->transform->width,
                       m_panel->transform->height};
    ClippingScope clip(requested);
    DrawRectangle(requested.x, requested.y, requested.width, requested.height, m_panel->backgroundColor);
    for (auto& widget : m_panel->GetChildren()) {
        if (widget->visible) {
            widget->GetRenderer()->Render(m_panel->transform);
        }
    }
}
} // llw

#endif //LIGHTWEIGTHGUI_PANEL_H
