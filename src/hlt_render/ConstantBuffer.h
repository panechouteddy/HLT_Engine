#pragma once
#pragma once
#include "MathHelper.h"
#include "d3dUtil.h"
#include "UploadBuffer.h"
#include <DirectXMath.h>
using namespace DirectX;



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
	ID3D12Resource* GetResource();
	void SetWorldMatrix(XMFLOAT4X4 entityworld) {m_world= entityworld ;}

};