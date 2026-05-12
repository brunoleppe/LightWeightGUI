/**
 * @file InputSystem.cpp
 * @author Bruno Leppe
 * @date 7/5/2026
 */

#include "InputSystem.h"

#include <iostream>

#include "Widget.h"

namespace lw {
Rect InputSystem::GetIntersection(const Rect& r1, const Rect& r2) {
    int x = std::max(r1.x, r2.x);
    int y = std::max(r1.y, r2.y);
    int w = std::min(r1.x + r1.width, r2.x + r2.width) - x;
    int h = std::min(r1.y + r1.height, r2.y + r2.height) - y;

    // If they don't overlap, return a zero rect
    if (w < 0 || h < 0) return {0, 0, 0, 0};
    return {x, y, w, h};
}

Widget* InputSystem::HitTest(Widget* widget, int x, int y, const Rect& parentAbsRect, const Rect& currentClip) {
    if (!widget->visible || !widget->enabled) return nullptr;

    Rect absBounds{
        parentAbsRect.x + widget->transform->x,
        parentAbsRect.y + widget->transform->y,
        widget->transform->width,
        widget->transform->height
    };

    Rect nextClip = GetIntersection(currentClip, absBounds);
    bool mouseInClip = (x >= nextClip.x && x < nextClip.x + nextClip.width &&
                        y >= nextClip.y && y < nextClip.y + nextClip.height);

    if (!mouseInClip) return nullptr;

    const auto& children = widget->GetChildren();
    for (auto it = children.rbegin(); it != children.rend(); ++it) {
        if (Widget* hit = HitTest(it->get(), x, y, absBounds, nextClip)) {
            return hit;
        }
    }

    return widget;
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

void InputSystem::Process(Widget* overlay, Widget* root, const InputState& input, const Rect& rootRect) {
    Widget* hit = nullptr;
    if (!overlay->GetChildren().empty()) {
        hit = HitTest(overlay, input.mouseX, input.mouseY, rootRect, rootRect);
    }
    if (hit == nullptr || hit == overlay) {
        hit = HitTest(root, input.mouseX, input.mouseY, rootRect, rootRect);
    }

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
            if (m_capturedWidget == hit) {
                m_capturedWidget->onClick.Broadcast(m_capturedWidget);
#ifndef NDEBUG
                std::cout << "Clicked: " << m_capturedWidget->name << std::endl;
#endif
            }
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
