#pragma once
using namespace DirectX;
#include "MathHelper.h"
#include <DirectXMath.h>

struct ColorConstants
{
	XMFLOAT4 ObjectColor;
};

template <typename T>
class UploadBuffer;

class ColorBuffer
{

	protected:
		UploadBuffer<ColorConstants>* m_pColorConstants;
	public:
		ColorBuffer(Microsoft::WRL::ComPtr<ID3D12Device> Device);
		~ColorBuffer();

		UploadBuffer<ColorConstants>* GetBuffer() { return m_pColorConstants; }
		ID3D12Resource* GetResource();

};

