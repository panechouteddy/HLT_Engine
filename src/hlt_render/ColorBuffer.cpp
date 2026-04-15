#include "pch.h"
#include "ColorBuffer.h"

ColorBuffer::ColorBuffer(Microsoft::WRL::ComPtr<ID3D12Device> Device)
{
	m_pColorConstants = new UploadBuffer<ColorConstants>(Device.Get(), 1, true);
}

ColorBuffer::~ColorBuffer()
{
	delete m_pColorConstants;
}
ID3D12Resource* ColorBuffer::GetResource()
{
	return m_pColorConstants->Resource();
}