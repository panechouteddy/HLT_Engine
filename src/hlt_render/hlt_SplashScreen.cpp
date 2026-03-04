#include "pch.h"

void hlt_SplashScreen::Initialize(WCHAR* fontFamilyName, float fontSize, WCHAR* localName, D2D1_COLOR_F fontColor)
{
	hlt_D2DResource::Initialize(fontFamilyName, fontSize, localName, fontColor);

	m_WindowSize = D3DApp::GetApp()->GetSize();
}

void hlt_SplashScreen::Update()
{
	m_TextOpacity -= 0.1f;
}

//void hlt_SplashScreen::Draw(float WindowWidthMiddle, float WindowHightMiddle)
//{
//	DrawRect(0, 0, WindowWidthMiddle * 2, WindowHightMiddle * 2, D2D1::ColorF(D2D1::ColorF::DarkRed), D2D1::ColorF(D2D1::ColorF::DarkRed));
//	
//	D2D1_RECT_F rect = DrawRect(WindowWidthMiddle - 150, WindowHightMiddle - 50, WindowWidthMiddle + 150, WindowHightMiddle + 50, D2D1::ColorF(D2D1::ColorF::Coral), D2D1::ColorF(D2D1::ColorF::White));
//	std::wstring label = L"FPS: ";
//
//	m_textFormatBody->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
//	m_textFormatBody->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
//	
//	m_textBrush->SetOpacity(m_TextOpacity);
//
//	m_d2dContext->DrawText(
//		label.c_str(),
//		(UINT32)label.length(),
//		m_textFormatBody.Get(),
//		rect,
//		m_textBrush.Get()
//	);
//}
//
//D2D1_RECT_F hlt_SplashScreen::DrawRect(float left, float top, float right, float bottom, D2D1::ColorF colorBrush, D2D1::ColorF colorRectEdge)
//{
//	D2D1_RECT_F rect = D2D1::RectF(
//		left, top, right, bottom
//	);
//	ComPtr<ID2D1SolidColorBrush> RectangleColor;
//
//	auto color = colorBrush;
//	m_d2dContext->CreateSolidColorBrush(color, &RectangleColor);
//
//	RectangleColor->SetOpacity(m_TextOpacity);
//
//	m_d2dContext->FillRectangle(rect, RectangleColor.Get());
//
//	RectangleColor->SetColor(colorRectEdge);
//
//	RectangleColor->SetOpacity(m_TextOpacity);
//
//	m_d2dContext->DrawRectangle(rect, RectangleColor.Get(), 2.0f);
//
//	return rect;
//}
