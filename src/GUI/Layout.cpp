/**
 * @file Layout.cpp
 * @author Bruno Leppe
 * @date 8/5/2026
 */

#include "Layout.h"

#include <algorithm>
#include <queue>
#include <stack>

#include "Widget.h"

namespace lw {
namespace Layout {
std::vector<Rect> Compute(const Rect& parent, const LinearLayout& desc) {
    if (desc.count <= 0) return {};

    std::vector<Rect> rects;
    rects.reserve(desc.count);

    const int innerW = parent.width - 2 * desc.padding;
    const int innerH = parent.height - 2 * desc.padding;

    if (desc.horizontal) {
        const int itemW = (innerW - desc.spacing * (desc.count - 1)) / desc.count;
        for (int i = 0; i < desc.count; ++i) {
            rects.push_back({
                parent.x + desc.padding + i * (itemW + desc.spacing),
                parent.y + desc.padding,
                itemW,
                innerH
            });
        }
    } else {
        const int itemH = (innerH - desc.spacing * (desc.count - 1)) / desc.count;
        for (int i = 0; i < desc.count; ++i) {
            rects.push_back({
                parent.x + desc.padding,
                parent.y + desc.padding + i * (itemH + desc.spacing),
                innerW,
                itemH
            });
        }
    }

    return rects;
}

std::vector<Rect> Compute(const Rect& parent, const GridLayout& desc) {
    if (desc.count <= 0 || desc.columns <= 0) return {};

    std::vector<Rect> rects;
    rects.reserve(desc.count);

    const int cols = desc.columns;
    const int rows = (desc.count + cols - 1) / cols;

    const int innerW = parent.width - 2 * desc.padding;
    const int innerH = parent.height - 2 * desc.padding;

    const int cellW = (innerW - desc.spacing * (cols - 1)) / cols;
    const int cellH = (innerH - desc.spacing * (rows - 1)) / rows;

    for (int i = 0; i < desc.count; ++i) {
        const int col = i % cols;
        const int row = i / cols;
        rects.push_back({
            parent.x + desc.padding + col * (cellW + desc.spacing),
            parent.y + desc.padding + row * (cellH + desc.spacing),
            cellW,
            cellH
        });
    }

    return rects;
}
} // namespace Layout
void DefaultLayout::Layout() {
    // do nothing
}

void LayoutSubsystem::Layout(Widget* root) {
    //traverse the widget tree
    m_queue.clear();
    if (!root) return;

    // Standard BFS to visit every visible node
    std::queue<std::pair<Widget*, int>> traversalStack;
    traversalStack.push({root, 0});

    while (!traversalStack.empty()) {
        auto [current, depth] = traversalStack.front();
        traversalStack.pop();

        if (!current->visible) { continue; }
        if (current->needsRefresh) {
            m_queue.push_back({current, depth});
        }

        for (auto& child : current->GetChildren()) {
            traversalStack.push({child.get(), depth + 1});
        }
    }

    // Sort by depth so we can process linearly
    std::sort(m_queue.begin(), m_queue.end(), [](const LayoutEntry& a, const LayoutEntry& b) {
        return a.depth < b.depth;
    });

    for (auto& entry : m_queue) {
        entry.widget->GetLayout()->Layout();
    }
}
} // namespace lw
