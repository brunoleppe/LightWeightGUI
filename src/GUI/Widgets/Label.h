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

class Label : public Widget {

public:
    void GetTextSize() {
        Vector2 s = MeasureTextEx(font, text->c_str(), font->fontSize, font->spacing);
        transform = {0, 0, (int)s.x, (int)s.y};
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

    Property<std::string> text;
    Property<LwFont> font;
};


inline void LabelRenderer::Render(const Rect& rect) {
    DrawTextEx(m_label->font, m_label->text->c_str(), {(float)rect.x, (float)rect.y}, m_label->font->fontSize, m_label->font->spacing,
               Theme::Get().textMaterial.textColor);
}
} // lw

#endif //LIGHTWEIGTHGUI_LABEL_H
