/**
 * @file Widget.h
 * @author Bruno Leppe 
 * @date 7/5/2026
 */

#ifndef LIGHTWEIGTHGUI_WIDGET_H
#define LIGHTWEIGTHGUI_WIDGET_H
#include <functional>
#include <memory>
#include <optional>
#include <vector>

#include "Delegate.h"

namespace lw {
struct Rect {
    int x, y, width, height;

    bool operator==(const Rect& o) const {
        return x == o.x && y == o.y && width == o.width && height == o.height;
    }
};

struct ColorBase {
    int r, g, b, a;

    bool operator==(const ColorBase& o) const {
        return r == o.r && g == o.g && b == o.b && a == o.a;
    }
};

using LwColor = std::optional<ColorBase>;

struct Size {
    int width, height;
};

struct IRenderer {
    virtual ~IRenderer() = default;
    virtual void Refresh() = 0;
    virtual void Render(const Rect& rect) = 0;
};

struct IRenderable {
    virtual ~IRenderable() = default;
    virtual std::unique_ptr<IRenderer> CreateRenderer() = 0;
};

enum class InteractionState {
    Normal,
    Hovered,
    Pressed,
    Disabled
};

template <typename T>
struct Property {
private:
    T n;

public:
    std::function<void()> on_change;

    // FIX 3: Initialize memory safely
    Property(T default_val = T{})
        : n(default_val) {
    }

    Property& operator=(const T& n_) {
        // FIX 4: Only trigger events if the value actually changed
        if (!(this->n == n_)) {
            this->n = n_;
            if (on_change) {
                on_change();
            }
        }
        return *this;
    }

    operator T() const { return n; }
};

class Widget : public IRenderable {
    std::vector<std::unique_ptr<Widget>> children;
    Widget* parent = nullptr;

protected:
    std::unique_ptr<IRenderer> renderer;

public:
    // Declare properties
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

        transform.on_change = trigger_refresh;
        backgroundColor.on_change = trigger_refresh;
        foregroundColor.on_change = trigger_refresh;
    }

    virtual ~Widget() = default;

    const std::vector<std::unique_ptr<Widget>>& GetChildren() const {
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

    MulticastDelegate<void(Widget*)> onClick;
    MulticastDelegate<void(Widget*)> onClickDown;
    MulticastDelegate<void(Widget*)> onClickUp;
};
} // namespace lw

#endif //LIGHTWEIGTHGUI_WIDGET_H
