#pragma once

class hlt_UI : public hlt_D2DResource
{
public:
	hlt_UI() = default;

	void Draw(float WindowWidthMiddle, std::wstring stats);
};