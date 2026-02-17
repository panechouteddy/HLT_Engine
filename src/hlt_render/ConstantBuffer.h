#pragma once
#pragma once
#include "MathHelper.h"
#include "d3dApp.h"
#include "UploadBuffer.h"
#include <DirectXMath.h>
using namespace DirectX;

struct ObjectConstant
{
	XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
};

class ConstantBuffer
{

	XMFLOAT4X4 m_world = MathHelper::Identity4x4();
protected:
	UploadBuffer<ObjectConstant>* m_bufferConstats;
	

public:
	ConstantBuffer(Microsoft::WRL::ComPtr<ID3D12Device> Device);
	~ConstantBuffer();

	UploadBuffer<ObjectConstant>* GetBuffer() { return m_bufferConstats;}
	XMFLOAT4X4 GetWorldMatrix() { return m_world; }
	ID3D12Resource* GetResource() { return m_bufferConstats->Resource();}
	void SetWorldMatrix(XMFLOAT4X4 entityworld) {m_world= entityworld ;}

};