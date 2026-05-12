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
    LayoutPanelLayout(LayoutPanel* panel) : m_panel(panel) {}
    void Layout() override;
};


class LayoutPanel : public Panel {
public:
    LayoutPanel() {
        name = "LayoutPanel_"+std::to_string(reinterpret_cast<long long>(this));
    }
    LinearLayout layout{0,5,5,false};

    std::unique_ptr<IRenderer> CreateRenderer() override {
        return std::make_unique<LayoutPanelRenderer>(this);
    }
    std::unique_ptr<ILayout> CreateLayout() override {
        return std::make_unique<LayoutPanelLayout>(this);
    }
};

inline LayoutPanelRenderer::LayoutPanelRenderer(LayoutPanel* panel): PanelRenderer(panel), m_panel(panel) {

}

inline void LayoutPanelLayout::Layout() {
    std::vector<Widget*> visibleChildren = m_panel->GetVisibleChildren();
    m_panel->layout.count = visibleChildren.size();
    auto rects = lw::Layout::Compute(m_panel->transform, m_panel->layout);
    for (int i=0; i<rects.size(); i++){
        visibleChildren[i]->transform = rects[i];
    }
}


} // llw

#endif //LIGHTWEIGTHGUI_LAYOUTPANEL_H
