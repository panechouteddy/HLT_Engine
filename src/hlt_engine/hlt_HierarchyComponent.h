#pragma once
#include <vector> 
#include <DirectXMath.h>

#define NO_PARENT -1

namespace hlt_Component
{
	struct Hierarchy
	{
		static const int ID = 7;

		int parentID = -1;
		int hierarchyDepth = 0;
		bool isRefreshed = false;
	};
}