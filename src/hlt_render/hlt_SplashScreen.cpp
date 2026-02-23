#include "pch.h"

void hlt_SplashScreen::Initialize(ComPtr<ID3D12Device> device, ComPtr<ID3D12CommandQueue> commandQueue, int swapChainBC, ComPtr<ID3D12Resource>* swapChainBuffer, ComPtr<ID3D11Resource>* wrappedBackBuffers)
{
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

	ThrowIfFailed(m_d2dContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray), &m_textBrush));
}

void hlt_SplashScreen::Draw(const GameTimer& gt, int m_CurrBackBuffer, ComPtr<ID3D11Resource>* wrappedBackBuffers)
{
	if (m_Start)
		return;

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

	D2D1_RECT_F rect = D2D1::RectF(
		500,
		2 - 500.0f,
		2 + 500.0f,
		2 + 500.0f
	);
	std::wstring stats = L"FPS: ";
	DrawButton(rect, stats, false);
}

void hlt_SplashScreen::DrawButton(D2D1_RECT_F rect, std::wstring label, bool isHovered)
{
	auto color = isHovered ? D2D1::ColorF(0.4f, 0.4f, 0.4f) : D2D1::ColorF(0.2f, 0.2f, 0.2f);

	ComPtr<ID2D1SolidColorBrush> buttonBrush;
	m_d2dContext->CreateSolidColorBrush(color, &buttonBrush);

	m_d2dContext->FillRectangle(rect, buttonBrush.Get());

	buttonBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	m_d2dContext->DrawRectangle(rect, buttonBrush.Get(), 2.0f);

	m_d2dContext->DrawText(
		label.c_str(),
		(UINT32)label.length(),
		m_textFormatBody.Get(),
		rect,
		m_textBrush.Get()
	);

}
