/**
 * @file InputSystem.cpp
 * @author Bruno Leppe
 * @date 7/5/2026
 */

#include "InputSystem.h"
#include "Widget.h"

namespace lw {

Widget* InputSystem::HitTest(Widget* widget, int x, int y) {
    if (!widget->visible || !widget->enabled) return nullptr;

    // Children are tested in reverse so the topmost-rendered child wins.
    for (auto it = widget->GetChildren().rbegin(); it != widget->GetChildren().rend(); ++it) {
        if (Widget* hit = HitTest(it->get(), x, y))
            return hit;
    }

    Rect bounds = widget->transform;
    if (x >= bounds.x && x < bounds.x + bounds.width &&
        y >= bounds.y && y < bounds.y + bounds.height) {
        return widget;
    }

    return nullptr;
}

void InputSystem::SetHovered(Widget* widget) {
    if (m_hoveredWidget && m_hoveredWidget != widget) {
        if (static_cast<InteractionState>(m_hoveredWidget->interactionState) == InteractionState::Hovered)
            m_hoveredWidget->interactionState = InteractionState::Normal;
    }
    m_hoveredWidget = widget;
}

void InputSystem::SetCaptured(Widget* widget) {
    if (m_capturedWidget && m_capturedWidget != widget)
        m_capturedWidget->interactionState = InteractionState::Normal;
    m_capturedWidget = widget;
}

void InputSystem::Process(Widget* root, const InputState& input) {
    Widget* hit = HitTest(root, input.mouseX, input.mouseY);

    if (input.mousePressed) {
        SetHovered(hit);
        SetCaptured(hit);
        if (m_capturedWidget) {
            m_capturedWidget->interactionState = InteractionState::Pressed;
            m_capturedWidget->onClickDown.Broadcast(m_capturedWidget);
        }
    } else if (input.mouseReleased) {
        if (m_capturedWidget) {
            m_capturedWidget->onClickUp.Broadcast(m_capturedWidget);
            if (m_capturedWidget == hit)
                m_capturedWidget->onClick.Broadcast(m_capturedWidget);
            m_capturedWidget->interactionState = (m_capturedWidget == hit)
                ? InteractionState::Hovered
                : InteractionState::Normal;
            m_capturedWidget = nullptr;
        }
        SetHovered(hit);
        if (hit) hit->interactionState = InteractionState::Hovered;
    } else if (!input.mouseDown) {
        // Plain mouse move — no buttons held.
        SetHovered(hit);
        if (hit) hit->interactionState = InteractionState::Hovered;
    }
    // mouseDown without pressed/released = drag: keep captured widget Pressed, leave hover alone.
}

} // namespace lw