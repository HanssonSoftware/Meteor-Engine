/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <vector>
#include <functional>

/** Simple aliased function. */
template <typename ... Args>
struct Delegate
{
private:
    typedef std::function<void(Args...)> PrivDelegate;
public:
    constexpr Delegate() {};

    template <class T, class Obj>
    void Subscribe(T&& Func, Obj* Object)
    {
        PrivDelegate Function = [Func, Object](Args... args) {
            (Object->*Func)(args...);
            };
        callbacks.emplace_back(Function);
    }

    template <class Obj>
    void UnSubscribe(Obj* Object)
    {
        for (PrivDelegate& cb : callbacks)
        {
            if (cb)
            {

            }
        }
    }

    void Invoke(Args... args)
    {
        for (PrivDelegate& a : callbacks)
        {
            a(args...);
        }
    }

private:
    std::vector<PrivDelegate> callbacks;
};
