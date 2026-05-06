//
// Created by bruno on 5/5/26.
//

#ifndef LIGHTWEIGHTGUI_ENGINE_H
#define LIGHTWEIGHTGUI_ENGINE_H
#include <string>
#include <vector>
#include <unordered_map>
#include "../GUI/GUIPrimitives.h"

namespace lw {
using WidgetID = unsigned int;
template <class>
inline constexpr bool always_false_v = false;
constexpr WidgetID INVALID_WIDGET_ID = 0xFFFFFFFF;

namespace Components {
struct PanelComponent {
    Color bgColor;
};

struct TextComponent {
    std::string text;
    Color color;
};

struct ButtonComponent {
    Color hoverColor;
    Color pressColor;
};
}

class Registry {
private:
    WidgetID nextId = 1;
    std::vector<bool> isAlive; // Tracks valid IDs

    // Sparse Storage for optional data
    std::tuple<
        std::unordered_map<WidgetID, Components::PanelComponent>,
        std::unordered_map<WidgetID, Components::TextComponent>,
        std::unordered_map<WidgetID, Components::ButtonComponent>
        // Add std::unordered_map<WidgetID, ImageComponent> here later!
    > components;

public:
    WidgetID CreateWidget() {
        WidgetID id = nextId++;
        if (id >= isAlive.size()) isAlive.resize(id + 100, false);
        isAlive[id] = true;
        return id;
    }

    void DestroyWidget(WidgetID id) {
        if (id >= isAlive.size() || !isAlive[id]) return;
        isAlive[id] = false;

        // Notice: We don't even need to clear the maps manually anymore.
        // We can use a template fold expression (C++17) to clear the ID from ALL maps!
        std::apply([id](auto&... map) {
            (map.erase(id), ...);
        }, components);
    }

    // --- THE TEMPLATE ROUTING ENGINE ---

    template <typename T>
    void AddComponent(WidgetID id, const T& data) {
        // std::get grabs the exact map matching this type at compile time
        std::get<std::unordered_map<WidgetID, T>>(components)[id] = data;
    }

    template <typename T>
    bool HasComponent(WidgetID id) const {
        auto& map = std::get<std::unordered_map<WidgetID, T>>(components);
        return map.count(id) > 0;
    }

    template <typename T>
    T& GetComponent(WidgetID id) {
        assert(HasComponent<T>(id) && "Widget does not have this component!");
        return std::get<std::unordered_map<WidgetID, T>>(components).at(id);
    }

    template <typename T>
    void RemoveComponent(WidgetID id) {
        std::get<std::unordered_map<WidgetID, T>>(components).erase(id);
    }
};
class Engine {
    static inline Registry registry;
public:
    static Registry& GetRegistry() { return registry; }
};
} // lw

#endif //LIGHTWEIGHTGUI_ENGINE_H
