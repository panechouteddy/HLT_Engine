#include "pch.h"

void hlt_D2DResource::Initialize(ID3D11On12Device* d11On12, ID2D1DeviceContext2* d2dCtx, ID3D11DeviceContext* d11Ctx, int swapChainBC, ComPtr<ID3D12Resource>* swapChainBuffer, 
	ComPtr<ID3D11Resource>* wrappedBackBuffers, WCHAR* fontFamilyName, float fontSize, WCHAR* localName, const D2D1_COLOR_F& fontColor)
{
	m_d3d11On12Device = d11On12;
	m_d2dContext = d2dCtx;
	m_d3d11DeviceContext = d11Ctx;

	D3D11_RESOURCE_FLAGS d3d11Flags = { D3D11_BIND_RENDER_TARGET };
	for (UINT i = 0; i < (UINT)swapChainBC; i++)
	{
		if (wrappedBackBuffers[i] == nullptr) {
			ThrowIfFailed(m_d3d11On12Device->CreateWrappedResource(
				swapChainBuffer[i].Get(),
				&d3d11Flags,
				D3D12_RESOURCE_STATE_PRESENT,
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				IID_PPV_ARGS(&wrappedBackBuffers[i])
			));
		}
	}

	ComPtr<IDWriteFactory> writeFactory;
	ThrowIfFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &writeFactory));

	ThrowIfFailed(writeFactory->CreateTextFormat(
		fontFamilyName, nullptr, DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		fontSize, localName, &m_textFormatBody
	));

	ThrowIfFailed(m_d2dContext->CreateSolidColorBrush(fontColor, &m_textBrush));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

void hlt_D2DResource::Draw()
{
	//D2D1_RECT_F layoutRect = D2D1::RectF(WindowWidthMiddle - 50, 50.0f, WindowWidthMiddle + 50, 300.0f);

	/*m_d2dContext->DrawText(
		stats.c_str(),
		(UINT32)stats.length(),
		m_textFormatBody.Get(),
		layoutRect,
		m_textBrush.Get()
	);*/

	//DrawRect(0, 0, WindowWidthMiddle * 2, WindowHightMiddle * 2, D2D1::ColorF(D2D1::ColorF::DarkRed), D2D1::ColorF(D2D1::ColorF::DarkRed));

	//if(m_pRect != nullptr) m_d2dContext->DrawRectangle(m_pRect, m_RectColor.Get(), 2.0f);

	/*D2D1_RECT_F rect = DrawRect(WindowWidthMiddle - 150, WindowHightMiddle - 50, WindowWidthMiddle + 150, WindowHightMiddle + 50, D2D1::ColorF(D2D1::ColorF::Coral), D2D1::ColorF(D2D1::ColorF::White));
	std::wstring label = L"FPS: ";*/

	/*m_textFormatBody->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_textFormatBody->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);*/

	/*m_textBrush->SetOpacity(m_GlobalOpacity);

	m_d2dContext->DrawText(
		m_Text.c_str(),
		(UINT32)m_Text.length(),
		m_textFormatBody.Get(),
		m_TextArea,
		m_textBrush.Get()
	);*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

D2D1_RECT_F hlt_D2DResource::MakeRectangle(float left, float top, float right, float bottom, D2D1::ColorF colorBrush, D2D1::ColorF colorRectEdge, float textOpacity)
{
	D2D1_RECT_F rect = D2D1::RectF(
		left, top, right, bottom
	);
	ComPtr<ID2D1SolidColorBrush> RectangleColor;

	auto color = colorBrush;
	m_d2dContext->CreateSolidColorBrush(color, &RectangleColor);

	RectangleColor->SetOpacity(textOpacity);

	m_d2dContext->FillRectangle(rect, RectangleColor.Get());

	RectangleColor->SetColor(colorRectEdge);

	RectangleColor->SetOpacity(textOpacity);

	m_d2dContext->DrawRectangle(rect, RectangleColor.Get(), 2.0f);

	return rect;
}