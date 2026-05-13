/**
 * @file Layout.h
 * @author Bruno Leppe
 * @date 8/5/2026
 */

#ifndef LIGHTWEIGHTGUI_LAYOUT_H
#define LIGHTWEIGHTGUI_LAYOUT_H

#include "Types.h"
#include <vector>

#include "ILayout.h"

namespace lw {
struct LinearLayout {
    int count;
    int padding = 0;
    int spacing = 0;
    bool horizontal = true;
};

struct GridLayout {
    int count;
    int columns;
    int padding = 0;
    int spacing = 0;
};

class DefaultLayout : public ILayout {
public:
    void Layout() override;
};

class LayoutSubsystem {
    struct LayoutEntry {
        Widget* widget;
        int depth;
    };

    std::vector<LayoutEntry> m_queue;

public:
    void Layout(Widget* widget);
};

namespace Layout {
// Divides parent into `desc.count` equal slots along one axis.
std::vector<Rect> Compute(const Rect& parent, const LinearLayout& desc);

// Divides parent into a uniform grid of `desc.columns` columns,
// filling rows top-to-bottom until all `desc.count` slots are placed.
std::vector<Rect> Compute(const Rect& parent, const GridLayout& desc);
} // namespace Layout
} // namespace lw

#endif // LIGHTWEIGHTGUI_LAYOUT_H
