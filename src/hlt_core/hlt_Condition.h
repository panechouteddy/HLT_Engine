#pragma once
#include "hlt_Function.h"


//template <typename... Args>
//class hlt_ConditionWrapper
//{
//private:
//	std::function<bool(Args...)> m_Condition;
//
//public:
//	hlt_Condition() = default;
//	~hlt_Condition() = default;
//
//	void SetCondition(std::function<bool(Args...)> newFunction) { m_Condition = newFunction; }
//	bool Try(Args... args) { return m_Condition(args); }
//};

//class hlt_Condition2
//{
//private:
//	hlt_Function* m_Condition;
//
//public:
//	template <typename... Args>
//	hlt_Condition2(std::function<bool(Args...)> function) { m_Condition = function; }
//
//    template <typename... Args>
//    void Set(Args... args)
//    {
//        if (m_Condition != nullptr)
//            delete m_Condition;
//
//        test = new hlt_Function<bool, args>();
//    }
//
//    template<typename... Args>
//    bool Try(Args... args) {if(m_Condition != nullptr) return m_Condition }
//};

class hlt_Condition
{
private:
    std::function<bool()> m_Condition;

public:
    hlt_Condition() = default;
    // On prend la fonction ET les arguments qu'elle doit utiliser
    template <typename... Args>
    void SetCondition(std::function<bool(Args...)> func, Args... args) { m_Condition = [=]() { return func(args...); }; }

    bool Try() { return m_Condition ? m_Condition() : false; }
};