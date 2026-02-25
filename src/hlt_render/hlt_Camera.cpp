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
    if (DEBUG)
        DebugInput();

    XMMATRIX view = XMMatrixIdentity();

    view *= XMMatrixTranslation(m_Transform.pos.x, m_Transform.pos.y, m_Transform.pos.z);

    XMVECTOR rotation = XMLoadFloat4(&m_Transform.quaternion);
    view *= XMMatrixRotationQuaternion(rotation);

    //XMVECTOR pos = XMVectorSet(1.5f, XM_PIDIV4, 5.f, 1.f);
    //XMVECTOR target = XMVectorZero();
    //XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); //cam

    //XMMATRIX view = XMMatrixLookAtLH(pos, target, up);

    XMStoreFloat4x4(&m_View, view);

    float fovY = DirectX::XM_PIDIV4; float nearPlane = 0.01f; float farPlane = 100.f;

    DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(fovY, D3DApp::GetApp()->GetWindowRatio(), nearPlane, farPlane);
    DirectX::XMStoreFloat4x4(&m_Proj, proj);
}

void hlt_Camera::DebugInput()
{
    hlt_Input::KeyboardInput& keyboardInput = HLT_KEYBOARD;

    if (keyboardInput.IsKey(VK_Z) || keyboardInput.IsKey(VK_W))
        m_Transform.Move(-10.f * hlt_Time::GetInstance().GetDeltaTime());
    if (keyboardInput.IsKey(VK_S))
        m_Transform.Move(10.f * hlt_Time::GetInstance().GetDeltaTime());
    if (keyboardInput.IsKey(VK_Q) || keyboardInput.IsKey(VK_A))
        m_Transform.Move(10.f * hlt_Time::GetInstance().GetDeltaTime(), m_Transform.right);
    if (keyboardInput.IsKey(VK_D))
        m_Transform.Move(-10.f * hlt_Time::GetInstance().GetDeltaTime(), m_Transform.right);
    if (keyboardInput.IsKey(VK_SPACE))
        m_Transform.Move(-10.f * hlt_Time::GetInstance().GetDeltaTime(), m_Transform.up);
    if (keyboardInput.IsKey(VK_LCONTROL))
        m_Transform.Move(10.f * hlt_Time::GetInstance().GetDeltaTime(), m_Transform.up);

    // ROTATION TO DO
}

//{
//
//    // Get Datas
//    XMMATRIX viewMatrix = XMLoadFloat4x4(&cameraViewMat);
//
//    //Move Up and bottom
//    if (GetAsyncKeyState('Z')) {
//        viewMatrix *= XMMatrixTranslation(0.0f, -0.0001f, 0.0f);
//    }
//    else if (GetAsyncKeyState('S')) {
//        viewMatrix *= XMMatrixTranslation(0.0f, 0.0001f, 0.0f);
//    }
//
//    //Move Right and Left
//    if (GetAsyncKeyState('Q')) {
//        viewMatrix *= XMMatrixTranslation(0.0001f, 0.0f, 0.0f);
//    }
//    else if (GetAsyncKeyState('D')) {
//        viewMatrix *= XMMatrixTranslation(-0.0001f, 0.0f, 0.0f);
//    }
//
//    //Move forward/Backward
//    if (GetAsyncKeyState('P')) {
//        viewMatrix *= XMMatrixTranslation(0.0f, 0.0f, -0.0001f);
//    }
//    else if (GetAsyncKeyState('M')) {
//        viewMatrix *= XMMatrixTranslation(0.0f, 0.0f, 0.0001f);
//    }
//
//    //Rotate Y axis
//    if (GetAsyncKeyState('A')) {
//        viewMatrix *= XMMatrixRotationY(0.0001f);
//    }
//    else if (GetAsyncKeyState('E')) {
//        viewMatrix *= XMMatrixRotationY(-0.0001f);
//    }
//
//    //Rotate X axis
//    if (GetAsyncKeyState('R')) {
//        viewMatrix *= XMMatrixRotationX(0.0001f);
//    }
//    else if (GetAsyncKeyState('T')) {
//        viewMatrix *= XMMatrixRotationX(-0.0001f);
//    }
//
//    //Rotate Z axis
//    if (GetAsyncKeyState('F')) {
//        viewMatrix *= XMMatrixRotationZ(0.0001f);
//    }
//    else if (GetAsyncKeyState('G')) {
//        viewMatrix *= XMMatrixRotationZ(-0.0001f);
//    }
//
//    //Update View Matrix of Camera
//    XMStoreFloat4x4(&cameraViewMat, viewMatrix);
//}

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

