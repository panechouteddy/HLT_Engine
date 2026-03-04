#pragma once
#include <functional>

template <typename returnType, typename... Args>
class hlt_Function
{
private:
	std::function<returnType(Args...)> m_Function;

public:
	hlt_Function() = default;

	void Set(std::function<returnType(Args...)> function) { m_Function = function; }

	template<typename T>
	void Set(T* instance, returnType(T::* method)(Args...)) {
		m_Function = [instance, method](Args... args) -> returnType {
			return (instance->*method)(std::forward<Args>(args)...);
			};
	}

	returnType Execute(Args... args) { 
        if (m_Function) 
            return m_Function(std::forward<Args>(args)...); 
        // Optionally handle the case where m_Function is not set
        if constexpr (!std::is_void_v<returnType>)
            return returnType{};
	}
};