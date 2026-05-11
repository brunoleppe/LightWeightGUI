<!-- TOC -->
* [LightWeightGUI — Widget Architecture](#lightweightgui--widget-architecture)
  * [Overview](#overview)
  * [Directory Structure](#directory-structure)
  * [Core Types (`Types.h`)](#core-types-typesh)
  * [Property System (`Property.h`)](#property-system-propertyh)
  * [Event System (`Delegate.h`)](#event-system-delegateh)
  * [Widget (`Widget.h`)](#widget-widgeth)
    * [Class Hierarchy](#class-hierarchy)
    * [Key Members](#key-members)
    * [Invalidation](#invalidation)
    * [Renderer & Layout (Lazy Factory)](#renderer--layout-lazy-factory)
  * [Rendering (`Renderer.h/.cpp`, `ClippingStack.h`)](#rendering-rendererhcpp-clippingstackh)
    * [RenderSubsystem](#rendersubsystem)
    * [ClippingStack (`ClippingStack.h`)](#clippingstack-clippingstackh)
    * [IRenderer Interface](#irenderer-interface)
  * [Input System (`InputSystem.h/.cpp`)](#input-system-inputsystemhcpp)
    * [Responsibilities](#responsibilities)
    * [Hit Testing](#hit-testing)
    * [Input Processing (per frame)](#input-processing-per-frame)
  * [Application & Main Loop (`Application.h/.cpp`)](#application--main-loop-applicationhcpp)
    * [Per-Frame Update](#per-frame-update)
  * [Layout System (WIP)](#layout-system-wip)
  * [Design Patterns](#design-patterns)
  * [Known Gaps](#known-gaps)
<!-- TOC --> 

# LightWeightGUI — Widget Architecture

## Overview

LightWeightGUI is a lightweight, modular GUI framework built on top of [Raylib](https://www.raylib.com/). The architecture is organized around a **widget tree** with three distinct subsystems operating per frame:

1. **Layout** — compute widget positions (WIP)
2. **Input** — hit test, hover/capture state, event dispatch
3. **Rendering** — depth-first traversal, scissor clipping, draw calls

---

## Directory Structure

```
src/
├── main.cpp
├── Application.h / Application.cpp
└── GUI/
    ├── Types.h               # Rect, LwColor, Size, InteractionState, InputState
    ├── Property.h            # Property<T> with change callbacks
    ├── Delegate.h            # Delegate and MulticastDelegate event system
    ├── IRenderer.h           # IRenderer / IRenderable interfaces
    ├── Renderer.h / .cpp     # RenderSubsystem (tree traversal + clipping)
    ├── ClippingStack.h       # Scissor rectangle stack
    ├── ILayout.h             # ILayout / ILayoutable interfaces
    ├── InputSystem.h / .cpp  # Hit testing and input event dispatch
    ├── Widget.h              # Base Widget class
    └── Widgets/
        ├── Panel.h           # Basic container widget
        ├── LayoutPanel.h     # Container with linear layout (WIP)
        └── MainWindow.h      # Root window widget
```

---

## Core Types (`Types.h`)

| Type | Description |
|---|---|
| `Rect` | `{x, y, width, height}` — integer rectangle for position/size |
| `Size` | `{width, height}` — dimension pair |
| `LwColor` | Extends Raylib `Color` with `==` operator and `GetRandomColor()` |
| `InteractionState` | `Normal`, `Hovered`, `Pressed`, `Disabled` |
| `InputState` | Captures mouse position and button state per frame |

---

## Property System (`Property.h`)

`Property<T>` is a generic wrapper that fires a callback when its value changes.

```cpp
Property<T> {
    T value
    std::function<void()> on_change

    operator=(newValue)  // calls on_change only if value changed
    operator T()         // implicit conversion
    operator->()         // pointer access
}
```

Widgets use properties for all observable state. The callbacks wire property changes to invalidation:

```cpp
// In Widget constructor:
backgroundColor.on_change = [this]() { Redraw(); };   // marks needsRedraw
transform.on_change      = [this]() { Refresh(); };   // marks needsRefresh + bubbles up
```

---

## Event System (`Delegate.h`)

A type-safe, template-based event system with two flavors:

| Type | Subscribers | Return value |
|---|---|---|
| `Delegate<Ret, Args...>` | One | `Ret` |
| `MulticastDelegate<Args...>` | Many | `void` |

Supports binding free functions, member functions, and lambdas via `IDelegateHandler` subclasses (`StaticDelegate`, `MethodDelegate`, `FunctorDelegate`).

Widgets expose three multicast events:
```cpp
MulticastDelegate<Widget*> onClick;      // released over widget
MulticastDelegate<Widget*> onClickDown;  // mouse pressed
MulticastDelegate<Widget*> onClickUp;    // mouse released
```

---

## Widget (`Widget.h`)

The base class for all UI elements. Implements both `IRenderable` and `ILayoutable`.

### Class Hierarchy

```
IRenderable      ILayoutable
      \           /
       \         /
        Widget
        /     \
    Panel    MainWindow
      |
  LayoutPanel
```

### Key Members

**Properties (all `Property<T>`):**
- `transform` — position and size relative to parent
- `backgroundColor`, `foregroundColor`
- `interactionState` — `Normal / Hovered / Pressed / Disabled`
- `visible`, `enabled`

**State flags:**
- `needsRedraw` — renderer should re-draw this widget (starts `true`)
- `needsRefresh` — layout needs recomputation (starts `true`)
- `focused`

**Hierarchy:**
- `children` — `vector<unique_ptr<Widget>>` (owns children)
- `parent` — raw pointer (non-owning back-reference)

**Events:**
- `onClick`, `onClickDown`, `onClickUp` — `MulticastDelegate<Widget*>`

### Invalidation

```
Redraw()   → needsRedraw = true
Refresh()  → needsRefresh = true → parent->Refresh() (bubbles up to root)
```

`needsRedraw` is reset to `false` immediately after `Render()` is called. Widgets only re-render when a property change calls `Redraw()` or `Refresh()`.

### Renderer & Layout (Lazy Factory)

```cpp
GetRenderer() → lazy-creates via CreateRenderer() and caches
GetLayout()   → lazy-creates via CreateLayout() and caches
```

`CreateRenderer()` and `CreateLayout()` are `virtual` — subclasses override them to provide their own implementations (Template Method pattern).

---

## Rendering (`Renderer.h/.cpp`, `ClippingStack.h`)

### RenderSubsystem

Performs a **depth-first traversal** of the widget tree using an explicit stack, with deferred "exit markers" to manage clipping region push/pop symmetrically.

**Algorithm:**

```
Render(root, screenRect):
  Push screenRect onto ClippingStack
  Push initial task {root, parentAbsRect={0,0,0,0}}

  while stack not empty:
    task = pop()

    if task.isExitNode:
      ClippingStack::Pop(); continue

    skip if !widget->visible

    widgetAbs = {
      parentAbsRect.x + transform.x,
      parentAbsRect.y + transform.y,
      transform.width,
      transform.height
    }

    push exit marker for this widget
    push children in reverse order (so first child renders first)
    ClippingStack::Push(widgetAbs)   // auto-intersects with parent

    if widget->needsRedraw:
      widget->GetRenderer()->Render(parentAbsRect)
      widget->needsRedraw = false

  ClippingStack::Pop()
```

Children are pushed in **reverse** so the first child ends up on top of the stack and is processed first, matching visual depth order.

### ClippingStack (`ClippingStack.h`)

A static stack of `Rect` scissor regions backed by Raylib's `BeginScissorMode` / `EndScissorMode`.

- `Push(rect)` — intersects with the current top, begins scissor mode
- `Pop()` — restores parent rect; ends scissor mode when stack empties
- `Peek()` — returns current clip rect
- `IsCurrentVisible()` — true if clip area is non-zero

**RAII wrapper:** `ClippingScope` pushes on construction and pops on destruction, preventing mismatched push/pop.

### IRenderer Interface

```cpp
struct IRenderer {
    virtual void Render(const Rect& parentAbsRect) = 0;
};

struct IRenderable {
    virtual std::unique_ptr<IRenderer> CreateRenderer() = 0;
};
```

Each widget subclass provides its own renderer. For example, `PanelRenderer` computes its absolute rect from `parentAbsRect + transform` and calls `DrawRectangle()`.

---

## Input System (`InputSystem.h/.cpp`)

### Responsibilities

- **Hit testing** — find the topmost widget under the cursor
- **Hover state** — track which widget the mouse is over
- **Capture state** — keep a widget active during a drag operation
- **Event dispatch** — broadcast `onClick*` events

### Hit Testing

Recursive depth-first search, children tested in **reverse** (matching render order so the visually topmost widget wins):

```
HitTest(widget, x, y):
  for each child (reverse):
    result = HitTest(child, x, y)
    if result: return result
  if widget.bounds contains (x, y) and visible and enabled:
    return widget
  return nullptr
```

### Input Processing (per frame)

```
mousePressed:
  hit = HitTest(root, mouse)
  SetHovered(hit); SetCaptured(hit)
  hit.interactionState = Pressed
  hit.onClickDown.Broadcast(hit)

mouseReleased:
  captured.onClickUp.Broadcast(captured)
  if HitTest == captured:
    captured.onClick.Broadcast(captured)
  update interactionState based on hover
  clear capture

mouseMoved (no button):
  hit = HitTest(root, mouse)
  SetHovered(hit)
  hit.interactionState = Hovered

drag (button held, no press/release event):
  captured.interactionState = Pressed
```

---

## Application & Main Loop (`Application.h/.cpp`)

```cpp
class Application {
    MainWindow*      m_window;
    InputSystem      m_inputSystem;
    RenderSubsystem  m_renderSubsystem;
    // LayoutSubsystem m_layoutSubsystem; // not yet implemented
};
```

### Per-Frame Update

```
1. Capture InputState (mouse position, button down/pressed/released)
2. Layout pass       (not yet implemented)
3. m_inputSystem.Process(root, input)
4. BeginDrawing()
   m_renderSubsystem.Render(root, screenRect)
   EndDrawing()
```

Platform support: standard `while (!WindowShouldClose())` loop on desktop; `emscripten_set_main_loop` on Web.

---

## Layout System (WIP)

The layout system is structurally defined but not yet implemented.

**Interfaces (`ILayout.h`):**
```cpp
struct ILayout    { virtual void Layout() = 0; };
struct ILayoutable{ virtual std::unique_ptr<ILayout> CreateLayout() = 0; };
```

**LayoutPanel** holds a `LinearLayout` struct and a `LayoutPanelLayout` that calls `lw::Layout::Compute()` — but `Layout.h` and `LayoutSubsystem` don't exist yet.

---

## Design Patterns

| Pattern | Where used |
|---|---|
| **Composite** | Widget tree (parent/children hierarchy) |
| **Template Method** | `CreateRenderer()` / `CreateLayout()` virtual factory methods |
| **Strategy** | `IRenderer` and `ILayout` allow swappable implementations |
| **Observer** | `Property<T>` callbacks + `MulticastDelegate` events |
| **Lazy Initialization** | `GetRenderer()` / `GetLayout()` create on first access |
| **RAII** | `ClippingScope` auto-manages scissor push/pop |
| **Deferred Traversal** | `RenderSubsystem` uses explicit stack with exit markers |

---

## Known Gaps

| Area | Status |
|---|---|
| Layout system | `Layout.h` / `LayoutSubsystem` not implemented |
| `LayoutPanel` | Depends on missing `Layout::Compute()` |
| Delegate unbind | No handle-based remove/unbind API |
| Default renderer | Base `Widget` has no renderer; subclasses must override |
| Theme system | Not yet designed |
