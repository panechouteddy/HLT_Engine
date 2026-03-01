#pragma once
#include <DirectXMath.h>

using namespace DirectX;

namespace hlt_Component
{
	struct ConstantMove
	{
		static const int ID = 1;

		float move;
		float saveMove;
		XMFLOAT3 dir;
	};
}