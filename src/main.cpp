#include <iostream>
#include <raylib.h>

#include "Application.h"
#include "GUI/Panel.h"

constexpr int screenWidth = 800;
constexpr int screenHeight = 450;
float rotation = 0.0f;

void UpdateFrame() {
    // Update
    //----------------------------------------------------------------------------------
    rotation += 0.2f;
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(GRAY);

    DrawText("some basic shapes available on raylib", 20, 20, 20, DARKGRAY);

    // Circle shapes and lines
    DrawCircle(screenWidth / 5, 120, 35, DARKBLUE);
    DrawCircleGradient(screenWidth / 5.0f, 220.0f, 60, GREEN, SKYBLUE);
    DrawCircleLines(screenWidth / 5, 340, 80, DARKBLUE);
    DrawEllipse(screenWidth / 5, 120, 25, 20, YELLOW);
    DrawEllipseLines(screenWidth / 5, 120, 30, 25, YELLOW);

    // Rectangle shapes and lines
    DrawRectangle(screenWidth / 4 * 2 - 60, 100, 120, 60, RED);
    DrawRectangleGradientH(screenWidth / 4 * 2 - 90, 170, 180, 130, MAROON, GOLD);
    DrawRectangleLines(screenWidth / 4 * 2 - 40, 320, 80, 60, ORANGE); // NOTE: Uses QUADS internally, not lines

    // Triangle shapes and lines
    DrawTriangle((Vector2){screenWidth / 4.0f * 3.0f, 80.0f},
                 (Vector2){screenWidth / 4.0f * 3.0f - 60.0f, 150.0f},
                 (Vector2){screenWidth / 4.0f * 3.0f + 60.0f, 150.0f}, VIOLET);

    DrawTriangleLines((Vector2){screenWidth / 4.0f * 3.0f, 160.0f},
                      (Vector2){screenWidth / 4.0f * 3.0f - 20.0f, 230.0f},
                      (Vector2){screenWidth / 4.0f * 3.0f + 20.0f, 230.0f}, DARKBLUE);

    // Polygon shapes and lines
    DrawPoly((Vector2){screenWidth / 4.0f * 3, 330}, 6, 80, rotation, BROWN);
    DrawPolyLines((Vector2){screenWidth / 4.0f * 3, 330}, 6, 90, rotation, BROWN);
    DrawPolyLinesEx((Vector2){screenWidth / 4.0f * 3, 330}, 6, 85, rotation, 6, BEIGE);

    // NOTE: We draw all LINES based shapes together to optimize internal drawing,
    // this way, all LINES are rendered in a single draw pass
    DrawLine(18, 42, screenWidth - 18, 42, BLACK);
    EndDrawing();
    //----------------------------------------------------------------------------------

}

int main(int argc, char* argv[]) {
    lw::Application app;
    lw::MainWindow* w = app.CreateWindow("LightWeightGUI", 600, 450);
    auto* p1 = static_cast<lw::Panel*>(w->AddWidget(std::make_unique<lw::Panel>()));
    auto* p2 = static_cast<lw::Panel*>(w->AddWidget(std::make_unique<lw::Panel>()));
    auto* p3 = static_cast<lw::Panel*>(w->AddWidget(std::make_unique<lw::Panel>()));
    p1->transform = {0,0,200,450};
    p1->backgroundColor = {RED};
    p2->transform = {200,0,200,450};
    p2->backgroundColor = {GREEN};
    p3->transform = {400,0,200,450};
    p3->backgroundColor = {BLUE};

    auto* p4 = static_cast<lw::Panel*>(p1->AddWidget(std::make_unique<lw::Panel>()));
    p4->transform = {100,100,200,200};
    p4->backgroundColor = {YELLOW};

    w->onClick.Bind([](lw::Widget* w) {
        std::cout << "Widget clicked: " << w << std::endl;
    });
    app.Run();
    return 0;
}
