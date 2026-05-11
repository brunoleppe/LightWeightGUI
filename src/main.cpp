#include <iostream>
#include <raylib.h>

#include "Application.h"
#include "Layout.h"
#include "LayoutPanel.h"
#include "GUI/Widgets/Panel.h"

int main(int argc, char* argv[]) {
    lw::Application app;
    lw::MainWindow* w = app.CreateWindow("LightWeightGUI", 600, 450);
    auto* p1 = static_cast<lw::LayoutPanel*>(w->AddWidget(std::make_unique<lw::LayoutPanel>()));
    auto* p2 = static_cast<lw::Panel*>(w->AddWidget(std::make_unique<lw::Panel>()));
    auto* p3 = static_cast<lw::Panel*>(w->AddWidget(std::make_unique<lw::Panel>()));
    p1->transform = {0,0,200,450};
    p1->backgroundColor = {RED};
    p2->transform = {200,0,200,450};
    p2->backgroundColor = {GREEN};
    p3->transform = {400,0,200,450};
    p3->backgroundColor = {BLUE};


    auto* p4 = static_cast<lw::Panel*>(p1->AddWidget(std::make_unique<lw::Panel>()));
    p4->backgroundColor = {YELLOW};
    auto* p5 = static_cast<lw::Panel*>(p1->AddWidget(std::make_unique<lw::Panel>()));
    p5->backgroundColor = {YELLOW};
    auto* p6 = static_cast<lw::Panel*>(p1->AddWidget(std::make_unique<lw::Panel>()));
    p6->backgroundColor = {YELLOW};

    p4->onClick.Bind([&p6, &p2](lw::Widget* w) {
        p6->visible = !p6->visible;
        p2->backgroundColor = lw::LwColor::GetRandomColor();
    });

    w->onClick.Bind([](lw::Widget* w) {
        std::cout << "Widget clicked: " << w << std::endl;
    });
    app.Run();
    return 0;
}
