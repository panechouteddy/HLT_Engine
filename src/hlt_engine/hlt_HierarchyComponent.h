#pragma once
#include <vector> 
#include <DirectXMath.h>

#define NO_PARENT -1

namespace hlt_Component
{
	struct Hierarchy
	{
		static const int ID = 7;

		int parentID = NO_PARENT;
		int hierarchyDepth = 0;
		bool isRefreshed = false;

		void Reset()
		{
			parentID = NO_PARENT;
			hierarchyDepth = 0;
			isRefreshed = false;
		}
	};
}