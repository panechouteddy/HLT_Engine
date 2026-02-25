#pragma once
#include <vector> 
#include <DirectXMath.h>

#define NO_PARENT -1

namespace hlt_Component
{
	struct Hierarchy
	{
		int parentID = -1;
	};
}