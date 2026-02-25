#pragma once

#include <DirectXMath.h>
using namespace DirectX;

struct hlt_Transform3D
{
public:
	// POSITION
	XMFLOAT3 pos{ 0, 0, 0 };

	// SCALE
	XMFLOAT3 sca{ 1, 1, 1 };

	// ROTATION
	XMFLOAT3 front;
	XMFLOAT4 quaternion;
	XMFLOAT4X4 rotation;

	// DX12 TRANSFORM
	bool isWorldUpdated = false;
	XMFLOAT4X4 world;
};