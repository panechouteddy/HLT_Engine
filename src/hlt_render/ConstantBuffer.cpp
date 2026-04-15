#include "pch.h"
#include "UploadBuffer.h"


ConstantBuffer::ConstantBuffer(Microsoft::WRL::ComPtr<ID3D12Device>  Device)
{
	m_pBufferConstants = new UploadBuffer<ObjectConstant>(Device.Get(), 1, true);
}

ConstantBuffer::~ConstantBuffer()
{
	delete m_pBufferConstants;
}


ID3D12Resource* ConstantBuffer::GetResource()
{
	return m_pBufferConstants->Resource();
}


