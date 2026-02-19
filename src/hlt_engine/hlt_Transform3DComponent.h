#pragma once
#include <hlt_core/hlt_Transform3D.h>

using namespace DirectX;

namespace hlt_Component
{
	struct Transform3D
	{
		static const int ID = 0;

		hlt_Transform3D transform;
	};
}