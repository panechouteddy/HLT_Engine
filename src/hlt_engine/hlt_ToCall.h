#pragma once
#include <functional>

struct hlt_ToCall
{
	hlt_Function<void> m_Start;
	hlt_Function<void> m_Update;
	hlt_Function<void> m_Exit;
};