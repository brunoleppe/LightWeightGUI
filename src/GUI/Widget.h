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
#include "IRenderer.h"
#include "Layout.h"
#include "Property.h"
#include "Types.h"

namespace lw {
class Widget : public IRenderable, public ILayoutable {
    std::vector<std::unique_ptr<Widget>> children;
    Widget* parent = nullptr;

    void Redraw() {
        needsRedraw = true;
    }

    void Refresh() {
        needsRefresh = true;
        needsRedraw = true;
        if (parent) {
            parent->Refresh();
        }
    }

protected:
    std::unique_ptr<IRenderer> renderer;
    std::unique_ptr<ILayout> layout;

public:
    Property<Rect> transform{};
    Property<LwColor> backgroundColor{};
    Property<LwColor> foregroundColor{};
    Property<InteractionState> interactionState{InteractionState::Normal};
    Property<bool> visible{true};
    Property<bool> enabled{true};

    bool focused{false};
    bool needsRedraw{true};
    bool needsRefresh{false};

    Widget() {
        auto trigger_refresh = [this]() {
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
        if (!renderer) {
            renderer = CreateRenderer();
        }
        return renderer.get();
    }

    ILayout* GetLayout() {
        if (!layout) {
            layout = CreateLayout();
        }
        return layout.get();
    }

    std::unique_ptr<ILayout> CreateLayout() override {
        return std::make_unique<DefaultLayout>();
    }

    MulticastDelegate<Widget*> onClick;
    MulticastDelegate<Widget*> onClickDown;
    MulticastDelegate<Widget*> onClickUp;


};
} // namespace lw

#endif // LIGHTWEIGHTGUI_WIDGET_H
