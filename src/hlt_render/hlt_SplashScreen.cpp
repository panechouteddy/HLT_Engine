#include "pch.h"
//
//#include "hlt_SplashScreen.h"
//
//void hlt_SplashScreen::Initialize(ID3D11On12Device* d11On12,
//	ID2D1DeviceContext2* d2dCtx,
//	ID3D11DeviceContext* d11Ctx,
//	int swapChainBC,
//	ComPtr<ID3D12Resource>* swapChainBuffer,
//	ComPtr<ID3D11Resource>* wrappedBackBuffers)
//{
//	m_d3d11On12Device = d11On12;
//	m_d2dContext = d2dCtx;
//	m_d3d11DeviceContext = d11Ctx;
//
//	D3D11_RESOURCE_FLAGS d3d11Flags = { D3D11_BIND_RENDER_TARGET };
//	for (UINT i = 0; i < (UINT)swapChainBC; i++)
//	{
//		if (wrappedBackBuffers[i] == nullptr) {
//			ThrowIfFailed(m_d3d11On12Device->CreateWrappedResource(
//				swapChainBuffer[i].Get(),
//				&d3d11Flags,
//				D3D12_RESOURCE_STATE_PRESENT,
//				D3D12_RESOURCE_STATE_RENDER_TARGET,
//				IID_PPV_ARGS(&wrappedBackBuffers[i])
//			));
//		}
//	}
//
//	ComPtr<IDWriteFactory> writeFactory;
//	ThrowIfFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &writeFactory));
//
//	ThrowIfFailed(writeFactory->CreateTextFormat(
//		L"Consolas", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
//		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
//		20.0f, L"en-us", &m_textFormatBody
//	));
//
//	ThrowIfFailed(m_d2dContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray), &m_textBrush));
//}
//
//void hlt_SplashScreen::Draw()
//{
//	if (m_Start)
//		return;
//
//	D2D1_RECT_F rect = D2D1::RectF(
//		50.0f, 50.0f, 300.0f, 300.0f
//	);
//	std::wstring stats = L"FPS: ";
//	DrawButton(rect, stats, true);
//
//}
//
//void hlt_SplashScreen::DrawButton(D2D1_RECT_F rect, std::wstring label, bool isHovered)
//{
//	auto color = isHovered ? D2D1::ColorF(D2D1::ColorF::WhiteSmoke) : D2D1::ColorF(D2D1::ColorF::White);
//
//	ComPtr<ID2D1SolidColorBrush> buttonBrush;
//	m_d2dContext->CreateSolidColorBrush(color, &buttonBrush);
//
//	m_d2dContext->FillRectangle(rect, buttonBrush.Get());
//
//	buttonBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
//	m_d2dContext->DrawRectangle(rect, buttonBrush.Get(), 2.0f);
//
//	m_textFormatBody->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
//	m_textFormatBody->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
//
//	m_d2dContext->DrawText(
//		label.c_str(),
//		(UINT32)label.length(),
//		m_textFormatBody.Get(),
//		rect,
//		m_textBrush.Get()
//	);
//
//}