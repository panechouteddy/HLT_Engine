#pragma once
#include <hlt_core/hlt_Box.h>

namespace hlt_Component
{
	struct BoxCollider
	{
		static const int ID = 4;

		hlt_Box::Box3D_AABB box;
		bool isColliding = false;
	};
}