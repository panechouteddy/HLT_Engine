#include "pch.h"
#include "hlt_Box.h"

hlt_Box::Box3D_AABB::Box3D_AABB(DirectX::FXMVECTOR center, DirectX::FXMVECTOR size)
{
	DirectX::XMFLOAT3 centerBox;
	DirectX::XMFLOAT3 sizeBox;
	XMStoreFloat3(&centerBox, center);
	XMStoreFloat3(&sizeBox, size);

	m_Box = DirectX::BoundingBox(centerBox, sizeBox);
}

void hlt_Box::Box3D_AABB::Zero()
{
	m_Box = DirectX::BoundingBox();
}

XMFLOAT3 hlt_Box::Box3D_AABB::Size()
{
	DirectX::XMVECTOR boxSize = XMLoadFloat3(&m_Box.Extents);
	DirectX::XMFLOAT3 multiplier = DirectX::XMFLOAT3(2, 2, 2);
	DirectX::XMVECTOR times2 = XMLoadFloat3(&multiplier);
	XMVectorMultiply(boxSize, times2);

	DirectX::XMFLOAT3 result;
	XMStoreFloat3(&result, boxSize);
	return result;
}

bool hlt_Box::Box3D_AABB::Contains(DirectX::FXMVECTOR p)
{
	DirectX::XMFLOAT3 pPos;
	XMStoreFloat3(&pPos, p);
	DirectX::BoundingBox point = DirectX::BoundingBox(pPos, DirectX::XMFLOAT3());
	return m_Box.Contains(point);
}

bool hlt_Box::Box3D_AABB::Contains(hlt_Box::Box3D_AABB box)
{
	return m_Box.Contains(box.m_Box);
}

hlt_Box::Box3D_AABB hlt_Box::Box3D_AABB::operator+(hlt_Transform3D boxPos)
{
	m_Box.Center = boxPos.pos;
	return *this;
}

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

hlt_Box::Box3D_OBB::Box3D_OBB(DirectX::FXMVECTOR centerPos, DirectX::FXMVECTOR size, DirectX::FXMVECTOR rotation)
{
	DirectX::XMFLOAT3 boxPos;
	DirectX::XMFLOAT3 boxSize;
	DirectX::XMFLOAT4 boxRotation;

	XMStoreFloat3(&boxPos, centerPos);
	XMStoreFloat3(&boxSize, size);
	XMStoreFloat4(&boxRotation, rotation);

	m_Box = DirectX::BoundingOrientedBox(
		boxPos,
		boxSize,
		boxRotation
	);
}

void hlt_Box::Box3D_OBB::Zero()
{
	m_Box = DirectX::BoundingOrientedBox();
}

bool hlt_Box::Box3D_OBB::Contains(Box3D_OBB obb)
{
	return m_Box.Contains(obb.m_Box);
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
