//
// Created by bruno on 6/5/26.
//

#ifndef LIGHTWEIGTHGUI_APPLICATION_H
#define LIGHTWEIGTHGUI_APPLICATION_H
#include "GUI/InputSystem.h"
#include "GUI/MainWindow.h"

namespace lw {


class Application {
#if defined(PLATFORM_WEB)
    static Application* s_app;
    static void UpdateFrame();
#endif
    MainWindow*  m_window{nullptr};
    InputSystem  m_inputSystem;

public:
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    Application();
    ~Application();
    MainWindow* CreateWindow(const char* title, int width, int height);
    void Run();
};
} // lw

#endif //LIGHTWEIGTHGUI_APPLICATION_H
