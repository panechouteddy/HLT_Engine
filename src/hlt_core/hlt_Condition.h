#pragma once
#include "hlt_Function.h"

class hlt_Condition
{
private:
    std::function<bool()> m_Condition;

public:
    hlt_Condition() = default;
    // Take function and parameters
    template <typename... Args>
    void SetCondition(std::function<bool(Args...)> func, Args... args) { m_Condition = [=]() { return func(args...); }; }

    bool Try() { return m_Condition ? m_Condition() : false; }
};