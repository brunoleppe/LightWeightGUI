//
// Created by bruno on 5/5/26.
//

#ifndef LIGHTWEIGHTGUI_WIDGET_H
#define LIGHTWEIGHTGUI_WIDGET_H
#include <iostream>
#include <memory>
#include <vector>
#include "GUIPrimitives.h"
#include "Engine.h"


namespace lw {
class Widget {
    std::vector<std::unique_ptr<Widget>> children;
    uint32_t id{};
    Widget* parent = nullptr;
public:
    Transform transform;
    Color color;
    bool visible;
    bool enabled;

    virtual ~Widget() = default;

    Widget() {
        id = Engine::GetRegistry().CreateWidget();
    }

    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;


    Widget* Add(std::unique_ptr<Widget> widget) {
        widget->parent = this;         // Link the mathematical tree
        Widget* ptr = widget.get();    // Keep a raw pointer for the return
        children.push_back(std::move(widget)); // Take memory ownership
        return ptr;
    }

    void Remove(Widget* widget) {
        std::erase_if(children,
                      [widget](
                      const std::unique_ptr<Widget>& w) {
                          return w.get() == widget;
                      });
    }

    void print() {
        std::cout << "Widget: " << id << std::endl;
        for (auto& child : children) {
            child->print();
        }
    }

    template <typename T>
    void AddComponent(const T& data) {
        Engine::GetRegistry().AddComponent<T>(id, data);
    }

    template <typename T>
    T& GetComponent() {
        return Engine::GetRegistry().GetComponent<T>(id);
    }

    template <typename T>
    bool HasComponent() const {
        return Engine::GetRegistry().HasComponent<T>(id);
    }
};
} // lw

#endif //LIGHTWEIGHTGUI_WIDGET_H
