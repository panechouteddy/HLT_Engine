#include "pch.h"
#include "hlt_Box.h"

hlt_Box::Box3D::Box3D(DirectX::XMFLOAT3 min, DirectX::XMFLOAT3 max)
{
	m_Min = min;
	m_Max = max;
}

void hlt_Box::Box3D::Zero()
{
	m_Min = { 0.f, 0.f, 0.f };
	m_Max = { 0.f, 0.f, 0.f };
}

bool hlt_Box::Box3D::Contains(DirectX::XMFLOAT3 p)
{
	return p.x >= m_Min.x && p.x <= m_Max.x && p.y >= m_Min.y && p.y <= m_Max.y && p.z >= m_Min.z && p.z <= m_Max.z;
}

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

hlt_Box::Box2D::Box2D(DirectX::XMFLOAT2 min, DirectX::XMFLOAT2 max)
{
	m_Min = min;
	m_Max = max;
}

bool hlt_Box::Box2D::Contains(DirectX::XMFLOAT2 p)
{
	return p.x >= m_Min.x && p.x <= m_Max.x && p.y >= m_Min.y && p.y <= m_Max.y;
}

void hlt_Box::Box2D::Zero()
{
	m_Min = { 0.f, 0.f };
	m_Max = { 0.f, 0.f };
}
