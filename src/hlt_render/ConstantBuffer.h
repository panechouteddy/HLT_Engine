#pragma once
#include "MathHelper.h"
#include <DirectXMath.h>

using namespace DirectX;

struct ObjectConstant
{
	ObjectConstant() {};
	ObjectConstant(XMFLOAT4X4 pos) { WorldViewProj = pos;}
	XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
};

template <typename T>
class UploadBuffer;


class ConstantBuffer
{

	XMFLOAT4X4 m_world = MathHelper::Identity4x4();
protected:
	UploadBuffer<ObjectConstant>* m_pBufferConstants;
	

public:
	ConstantBuffer(Microsoft::WRL::ComPtr<ID3D12Device> Device);
	~ConstantBuffer();

	UploadBuffer<ObjectConstant>* GetBuffer() { return m_pBufferConstants;}
	XMFLOAT4X4 GetWorldMatrix() { return m_world; }
	ID3D12Resource* GetResource();
	void SetWorldMatrix(XMFLOAT4X4 entityworld) {m_world= entityworld ;}

};