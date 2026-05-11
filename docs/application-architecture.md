<!-- TOC -->
* [Application Architecture](#application-architecture)
  * [Entry Point](#entry-point)
  * [Application Class](#application-class)
  * [Per-Frame Pipeline](#per-frame-pipeline)
  * [Subsystems](#subsystems)
    * [LayoutSubsystem](#layoutsubsystem)
    * [InputSystem](#inputsystem)
    * [RenderSubsystem](#rendersubsystem)
  * [MainWindow](#mainwindow)
  * [Platform Variants](#platform-variants)
  * [Known Gaps](#known-gaps)
<!-- TOC -->

# Application Architecture

## Entry Point

`main.cpp` constructs an `Application`, builds the widget tree, binds events, then calls `Run()`.

```cpp
lw::Application app;
lw::MainWindow* w = app.CreateWindow("LightWeightGUI", 600, 450);
// ... add widgets, bind events ...
app.Run();
```

---

## Application Class

```
Application
├── MainWindow*        m_window          (owns, created via CreateWindow)
├── InputSystem        m_inputSystem
├── LayoutSubsystem    m_layoutSubsystem 
└── RenderSubsystem    m_renderSubsystem
```

Non-copyable (`= delete` on copy constructor and assignment).
Constructor sets `FLAG_MSAA_4X_HINT` before any window is created.
Destructor calls `delete m_window` → triggers `CloseWindow()`.

---

## Per-Frame Pipeline

`Application::Update()` runs every frame in this order:

```
1. Capture InputState
   ├── GetMousePosition()
   ├── IsMouseButtonDown(LEFT)
   ├── IsMouseButtonPressed(LEFT)
   └── IsMouseButtonReleased(LEFT)

2. Layout pass
   └── m_layoutSubsystem.Layout(m_window)

3. Input pass
   └── m_inputSystem.Process(m_window, input)

4. Render pass
   ├── BeginDrawing()
   ├── m_renderSubsystem.Render(m_window, screenRect)
   ├── DrawText(mouse position debug)
   └── EndDrawing()
```

---

## Subsystems

### LayoutSubsystem

Computes widget positions before input and rendering.
`LayoutSubsystem` is declared in `Application.h`.
`LayoutPanel::Layout()` calls `lw::Layout::Compute()`.

### InputSystem

Stateful — tracks two widget pointers across frames:

| Field | Role |
|---|---|
| `m_hoveredWidget` | Widget under mouse |
| `m_capturedWidget` | Widget held during drag |

`Process(root, input)` runs hit testing and dispatches `onClick`, `onClickDown`, `onClickUp` events.

### RenderSubsystem

Traverses widget tree depth-first via explicit stack.
Manages scissor clipping via `ClippingStack`.
Calls `widget->GetRenderer()->Render(parentAbsRect)` when `widget->needsRedraw` is true, then resets it to `false`.

---

## MainWindow

`MainWindow` is a `Widget` subclass that owns the OS window.

```cpp
MainWindow(title, width, height)
  → InitWindow(width, height, title)    // if not already open
  → transform = {0, 0, width, height}

~MainWindow()
  → CloseWindow()
```

`MainWindowRenderer::Render()` calls `ClearBackground(GRAY)`.
Children are handled by `RenderSubsystem`, not by `MainWindow` itself.

---

## Platform Variants

| Platform | Loop mechanism |
|---|---|
| Desktop | `while (!WindowShouldClose()) { Update(); }` |
| Web (Emscripten) | `emscripten_set_main_loop(UpdateFrame, 0, 1)` |

Web build stores `Application*` in a static pointer (`s_app`) so the C-style callback `UpdateFrame()` can reach the instance.

---

## Known Gaps

| Item | Status |
|---|---|
