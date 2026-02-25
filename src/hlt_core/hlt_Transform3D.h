#pragma once

#include <DirectXMath.h>
using namespace DirectX;

class hlt_Transform3D
{
public:
	// POSITION
	XMFLOAT3 pos{ 0, 0, 0 };

	// SCALE
	XMFLOAT3 sca{ 1, 1, 1 };

	// ROTATION
	XMFLOAT3 front{ 0, 0, 1 };
	XMFLOAT3 right{ 1, 0, 0 };
	XMFLOAT3 up{ 0, 1, 0 };
	XMFLOAT4 quaternion{ 0.f, 0.f, 0.f, 1.f };
	XMFLOAT4X4 rotation;

	// DX12 TRANSFORM
	bool isWorldUpdated = false;
	XMFLOAT4X4 world;

public:
	// POSITION
	void Move(float distance);
	void Move(float distance, XMFLOAT3 dir);

	// SCALE
	void SetScale(float scale);

	// ROTATION
	void ResetRotation();
	void Identity();
	void SetRotation(hlt_Transform3D& transform);
	void AddYPR(FXMVECTOR ypr);
	void AddYPR(float yaw, float pitch, float raw);
	void SetYPR(FXMVECTOR ypr);

	void OrbitAround();
};