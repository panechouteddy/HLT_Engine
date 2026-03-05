#include "pch.h"

void hlt_UI::Draw(float WindowWidthMiddle, std::wstring text)
{
	D2D1_RECT_F layoutRect = D2D1::RectF(WindowWidthMiddle - 50, 50.0f, WindowWidthMiddle + 50, 300.0f);
	m_d2dContext->DrawText(
		text.c_str(),
		(UINT32)text.length(),
		m_textFormatBody.Get(),
		layoutRect,
		m_textBrush.Get()
	);
}
