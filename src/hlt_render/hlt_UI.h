#pragma once

class hlt_UI : public hlt_D2DResource
{
public:
	hlt_UI() = default;
	
	void Initialize(ComPtr<ID3D12Device> device, ComPtr<ID3D12CommandQueue> commandQueue, int swapChainBC,
		ComPtr<ID3D12Resource>* swapChainBuffer, ComPtr<ID3D11Resource>* wrappedBackBuffers);
	void Draw(const GameTimer& gt, int m_CurrBackBuffer, ComPtr<ID3D11Resource>* wrappedBackBuffers, float WindowWidthMiddle);

public:

	ComPtr<ID3D11Device> m_d3d11Device;
	ComPtr<ID3D11DeviceContext> m_d3d11DeviceContext;
	ComPtr<ID3D11On12Device> m_d3d11On12Device;
	ComPtr<ID2D1DeviceContext2> m_d2dContext;

	ComPtr<IDWriteTextFormat> m_textFormatBody;
	ComPtr<ID2D1SolidColorBrush> m_textBrush;
};