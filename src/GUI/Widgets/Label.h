/**
 * @file Label.h
 * @author Bruno Leppe 
 * @date 12/5/2026
 */

#ifndef LIGHTWEIGTHGUI_LABEL_H
#define LIGHTWEIGTHGUI_LABEL_H
#include "Theme.h"
#include "Widget.h"

namespace lw {
class Label;

class LabelRenderer : public IRenderer {
    Label* m_label;

public:
    LabelRenderer(Label* label)
        : m_label(label) {
    }

    void Render(const Rect& rect) override;
};

class LabelMeasurer : public IMeasurer {
    Label* m_label;

public:
    LabelMeasurer(Label* label)
        : m_label(label) {
    }

    LwSize ComputeMinSize() const override;
};

class Label : public Widget {
public:
    LwSize GetTextSize() {
        Vector2 s = MeasureTextEx(font, text->c_str(), font->fontSize, font->spacing);
        Rect newTransform = {transform->x, transform->y, (int)s.x, (int)s.y};
        transform = newTransform;
        return {transform->width, transform->height};
    }

    std::unique_ptr<IRenderer> CreateRenderer() override {
        return std::make_unique<LabelRenderer>(this);
    }

    Label()
        : text("Label"), font({GetFontDefault(), 12.f, 1.f}) {
        auto OnTextChanged = [this]() {
            needsRedraw = true;
            GetTextSize();
        };
        text.on_change = OnTextChanged;
        font.on_change = OnTextChanged;
        GetTextSize();
    }

    std::unique_ptr<IMeasurer> CreateMeasurer() override {
        return std::make_unique<LabelMeasurer>(this);
    }

    Property<std::string> text;
    Property<LwFont> font;
};


inline void LabelRenderer::Render(const Rect& rect) {
    DrawTextEx(m_label->font, m_label->text->c_str(), {(float)rect.x, (float)rect.y}, m_label->font->fontSize,
               m_label->font->spacing,
               Theme::Get().textMaterial.textColor);
}

inline LwSize LabelMeasurer::ComputeMinSize() const {
    return m_label->GetTextSize();
}
} // lw

#endif //LIGHTWEIGTHGUI_LABEL_H
