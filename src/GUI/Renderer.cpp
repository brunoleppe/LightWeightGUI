/**
 * @file Renderer.cpp
 * @author Bruno Leppe
 * @date 8/5/2026
 */

#include "Renderer.h"

#include <stack>

#include "ClippingStack.h"
#include "Widget.h"

namespace lw {

void RenderSubsystem::Render(Widget* root, const Rect& rect) {
    if (!root) return;

    std::stack<RenderTask> stack;
    ClippingStack::Push(rect);

    // Root's parent offset is {0,0,0,0} — its transform IS its absolute rect.
    stack.push({root, {0, 0, 0, 0}, false});

    while (!stack.empty()) {
        RenderTask task = stack.top();
        stack.pop();

        if (task.isExitNode) {
            ClippingStack::Pop();
            continue;
        }

        Widget* widget = task.widget;
        if (!widget->visible) continue;

        // Compute this widget's absolute rect from its parent's absolute origin.
        Rect widgetAbs {
            task.parentAbsRect.x + widget->transform->x,
            task.parentAbsRect.y + widget->transform->y,
            widget->transform->width,
            widget->transform->height
        };

        // Exit marker — pops the clip pushed below once all children are done.
        stack.push({widget, {}, true});

        // Push children in reverse so the first child renders first.
        const auto& children = widget->GetChildren();
        for (auto it = children.rbegin(); it != children.rend(); ++it) {
            stack.push({it->get(), widgetAbs, false});
        }

        // ClippingStack::Push already intersects with the current top internally.
        ClippingStack::Push(widgetAbs);

        // Pass the parent's absolute rect so the renderer can recompute its own
        // absolute position the same way: parentAbs + transform.
        if (widget->needsRedraw) {
            widget->GetRenderer()->Render(task.parentAbsRect);
            widget->needsRedraw = false;
        }
    }

    ClippingStack::Pop();
}
} // lw