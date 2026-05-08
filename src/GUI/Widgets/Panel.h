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
    std::unique_ptr<IRenderer> CreateRenderer() override {
        return std::make_unique<PanelRenderer>(this);
    }
};


inline void PanelRenderer::Render(const Rect& parentAbsRect) {
    Rect myRect {
        parentAbsRect.x + m_panel->transform->x,
        parentAbsRect.y + m_panel->transform->y,
        m_panel->transform->width,
        m_panel->transform->height
    };
    DrawRectangle(myRect.x, myRect.y, myRect.width, myRect.height, m_panel->backgroundColor);
    // RenderSubsystem handles children traversal and clipping.
}
} // llw

#endif //LIGHTWEIGTHGUI_PANEL_H
