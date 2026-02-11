#pragma once
#include <DirectXMath.h>

using namespace DirectX;

namespace hlt_Component
{
	struct hlt_CMoveComponent
	{
		static const int ID = 1;

		float move;
		XMFLOAT3 dir;
	};
}