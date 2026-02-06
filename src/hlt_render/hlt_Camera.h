#pragma once
class hlt_Camera
{
public:
	float m_height;
	float m_width;
	XMFLOAT4X4 m_View;
	XMFLOAT4X4 m_Proj;
	XMFLOAT4X4 m_ViewProj;

	hlt_Camera();

	void Update();
};

