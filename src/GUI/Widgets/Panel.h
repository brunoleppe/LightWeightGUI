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
    void DrawWidgetShadow(Rectangle bounds, int elevationLevel, float roundness = 0.0f) {

        if (elevationLevel == 0) return;
        float shadowAlpha = Theme::Get().GetAlpha(elevationLevel);
        int yOffset = Theme::Get().GetShadowOffset(elevationLevel);
        Color shadowColor = Fade(BLACK, shadowAlpha);
        Rectangle shadowBounds = bounds;
        shadowBounds.y += yOffset;
        if (roundness > 0.0f) {
            DrawRectangleRounded(shadowBounds, roundness, 16, shadowColor);
        } else {
            DrawRectangleRec(shadowBounds, shadowColor);
        }
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

    LwColor panelColor = Theme::Get().floatingCard.surface.color1;
    panelColor = DrawingUtils::GetElevationColor(panelColor, Theme::Get().GetAlpha(m_panel->zIndex));
    DrawWidgetShadow(panelRect, m_panel->zIndex,Theme::Get().floatingCard.cornerRadius);
    DrawRectangleRounded(panelRect, Theme::Get().floatingCard.cornerRadius, 4, panelColor);
}
} // llw

#endif //LIGHTWEIGTHGUI_PANEL_H
