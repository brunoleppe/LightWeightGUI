//
// Created by bruno on 7/5/26.
//

#ifndef LIGHTWEIGTHGUI_CLIPPINGSTACK_H
#define LIGHTWEIGTHGUI_CLIPPINGSTACK_H
#include <stack>

#include "Types.h"
#include "raylib.h"

namespace lw {
class ClippingStack {
    static inline std::stack<Rect> s_stack;

public:
    static void Push(const Rect& rect) {
        Rect activeRect = rect;

        if (!s_stack.empty()) {
            const Rect& parent = s_stack.top();

            // Calculate intersection with the parent
            int nx = std::max(activeRect.x, parent.x);
            int ny = std::max(activeRect.y, parent.y);
            int nw = std::min(activeRect.x + activeRect.width, parent.x + parent.width) - nx;
            int nh = std::min(activeRect.y + activeRect.height, parent.y + parent.height) - ny;

            activeRect = {nx, ny, std::max(0, nw), std::max(0, nh)};
        }

        s_stack.push(activeRect);

        // Update Raylib state
        BeginScissorMode(activeRect.x, activeRect.y,
                         activeRect.width, activeRect.height);
    }
    static const Rect& Peek() {
        return s_stack.top();
    }
    static void Pop() {
        if (s_stack.empty()) {
            EndScissorMode();
            return;
        }
        s_stack.pop();
        if (s_stack.empty()) {
            EndScissorMode();
        } else {
            // Restore the previous clipping state
            const Rect& prev = s_stack.top();
            BeginScissorMode(prev.x, prev.y,
                             prev.width, prev.height);
        }
    }

    static bool IsCurrentVisible() {
        if (s_stack.empty()) return true;
        return (s_stack.top().width > 0 && s_stack.top().height > 0);
    }

    static bool Intersects(const Rect& rect) {
        if (s_stack.empty()) return true;
        const Rect& top = s_stack.top();
        int nw = std::min(rect.x + rect.width,  top.x + top.width)  - std::max(rect.x, top.x);
        int nh = std::min(rect.y + rect.height, top.y + top.height) - std::max(rect.y, top.y);
        return nw > 0 && nh > 0;
    }
};

struct ClippingScope {
    // Constructor performs the Push
    explicit ClippingScope(Rect requestedRect) {
        ClippingStack::Push(requestedRect);
    }

    // Destructor performs the Pop automatically
    ~ClippingScope() {
        ClippingStack::Pop();
    }

    // Prevent copying to ensure one Pop per Push
    ClippingScope(const ClippingScope&) = delete;
    ClippingScope& operator=(const ClippingScope&) = delete;

    // Allow moving if you need to pass ownership
    ClippingScope(ClippingScope&&) noexcept = default;

    // Helper to check if we should even bother drawing
    bool IsVisible() const {
        return ClippingStack::IsCurrentVisible();
    }
};
} // lw

#endif //LIGHTWEIGTHGUI_CLIPPINGSTACK_H
