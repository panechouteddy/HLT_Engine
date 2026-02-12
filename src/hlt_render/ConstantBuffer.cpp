#include "pch.h"
#include "ConstantBuffer.h"


ConstantBuffer::ConstantBuffer(Microsoft::WRL::ComPtr<ID3D12Device>  Device)
{
	m_bufferConstats = new UploadBuffer<CBfView>(Device.Get(), 1, true);
}

ConstantBuffer::~ConstantBuffer()
{
	delete m_bufferConstats;
}

UploadBuffer<CBfView>* ConstantBuffer::GetBuffer()
{
	return m_bufferConstats;
}

XMFLOAT4X4 ConstantBuffer::GetWorldMatrix()
{
	return m_world;
}

ID3D12Resource* ConstantBuffer::GetResource()
{
	return m_bufferConstats->Resource();
}

