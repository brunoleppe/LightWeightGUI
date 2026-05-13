//
// Created by bruno on 6/5/26.
//

#ifndef LIGHTWEIGTHGUI_APPLICATION_H
#define LIGHTWEIGTHGUI_APPLICATION_H
#include "InputSystem.h"
#include "MainWindow.h"
#include "OverlayRoot.h"
#include "Renderer.h"

namespace lw {
class Application {
#if defined(PLATFORM_WEB)
    static Application* s_app;
    static void UpdateFrame();
#endif
    MainWindow* m_window{nullptr};
    InputSystem m_inputSystem;
    LayoutSubsystem m_layoutSubsystem;
    RenderSubsystem m_renderSubsystem;
    static inline OverlayRoot m_overlayRoot;

    void Update();

public:
    static Widget* AddToOverlay(std::unique_ptr<Widget> widget);

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    Application();
    ~Application();
    MainWindow* CreateWindow(const char* title, int width, int height);
    void Run();
};
} // lw

#endif //LIGHTWEIGTHGUI_APPLICATION_H
