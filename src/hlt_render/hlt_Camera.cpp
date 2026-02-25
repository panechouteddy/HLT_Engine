#include "pch.h"
//#define constexpr float PI = 3.14f;
//
hlt_Camera::hlt_Camera()
{
	m_height = 5.0f;
	m_width = 2.0f;
	m_z = 0.0f;
}
void hlt_Camera::Update()
{

    float Theta = 1.5f * XM_PI;
    float Phi = XM_PIDIV4;
    float Radius = 5.0f;

    // Convert Spherical to Cartesian coordinates.
    float x = Radius * sinf(Phi) * cosf(Theta);
    float z = Radius * sinf(Phi) * sinf(Theta);
    float y = Radius * cosf(Phi) + 2;

    XMVECTOR pos = XMVectorSet(x, y, z, 2.0f);
    XMVECTOR target = XMVectorZero();
    XMVECTOR up = XMVectorSet(0.0f, 0.1f, 0.0f, 0.0f); //cam

    XMMATRIX view = XMMatrixLookAtLH(pos, target, up);

    XMStoreFloat4x4(&m_View, view);
    float fovY = DirectX::XM_PIDIV4; float nearPlane = 0.1f; float farPlane = 100.f;

    DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(fovY, D3DApp::GetApp()->GetWindowRatio(), nearPlane, farPlane);
    DirectX::XMStoreFloat4x4(&m_Proj, proj);
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

