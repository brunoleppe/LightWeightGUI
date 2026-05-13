/**
 * @file Panel.h
 * @author Bruno Leppe
 * @date 7/5/2026
 */

#ifndef LIGHTWEIGTHGUI_PANEL_H
#define LIGHTWEIGTHGUI_PANEL_H
#include "Utils.h"
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
        name = "Panel_" + std::to_string(reinterpret_cast<long long>(this));
    }

    std::unique_ptr<IRenderer> CreateRenderer() override {
        return std::make_unique<PanelRenderer>(this);
    }
};

inline void PanelRenderer::Render(const Rect& rect) {
    const float lineOffset = 2;
    Rectangle panelRect = {
        rect.x + lineOffset,
        rect.y + lineOffset,
        rect.width - lineOffset * 2,
        rect.height - lineOffset * 2
    };

    LwColor panelColor;
    if (m_panel->backgroundColor.IsSet()) {
        panelColor = m_panel->backgroundColor;
    } else {
        panelColor = DrawingUtils::GetElevationColor(Theme::Get().panelBackground.surface.color1,
                                                     Theme::Get().GetAlpha(m_panel->zIndex));
    }
    DrawingUtils::DrawWidgetShadow(panelRect, m_panel->zIndex, 0);
    if (Theme::Get().panelBackground.cornerRadius == 0.f) {
        DrawRectangle(panelRect.x, panelRect.y, panelRect.width, panelRect.height, panelColor);
    } else {
        DrawRectangleRounded(panelRect, Theme::Get().panelBackground.cornerRadius, 16, panelColor);
    }

    if (Theme::Get().panelBackground.border.thickness > 0) {
        if (Theme::Get().panelBackground.cornerRadius == 0.f) {
            DrawRectangleLinesEx(panelRect, Theme::Get().panelBackground.border.thickness,
                                 Theme::Get().panelBackground.border.color);
        } else {
            DrawRectangleRoundedLines(panelRect, Theme::Get().panelBackground.cornerRadius, 16,
                                      Theme::Get().panelBackground.border.thickness,
                                      Theme::Get().panelBackground.border.color);
        }
    }
}
} // llw

#endif //LIGHTWEIGTHGUI_PANEL_H
