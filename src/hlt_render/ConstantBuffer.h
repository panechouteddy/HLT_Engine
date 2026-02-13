#pragma once
#pragma once
#include "MathHelper.h"
#include "d3dApp.h"
#include "UploadBuffer.h"
#include <DirectXMath.h>
using namespace DirectX;

struct CBfView
{
	XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
};

class ConstantBuffer
{

	XMFLOAT4X4 m_world = MathHelper::Identity4x4();
protected:
	UploadBuffer<CBfView>* m_bufferConstats;
	

public:
	ConstantBuffer(Microsoft::WRL::ComPtr<ID3D12Device> Device);
	~ConstantBuffer();

	UploadBuffer<CBfView>* GetBuffer();
	XMFLOAT4X4 GetWorldMatrix();
	ID3D12Resource* GetResource();
	void SetWorldMatrix(XMFLOAT4X4 entityworld) {m_world= entityworld ;}
};