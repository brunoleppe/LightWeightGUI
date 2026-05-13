#include <iostream>
#include <raylib.h>

#include "Application.h"
#include "Label.h"
#include "Layout.h"
#include "LayoutPanel.h"
#include "GUI/Widgets/Panel.h"

int main(int argc, char* argv[]) {
    lw::Application app;
    lw::MainWindow* w = app.CreateWindow("LightWeightGUI", 600, 450);
    auto* p1 = static_cast<lw::LayoutPanel*>(w->AddWidget(std::make_unique<lw::LayoutPanel>()));
    auto* p2 = static_cast<lw::Panel*>(w->AddWidget(std::make_unique<lw::Panel>()));
    p1->transform = {0, 0, 200, 450};
    p1->name = "Panel1";
    p2->transform = {200, 0, 200, 450};
    p2->name = "Panel2";

    auto* p4 = static_cast<lw::Panel*>(p1->AddWidget(std::make_unique<lw::Panel>()));
    auto* p5 = static_cast<lw::Panel*>(p1->AddWidget(std::make_unique<lw::Panel>()));
    auto* p6 = static_cast<lw::Panel*>(p1->AddWidget(std::make_unique<lw::Panel>()));

    p4->name = "Panel4";
    p5->name = "Panel5";
    p6->name = "Panel6";
    p4->minSize = {50, 50};
    p5->minSize = {50, 50};
    p6->minSize = {50, 50};

    lw::Label* l1 = static_cast<lw::Label*>(p4->AddWidget(std::make_unique<lw::Label>()));
    lw::Label* l2 = static_cast<lw::Label*>(p5->AddWidget(std::make_unique<lw::Label>()));
    lw::Label* l3 = static_cast<lw::Label*>(p6->AddWidget(std::make_unique<lw::Label>()));
    l1->Position(20, 20);
    l2->Position(20, 20);
    l3->Position(20, 20);

    p4->onClick.Bind([&p6, &p2](lw::Widget* w) {
        p6->visible = !p6->visible;
        p2->backgroundColor = lw::LwColor::GetRandomColor();
    });

    auto* over1 = lw::Application::AddToOverlay(std::make_unique<lw::Panel>());
    over1->transform = {200, 175, 50, 50};
    over1->name = "Overlay1";

    app.Run();
    return 0;
}
