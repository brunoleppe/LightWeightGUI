/**
 * @file Renderer.cpp
 * @author Bruno Leppe
 * @date 8/5/2026
 */

#include "Renderer.h"

#include <ranges>
#include <stack>

#include "ClippingStack.h"
#include "Widget.h"

namespace lw {
void RenderSubsystem::RenderWidgetTree(Widget* root, const Rect& rect) {
    std::stack<RenderTask> stack;
    ClippingStack::Push(rect);
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

        // Calculate absolute position once
        Rect widgetAbs{
            task.parentAbsRect.x + widget->transform->x,
            task.parentAbsRect.y + widget->transform->y,
            widget->transform->width,
            widget->transform->height
        };

        // Viewport Culling
        if (!ClippingStack::Intersects(widgetAbs)) continue;

        ClippingStack::Push(widgetAbs);
        stack.push({widget, {}, true}); // Push exit node

        // Push children in reverse for correct Z-order
        const auto& children = widget->GetChildren();
        for (const auto& child : std::ranges::reverse_view(children)) {
            stack.push({child.get(), widgetAbs, false});
        }

        widget->GetRenderer()->Render(widgetAbs);
    }
    ClippingStack::Pop();
}

RenderSubsystem::~RenderSubsystem() {
    if (m_initialized) UnloadRenderTexture(m_composite);
}

void RenderSubsystem::Render(Widget* overlay, Widget* root, const Rect& rect) {
    if (!root) return;

    // Init or recreate composite on resize
    if (!m_initialized || m_composite.texture.width != rect.width || m_composite.texture.height != rect.height) {
        if (m_initialized) UnloadRenderTexture(m_composite);
        m_composite = LoadRenderTexture(rect.width, rect.height);
        m_initialized = true;
        Widget::anyDirty = true;
    }

    // Rebuild composite only when something changed
    if (Widget::anyDirty) {
        BeginTextureMode(m_composite);
        ClearBackground(BLANK);
        RenderWidgetTree(root, rect);
        EndTextureMode();
        Widget::anyDirty = false;
    }

    // Always blit composite to screen — both buffers receive valid data every frame
    auto w = static_cast<float>(m_composite.texture.width);
    auto h = static_cast<float>(m_composite.texture.height);
    DrawTextureRec(m_composite.texture, {0.0f, 0.0f, w, -h}, {0.0f, 0.0f}, WHITE);
    RenderWidgetTree(overlay, rect);
}
} // lw
