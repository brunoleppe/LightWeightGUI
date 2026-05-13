/**
 * @file ILayout.h
 * @author Bruno Leppe 
 * @date 8/5/2026
 */

#ifndef LIGHTWEIGTHGUI_ILAYOUT_H
#define LIGHTWEIGTHGUI_ILAYOUT_H

#include <memory>

namespace lw {
class Widget;

class ILayout {
public:
    virtual ~ILayout() = default;
    virtual void Layout() = 0;
};

class ILayoutable {
public:
    virtual ~ILayoutable() = default;
    virtual std::unique_ptr<ILayout> CreateLayout() = 0;
};
} // lw

#endif //LIGHTWEIGTHGUI_ILAYOUT_H
