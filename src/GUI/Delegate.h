//
// Created by bruno on 5/5/26.
//

#ifndef LIGHTWEIGHTGUI_DELEGATE_H
#define LIGHTWEIGHTGUI_DELEGATE_H
#include <functional>
#include <memory>
#include <vector>

namespace lw {
template <typename Ret, typename... Args>
class IDelegateHandler {
public:
    virtual ~IDelegateHandler() = default;
    virtual Ret operator()(Args... args) = 0;
};

template <typename Ret, typename... Args>
class StaticDelegate : public IDelegateHandler<Ret, Args...> {
    using FuncPtr = Ret (*)(Args...);
    FuncPtr m_func;

public:
    StaticDelegate(FuncPtr func)
        : m_func(func) {
    }

    Ret operator()(Args... args) override {
        return m_func(args...);
    }
};

template <typename T, typename Ret, typename... Args>
class MethodDelegate : public IDelegateHandler<Ret, Args...> {
    using MethodPtr = Ret (T::*)(Args...);
    T* m_instance;
    MethodPtr m_method;

public:
    MethodDelegate(T* instance, MethodPtr method)
        : m_instance(instance), m_method(method) {
    }

    Ret operator()(Args... args) override {
        return (m_instance->*m_method)(args...);
    }
};

template <typename F, typename Ret, typename... Args>
class FunctorDelegate : public IDelegateHandler<Ret, Args...> {
    F functor; // Stores the lambda or functor

public:
    // Use universal reference (F&&) to handle both lvalue and rvalue lambdas
    explicit FunctorDelegate(F&& f)
        : functor(std::forward<F>(f)) {
    }

    // The Execute call simply invokes the stored functor
    Ret operator()(Args... args) override {
        // We check if it's still valid/callable
        return functor(std::forward<Args>(args)...);
    }
};


template <typename Ret, typename... Args>
class Delegate {
    std::unique_ptr<IDelegateHandler<Ret, Args...>> m_handler;

public:
    Ret operator()(Args... args) {
        return Execute(args...);
    }

    Ret Execute(Args... args) {
        if (m_handler) {
            return m_handler->operator()(args...);
        }
        return Ret();
    }

    void Bind(void (*func)(Args...)) {
        m_handler = std::make_unique<StaticDelegate<Ret, Args...>>(func);
    }

    template <typename T>
    void Bind(T* obj, void (T::*method)(Args...)) {
        m_handler = std::make_unique<MethodDelegate<T, Ret, Args...>>(obj, method);
    }

    template <typename F>
    void Bind(F&& lambda) {
        m_handler = std::make_unique<FunctorDelegate<F, Ret, Args...>>(std::forward<F>(lambda));
    }
};

template <typename... Args>
class MulticastDelegate {
    std::vector<std::unique_ptr<IDelegateHandler<void, Args...>>> m_handlers;

public:
    void operator()(Args... args) {
        Broadcast(args...);
    }

    void Broadcast(Args... args) {
        for (const auto& handler : m_handlers) {
            handler->operator()(args...);
        }
    }

    void Bind(void (*func)(Args...)) {
        m_handlers.push_back(std::make_unique<StaticDelegate<void, Args...>>(func));
    }

    template <typename T>
    void Bind(T* obj, void (T::*method)(Args...)) {
        m_handlers.push_back(std::make_unique<MethodDelegate<T, void, Args...>>(obj, method));
    }

    template <typename F>
    void Bind(F&& lambda) {
        m_handlers.push_back(std::make_unique<FunctorDelegate<F, void, Args...>>(std::forward<F>(lambda)));
    }
};
} // lw

#endif //LIGHTWEIGHTGUI_DELEGATE_H
