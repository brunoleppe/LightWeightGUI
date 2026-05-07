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
#include "IRenderer.h"
#include "Property.h"
#include "Types.h"

namespace lw {

class Widget : public IRenderable {
    std::vector<std::unique_ptr<Widget>> children;
    Widget* parent = nullptr;

protected:
    std::unique_ptr<IRenderer> renderer;

public:
    Property<Rect> transform{};
    Property<LwColor> backgroundColor{};
    Property<LwColor> foregroundColor{};
    Property<InteractionState> interactionState{InteractionState::Normal};

    bool visible{true};
    bool enabled{true};
    bool focused{false};
    bool needsRedraw{true};

    Widget() {
        auto trigger_refresh = [this]() {
            needsRedraw = true;
            if (renderer) {
                renderer->Refresh();
            }
        };

        transform.on_change       = trigger_refresh;
        backgroundColor.on_change = trigger_refresh;
        foregroundColor.on_change = trigger_refresh;
        interactionState.on_change = trigger_refresh;
    }

    ~Widget() override = default;

    [[nodiscard]] const std::vector<std::unique_ptr<Widget>>& GetChildren() const {
        return children;
    }

    Widget* AddWidget(std::unique_ptr<Widget> widget) {
        children.push_back(std::move(widget));
        children.back()->SetParent(this);
        needsRedraw = true;
        return children.back().get();
    }

    void SetParent(Widget* parent_) {
        this->parent = parent_;
        needsRedraw = true;
        GetRenderer()->Refresh();
    }

    void RemoveWidget(const Widget* widget) {
        for (auto it = children.begin(); it != children.end(); ++it) {
            if (it->get() == widget) {
                children.erase(it);
                return;
            }
        }
    }

    IRenderer* GetRenderer() {
        if (!renderer) {
            renderer = CreateRenderer();
        }
        return renderer.get();
    }

    MulticastDelegate<Widget*> onClick;
    MulticastDelegate<Widget*> onClickDown;
    MulticastDelegate<Widget*> onClickUp;
};

} // namespace lw

#endif // LIGHTWEIGHTGUI_WIDGET_H