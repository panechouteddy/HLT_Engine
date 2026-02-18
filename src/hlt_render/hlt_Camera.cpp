#include "pch.h"
//#define constexpr float PI = 3.14f;
//
hlt_Camera::hlt_Camera()
{
	m_height = 5.0f;
	m_width = 2.0f;
	m_z = 0.0f;
}
//
//void hlt_Camera::Update(float aspectRatio)
//{
//	XMVECTOR pos = XMVectorSet(m_width, m_height, m_z, 1.0f);
//	XMVECTOR target = XMVectorZero();
//	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
//
//	XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
//	XMStoreFloat4x4(&m_View, view);
//
//	XMMATRIX proj = XMMatrixPerspectiveFovLH(0.25f * PI , aspectRatio, 1.0f, 1000.0f);
//	XMStoreFloat4x4(&m_Proj, proj);
//
//	XMMATRIX viewProj = proj * view;
//	XMStoreFloat4x4(&m_ViewProj,viewProj);
//}

