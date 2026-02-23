#include "pch.h"

void hlt_UI::Initialize(ComPtr<ID3D12Device> device, ComPtr<ID3D12CommandQueue> commandQueue, int swapChainBC,
	ComPtr<ID3D12Resource>* swapChainBuffer, ComPtr<ID3D11Resource>* wrappedBackBuffers)
{
	UINT d3d11DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	ThrowIfFailed(D3D11On12CreateDevice(
		device.Get(),
		d3d11DeviceFlags,
		nullptr, 0,
		reinterpret_cast<IUnknown**>(commandQueue.GetAddressOf()),
		1,
		0,
		&m_d3d11Device,
		&m_d3d11DeviceContext,
		nullptr
	));

	ThrowIfFailed(m_d3d11Device.As(&m_d3d11On12Device));

	D3D11_RESOURCE_FLAGS d3d11Flags = { D3D11_BIND_RENDER_TARGET };

	for (UINT i = 0; i < swapChainBC; i++)
	{
		ThrowIfFailed(m_d3d11On12Device->CreateWrappedResource(
			swapChainBuffer[i].Get(),
			&d3d11Flags,
			D3D12_RESOURCE_STATE_PRESENT,     // État d'entrée
			D3D12_RESOURCE_STATE_RENDER_TARGET, // État de sortie après usage D2D
			IID_PPV_ARGS(&wrappedBackBuffers[i])
		));
	}


	D2D1_FACTORY_OPTIONS options = {};

	ComPtr<ID2D1Factory3> d2dFactory;
	ThrowIfFailed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3), &options, &d2dFactory));

	ComPtr<IDXGIDevice> dxgiDevice;
	ThrowIfFailed(m_d3d11On12Device.As(&dxgiDevice));

	ComPtr<ID2D1Device2> d2dDevice;
	ThrowIfFailed(d2dFactory->CreateDevice(dxgiDevice.Get(), &d2dDevice));
	ThrowIfFailed(d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_d2dContext));

	// 5. Création des ressources de texte (DirectWrite)
	ComPtr<IDWriteFactory> writeFactory;
	ThrowIfFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &writeFactory));

	ThrowIfFailed(writeFactory->CreateTextFormat(
		L"Consolas", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		20.0f, L"en-us", &m_textFormatBody
	));

	ThrowIfFailed(m_d2dContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_textBrush));

}

void hlt_UI::Draw(const GameTimer& gt, int m_CurrBackBuffer, ComPtr<ID3D11Resource>* wrappedBackBuffers, float WindowWidthMiddle, std::wstring stats)
{
	ComPtr<IDXGISurface> surface;
	ThrowIfFailed(wrappedBackBuffers[m_CurrBackBuffer].As(&surface));

	D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	ComPtr<ID2D1Bitmap1> d2dTargetBitmap;
	ThrowIfFailed(m_d2dContext->CreateBitmapFromDxgiSurface(surface.Get(), &bitmapProperties, &d2dTargetBitmap));

	m_d2dContext->SetTarget(d2dTargetBitmap.Get());

	// On acquiert la ressource (Le pont la passe de PRESENT à RENDER_TARGET en interne)
	m_d3d11On12Device->AcquireWrappedResources(wrappedBackBuffers[m_CurrBackBuffer].GetAddressOf(), 1);

	m_d2dContext->BeginDraw();

	// Dessin du texte
	m_d2dContext->DrawText(stats.c_str(), (UINT32)stats.length(), m_textFormatBody.Get(),
		D2D1::RectF(WindowWidthMiddle, 0, WindowWidthMiddle*2, 0), m_textBrush.Get());

	m_d2dContext->EndDraw();

	// On libère (Le pont repasse la ressource de RENDER_TARGET à PRESENT)
	m_d3d11On12Device->ReleaseWrappedResources(wrappedBackBuffers[m_CurrBackBuffer].GetAddressOf(), 1);

	m_d2dContext->SetTarget(nullptr);
	m_d3d11DeviceContext->Flush(); // Important pour envoyer les commandes D2D au GPU
}
