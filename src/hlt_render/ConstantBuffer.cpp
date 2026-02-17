#include "pch.h"
#include "ConstantBuffer.h"


ConstantBuffer::ConstantBuffer(Microsoft::WRL::ComPtr<ID3D12Device>  Device)
{
	m_bufferConstats = new UploadBuffer<ObjectConstant>(Device.Get(), 1, true);
}

ConstantBuffer::~ConstantBuffer()
{
	delete m_bufferConstats;
}

ID3D12Resource* ConstantBuffer::GetResource()
{
	return m_bufferConstats->Resource();
}


