#pragma once
#include <functional>

struct hlt_Fun
{

};

template <typename T>
struct hlt_Function : public hlt_Fun
{
	T::std::function<void()> pFunction = nullptr;
	T* instance = nullptr;
};


struct hlt_ToCall
{
public:
	hlt_Fun* pStart;
	hlt_Fun* pUpdate;
	hlt_Fun* pExit;
};
