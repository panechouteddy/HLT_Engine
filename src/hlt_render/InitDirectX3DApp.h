#pragma once
#include "D3DApp.h"

//#include <array>

//#include "ConstantBuffer.h"

using namespace DirectX;

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;



class InitDirectX3DApp : public D3DApp
{
public:
	InitDirectX3DApp(hlt_Window* window);
	~InitDirectX3DApp();

	virtual bool Initialize() override;

	ConstantBuffer* m_CBobject;

	virtual void OnResize() override;
	virtual void Update() override;
	virtual void Draw() override;


	float mTheta = 1.5f * XM_PI;
	float mPhi = XM_PIDIV4;
	float mRadius = 5.0f;

	XMFLOAT4X4 m_pos = MathHelper::Identity4x4();

};
