/**
 * @file ISizeStrategy.h
 * @author Bruno Leppe 
 * @date 12/5/2026
 */

#ifndef LIGHTWEIGTHGUI_ISIZESTRATEGY_H
#define LIGHTWEIGTHGUI_ISIZESTRATEGY_H
#include <memory>

#include "Types.h"

namespace lw {
class Widget;

class IMeasurer {
public:
    virtual ~IMeasurer() = default;
    virtual Size ComputeMinSize(const Widget* widget) = 0;
};

class IMeasurable {
public:
    virtual ~IMeasurable() = default;
    virtual std::unique_ptr<IMeasurer> CreateMeasurer() = 0;
};
} // lw

#endif //LIGHTWEIGTHGUI_ISIZESTRATEGY_H
