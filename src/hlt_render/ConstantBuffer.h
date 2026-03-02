#pragma once
#include "MathHelper.h"
#include <DirectXMath.h>

//#include "UploadBuffer.h"
using namespace DirectX;

struct ObjectConstant
{
	ObjectConstant() {};
	ObjectConstant(XMFLOAT4X4 pos) { WorldViewProj = pos;}
	XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
	XMFLOAT4X4 World = MathHelper::Identity4x4();
	XMFLOAT4X4 TexTransform = MathHelper::Identity4x4();
};

template <typename T>
class UploadBuffer;


class ConstantBuffer
{

protected:
	UploadBuffer<ObjectConstant>* m_pBufferConstants;
	

public:
	XMFLOAT4X4 World = MathHelper::Identity4x4();
	ConstantBuffer(Microsoft::WRL::ComPtr<ID3D12Device> Device);
	~ConstantBuffer();

	UploadBuffer<ObjectConstant>* GetBuffer() { return m_pBufferConstants;}
	ID3D12Resource* GetResource();

};