#pragma once
#include <d3d11on12.h>
#include <d2d1_3.h>
#include <windows.foundation.h>
#include <dwrite.h>

class hlt_SplashScreen : public hlt_D2DResource
{
public:
	hlt_SplashScreen() = default;

	void Initialize(ID3D11On12Device* d11On12,
		ID2D1DeviceContext2* d2dCtx,
		ID3D11DeviceContext* d11Ctx,
		int swapChainBC,
		ComPtr<ID3D12Resource>* swapChainBuffer,
		ComPtr<ID3D11Resource>* wrappedBackBuffers) override;
	
	void Draw(float WindowWidthMiddle, float WindowHightMiddle);
	D2D1_RECT_F DrawRect(float left, float top, float right, float bottom, D2D1::ColorF colorBrush, D2D1::ColorF colorRectEdge);

public:
	float m_Opacity = 1.f;
	ComPtr<IDWriteTextFormat> m_textFormatBody;
	ComPtr<ID2D1SolidColorBrush> m_textBrush;
};
