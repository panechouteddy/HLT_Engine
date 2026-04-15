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

		void Reset()
		{
			move = 0.f;
			saveMove = 0.f;
			dir = XMFLOAT3(0, 0, 0);
		}
	};
}