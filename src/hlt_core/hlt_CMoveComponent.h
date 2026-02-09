#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct hlt_CMoveComponent
{
	static const int ID = 1;

	float move;
	XMFLOAT3 dir;
};