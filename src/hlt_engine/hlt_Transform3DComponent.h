#pragma once
#include <DirectXMath.h>

using namespace DirectX;

namespace hlt_Component
{
	struct Transform3D
	{
	public:
		static const int ID = 0;

		// POSITION
		XMFLOAT3 pos;

		// SCALE
		XMFLOAT3 sca;

		// ROTATION
		XMFLOAT3 front;
		XMFLOAT4 quaternion;
		XMFLOAT4X4 rotation;

		// DX12 TRANSFORM
		bool isWorldUpdated;
		XMFLOAT4X4 world;
	};
}