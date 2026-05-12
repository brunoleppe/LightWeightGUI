/**
 * @file Widget.h
 * @author Bruno Leppe
 * @date 7/5/2026
 */

#ifndef LIGHTWEIGHTGUI_WIDGET_H
#define LIGHTWEIGHTGUI_WIDGET_H

#include <memory>
#include <vector>

#include "Delegate.h"
#include "ILayout.h"
#include "IMeasurer.h"
#include "IRenderer.h"
#include "Layout.h"
#include "Property.h"
#include "Types.h"

namespace lw {

class Widget;

class AbsoluteBoundingBoxMeasurer : public IMeasurer {
    Widget* m_widget;
public:
    AbsoluteBoundingBoxMeasurer(Widget* widget) : m_widget(widget) {}
    Size ComputeMinSize(const Widget* widget) override;
};


class Widget : public IRenderable, public ILayoutable, public IMeasurable {
    std::vector<std::unique_ptr<Widget>> children;
    Widget* parent = nullptr;

    void Redraw() {
        needsRedraw = true;
        anyDirty = true;
    }

    void Refresh() {
        needsRefresh = true;
        needsRedraw = true;
        if (parent) {
            parent->Refresh();
        }
    }

protected:
    std::unique_ptr<IRenderer> m_renderer;
    std::unique_ptr<ILayout> m_layout;
    std::unique_ptr<IMeasurer> m_measurer;

public:
    inline static bool anyDirty{true};

    Property<Rect> transform{};
    Property<LwColor> backgroundColor{};
    Property<LwColor> foregroundColor{};
    Property<InteractionState> interactionState{InteractionState::Normal};
    Property<bool> visible{true};
    Property<bool> enabled{true};

    bool focused{false};
    bool needsRedraw{true};
    bool needsRefresh{false};

    int zIndex{0};

    std::string name{};


    Widget() {
        auto trigger_refresh = [this]() {
            Redraw();
            Refresh();
        };
        auto trigger_redraw = [this]() {
            Redraw();
        };

        backgroundColor.on_change = trigger_redraw;
        foregroundColor.on_change = trigger_redraw;
        enabled.on_change = trigger_redraw;
        interactionState.on_change = trigger_redraw;

        transform.on_change = trigger_refresh;
        visible.on_change = trigger_refresh;
    }

    ~Widget() override = default;

    [[nodiscard]] const std::vector<std::unique_ptr<Widget>>& GetChildren() const {
        return children;
    }

    std::vector<Widget*> GetVisibleChildren() {
        std::vector<Widget*> visible_children;
        for (auto& child : children) {
            if (child->visible) {
                visible_children.push_back(child.get());
            }
        }
        return visible_children;
    }


    Widget* AddWidget(std::unique_ptr<Widget> widget) {
        widget->zIndex = zIndex + 1;
        children.push_back(std::move(widget));
        children.back()->SetParent(this);
        Refresh();
        return children.back().get();
    }

    void SetParent(Widget* parent_) {
        this->parent = parent_;
    }

    void RemoveWidget(const Widget* widget) {
        for (auto it = children.begin(); it != children.end(); ++it) {
            if (it->get() == widget) {
                children.erase(it);
                return;
            }
        }
        Refresh();
    }

    IRenderer* GetRenderer() {
        if (!m_renderer) {
            m_renderer = CreateRenderer();
        }
        return m_renderer.get();
    }

    ILayout* GetLayout() {
        if (!m_layout) {
            m_layout = CreateLayout();
        }
        return m_layout.get();
    }
    IMeasurer* GetMeasurer() {
        if (!m_measurer) {
            m_measurer = CreateMeasurer();
        }
        return m_measurer.get();
    }

    std::unique_ptr<ILayout> CreateLayout() override {
        return std::make_unique<DefaultLayout>();
    }
    std::unique_ptr<IMeasurer> CreateMeasurer() override {
        return std::make_unique<AbsoluteBoundingBoxMeasurer>(this);
    }

    MulticastDelegate<Widget*> onClick;
    MulticastDelegate<Widget*> onClickDown;
    MulticastDelegate<Widget*> onClickUp;

    void Position(int x, int y) {
        Rect newRect = {x, y, transform->width, transform->height};
        transform = newRect;
        Refresh();
    }
    void Size(int width, int height) {
        Rect newRect = {transform->x, transform->y, width, height};
        transform = newRect;
        Refresh();
    }
};

inline Size AbsoluteBoundingBoxMeasurer::ComputeMinSize(const Widget* widget) {
    int maxWidth = 0;
    int maxHeight = 0;
    for (auto& child : widget->GetChildren()) {
        Size childSize = child->GetMeasurer()->ComputeMinSize(child.get());
        maxWidth = std::max(maxWidth, childSize.width + child->transform->x);
        maxHeight = std::max(maxHeight, childSize.height + child->transform->y);
    }
    return {maxWidth, maxHeight};
}


} // namespace lw

#endif // LIGHTWEIGHTGUI_WIDGET_H
