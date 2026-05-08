/**
 * @file MainWIndow.h
 * @author Bruno Leppe 
 * @date 7/5/2026
 */

#ifndef LIGHTWEIGTHGUI_MAINWINDOW_H
#define LIGHTWEIGTHGUI_MAINWINDOW_H
#include "ClippingStack.h"
#include "Widget.h"
#include "raylib.h"

namespace lw {
class MainWindow;

class MainWindowRenderer : public IRenderer {
    MainWindow* m_window;

public:
    MainWindowRenderer(MainWindow* window)
        : m_window(window) {
    }

    void Refresh() override;
    void Render(const Rect& rect) override;
};


class MainWindow : public Widget {
public:
    MainWindow(const char* title, int width, int height)
        : Widget() {
        if (!IsWindowReady()) {
            InitWindow(width, height, title);
        }
        transform = {0, 0, width, height};
    }

    ~MainWindow() override {
        CloseWindow();
    }

    std::unique_ptr<IRenderer> CreateRenderer() override {
        return std::make_unique<MainWindowRenderer>(this);
    }
};

inline void MainWindowRenderer::Refresh() {
    //nothing to do here.
}

inline void MainWindowRenderer::Render(const Rect& rect) {
    ClearBackground(GRAY);
    ClippingScope clip(rect);
    for (auto& widget : m_window->GetChildren()) {
        if (widget->visible) {
            widget->GetRenderer()->Render(m_window->transform);
        }
    }
}
} // lw

#endif //LIGHTWEIGTHGUI_MAINWINDOW_H
