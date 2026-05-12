/**
 * @file LayoutPanel.h
 * @author Bruno Leppe 
 * @date 8/5/2026
 */

#ifndef LIGHTWEIGTHGUI_LAYOUTPANEL_H
#define LIGHTWEIGTHGUI_LAYOUTPANEL_H
#include "Layout.h"
#include "Panel.h"

namespace lw {
class LayoutPanel;

class LayoutPanelRenderer : public PanelRenderer {
    LayoutPanel* m_panel;

public:
    LayoutPanelRenderer(LayoutPanel* panel);
};

class LayoutPanelLayout : public ILayout {
    LayoutPanel* m_panel;

public:
    LayoutPanelLayout(LayoutPanel* panel)
        : m_panel(panel) {
    }

    void Layout() override;
};


class LayoutPanel : public Panel {
public:
    LayoutPanel() {
        name = "LayoutPanel_" + std::to_string(reinterpret_cast<long long>(this));
    }

    LinearLayout layout{0, 5, 5, false};

    std::unique_ptr<IRenderer> CreateRenderer() override {
        return std::make_unique<LayoutPanelRenderer>(this);
    }

    std::unique_ptr<ILayout> CreateLayout() override {
        return std::make_unique<LayoutPanelLayout>(this);
    }
};

inline LayoutPanelRenderer::LayoutPanelRenderer(LayoutPanel* panel)
    : PanelRenderer(panel), m_panel(panel) {
}

inline void LayoutPanelLayout::Layout() {
    const std::vector<std::unique_ptr<Widget>>& children = m_panel->GetChildren();
    if (children.empty()) { return; }
    const auto& layout = m_panel->layout;
    int currentY = layout.padding;
    int currentX = layout.padding;
    int availableWidth = m_panel->transform->width - layout.padding * 2;
    int availableHeight = m_panel->transform->height - layout.padding * 2;

    for (auto& child : children) {
        if (!child->visible) { continue; }
        // LwSize minSize = child->GetMeasurer()->ComputeMinSize();
        LwSize minSize = child->GetDesiredLayoutSize();
        Rect newTransform{currentX, currentY, 0 ,0};
        if (layout.horizontal) {
            newTransform.width = minSize.width;
            newTransform.height = std::max(minSize.height, availableHeight);
            currentX += newTransform.width + layout.spacing;
        }else {
            newTransform.width = std::max(minSize.width, availableWidth);
            newTransform.height = minSize.height;
            currentY += newTransform.height + layout.spacing;
        }
        child->transform = newTransform;
    }
}
} // llw

#endif //LIGHTWEIGTHGUI_LAYOUTPANEL_H
