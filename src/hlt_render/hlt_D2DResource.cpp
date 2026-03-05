#include "pch.h"

void hlt_D2DResource::StartDraw(int m_CurrBackBuffer, ComPtr<ID3D11Resource>* wrappedBackBuffers)
{
	m_d2dContext->SetTarget(nullptr);

	ComPtr<IDXGISurface> surface;
	ThrowIfFailed(wrappedBackBuffers[m_CurrBackBuffer].As(&surface));

	D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	ComPtr<ID2D1Bitmap1> d2dTargetBitmap;
	ThrowIfFailed(m_d2dContext->CreateBitmapFromDxgiSurface(surface.Get(), &bitmapProperties, &d2dTargetBitmap));

	m_d2dContext->SetTarget(d2dTargetBitmap.Get());

	m_d3d11On12Device->AcquireWrappedResources(wrappedBackBuffers[m_CurrBackBuffer].GetAddressOf(), 1);
	m_d2dContext->BeginDraw();
}

void hlt_D2DResource::EndDraw(int m_CurrBackBuffer, ComPtr<ID3D11Resource>* wrappedBackBuffers)
{
	m_d2dContext->EndDraw();

	m_d2dContext->SetTarget(nullptr);
	m_d3d11On12Device->ReleaseWrappedResources(wrappedBackBuffers[m_CurrBackBuffer].GetAddressOf(), 1);

	m_d3d11DeviceContext->Flush();
}


void hlt_D2DResource::ReleaseResources(int swapChainBC, ComPtr<ID3D11Resource>* wrappedBackBuffers)
{
	for (int i = 0; i < swapChainBC; i++)
	{
		wrappedBackBuffers[i].Reset();
	}
	if (m_d2dContext)
	{
		m_d2dContext->SetTarget(nullptr);
	}
}