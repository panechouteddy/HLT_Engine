#pragma once
#include <d3d11on12.h>
#include <d2d1_3.h>
#include <windows.foundation.h>
#include <dwrite.h>

class hlt_SplashScreen : public hlt_D2DResource
{
public:
	hlt_SplashScreen() = default;
	
	void Draw(float WindowWidthMiddle, float WindowHightMiddle);
	D2D1_RECT_F DrawRect(float left, float top, float right, float bottom, D2D1::ColorF colorBrush, D2D1::ColorF colorRectEdge);
};
