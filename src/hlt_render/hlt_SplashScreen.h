#pragma once
#include <d3d11on12.h>
#include <d2d1_3.h>
#include <windows.foundation.h>
#include <dwrite.h>

class hlt_SplashScreen : public hlt_D2DResource
{
public:
	hlt_SplashScreen(
		int swapChainBC,
		ComPtr<ID3D12Resource>* swapChainBuffer,
		ComPtr<ID3D11Resource>* wrappedBackBuffers) : hlt_D2DResource(swapChainBC, swapChainBuffer, wrappedBackBuffers) { ; }

	void Initialize(WCHAR* fontFamilyName,
		float fontSize,
		WCHAR* localName,
		D2D1_COLOR_F fontColor) override;

	void SetSmoothTransition(bool isSmooth) { m_SmoothTransition = isSmooth; }

	void Update();

private:
	bool m_SmoothTransition = false;

	XMINT2 m_WindowSize = { 0, 0 };
};

