#include "pch.h"
#include "hlt_Box.h"

hlt_Box::Box3D_AABB::Box3D_AABB(DirectX::FXMVECTOR minPos, DirectX::FXMVECTOR maxPos)
{
	XMStoreFloat3(&m_Min, minPos);
	XMStoreFloat3(&m_Max, maxPos);
}

void hlt_Box::Box3D_AABB::Zero()
{
	m_Min = { 0.f, 0.f, 0.f };
	m_Max = { 0.f, 0.f, 0.f };
}

XMFLOAT3 hlt_Box::Box3D_AABB::Size()
{
	XMFLOAT3 boxSize;
	XMVECTOR boxMax = XMLoadFloat3(&m_Max);
	XMVECTOR boxMin = XMLoadFloat3(&m_Min);
	XMStoreFloat3(&boxSize, XMVectorSubtract(boxMax, boxMin));
	return boxSize;
}

bool hlt_Box::Box3D_AABB::Contains(DirectX::FXMVECTOR p)
{
	DirectX::XMFLOAT3 pPos;
	XMStoreFloat3(&pPos, p);
	return pPos.x >= m_Min.x && pPos.x <= m_Max.x && pPos.y >= m_Min.y && pPos.y <= m_Max.y && pPos.z >= m_Min.z && pPos.z <= m_Max.z;
}

bool hlt_Box::Box3D_AABB::Contains(hlt_Box::Box3D_AABB box)
{
	if (Contains(box.m_Min))
		return true;
	if (Contains(box.m_Max))
		return true;

	XMFLOAT3 boxSize = box.Size();

	if (Contains(DirectX::XMFLOAT3(box.m_Max.x - boxSize.x, box.m_Max.y, box.m_Max.z)))
		return true;
	if (Contains(DirectX::XMFLOAT3(box.m_Max.x, box.m_Max.y - boxSize.y, box.m_Max.z)))
		return true;
	if (Contains(DirectX::XMFLOAT3(box.m_Max.x, box.m_Max.y, box.m_Max.z - boxSize.x)))
		return true;
	if (Contains(DirectX::XMFLOAT3(box.m_Min.x + boxSize.x, box.m_Min.y, box.m_Min.z)))
		return true;
	if (Contains(DirectX::XMFLOAT3(box.m_Min.x, box.m_Min.y + boxSize.y, box.m_Min.z)))
		return true;
	if (Contains(DirectX::XMFLOAT3(box.m_Min.x, box.m_Min.y, box.m_Min.z + boxSize.x)))
		return true;

	return false;
}

hlt_Box::Box3D_AABB hlt_Box::Box3D_AABB::operator+(hlt_Transform3D boxPos)
{
	Box3D_AABB newBox;
	DirectX::XMFLOAT3 boxSize = Size();
	DirectX::XMFLOAT3 halfBoxSize{ boxSize.x * 0.5f, boxSize.y * 0.5f, boxSize.z * 0.5f };
	newBox.m_Min = DirectX::XMFLOAT3(m_Min.x - halfBoxSize.x + boxPos.pos.x, m_Min.y - halfBoxSize.y + boxPos.pos.y, m_Min.z - halfBoxSize.z + boxPos.pos.z);
	newBox.m_Max = DirectX::XMFLOAT3(m_Max.x + halfBoxSize.x + boxPos.pos.x, m_Max.y + halfBoxSize.y + boxPos.pos.y, m_Max.z + halfBoxSize.z + boxPos.pos.z);

	return newBox;
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
