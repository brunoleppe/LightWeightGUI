/**
 * @file Property.h
 * @author Bruno Leppe
 * @date 7/5/2026
 */

#ifndef LIGHTWEIGHTGUI_PROPERTY_H
#define LIGHTWEIGHTGUI_PROPERTY_H

#include <functional>

namespace lw {
template <typename T>
struct Property {
private:
    T n;

public:
    std::function<void()> on_change;

    Property(T default_val = T{})
        : n(default_val) {
    }

    Property& operator=(const T& n_) {
        if (!(this->n == n_)) {
            this->n = n_;
            if (on_change) {
                on_change();
            }
        }
        return *this;
    }

    T* operator ->() {
        return &n;
    }
    T* operator ->() const{
        return &n;
    }
    operator T() const { return n; }
};
} // namespace lw

#endif // LIGHTWEIGHTGUI_PROPERTY_H
