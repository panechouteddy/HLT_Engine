#pragma once

class hlt_UI : public hlt_D2DResource
{
public:
	hlt_UI() = default;
	
	void Initialize(ComPtr<ID3D12Device> device, ComPtr<ID3D12CommandQueue> commandQueue, int swapChainBC,
		ComPtr<ID3D12Resource>* swapChainBuffer, ComPtr<ID3D11Resource>* wrappedBackBuffers) override;
	void Draw(float WindowWidthMiddle, std::wstring stats);

public:
	bool m_Initialize = false;

	ComPtr<IDWriteTextFormat> m_textFormatBody;
	ComPtr<ID2D1SolidColorBrush> m_textBrush;
};