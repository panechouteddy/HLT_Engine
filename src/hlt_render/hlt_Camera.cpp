#include "pch.h"

hlt_Camera::hlt_Camera()
{
    m_Transform.pos.z = -5;
	m_height = 5.0f;
	m_width = 2.0f;
	m_z = 0.0f;
}

void hlt_Camera::Update()
{
    if (DEBUG) DebugInput();
    
    if (m_IsMouseCamera) MouseCamera();

    m_Transform.UpdateWorld();
    XMMATRIX view = XMLoadFloat4x4(&m_Transform.world);

    XMVECTOR determinant;
    view = XMMatrixInverse(&determinant, view);

    XMStoreFloat4x4(&m_View, view);

    float fovY = DirectX::XM_PIDIV4; float nearPlane = 0.01f; float farPlane = 100.f;

    DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(fovY, D3DApp::GetApp()->GetWindowRatio(), nearPlane, farPlane);
    DirectX::XMStoreFloat4x4(&m_Proj, proj);
}

void hlt_Camera::MouseCamera()
{
    hlt_Input::KeyboardInput& keyboardInput = HLT_KEYBOARD;

    // if (keyboardInput.IsKey(VK_Z) || keyboardInput.IsKey(VK_W))
    //     m_Transform.Move(10.f * hlt_Time::GetInstance().GetDeltaTime());
    // if (keyboardInput.IsKey(VK_S))
    //     m_Transform.Move(-10.f * hlt_Time::GetInstance().GetDeltaTime());
    // if (keyboardInput.IsKey(VK_Q) || keyboardInput.IsKey(VK_A))
    //     m_Transform.Move(-10.f * hlt_Time::GetInstance().GetDeltaTime(), m_Transform.right);
    // if (keyboardInput.IsKey(VK_D))
    //     m_Transform.Move(10.f * hlt_Time::GetInstance().GetDeltaTime(), m_Transform.right);
    // if (keyboardInput.IsKey(VK_SPACE))
    //     m_Transform.pos.y += 10.f * hlt_Time::GetInstance().GetDeltaTime();
    // if (keyboardInput.IsKey(VK_LCONTROL))
    //     m_Transform.pos.y -= 10.f * hlt_Time::GetInstance().GetDeltaTime();

    hlt_Input::MouseInput& mouse = hlt_Input::MouseInput::GetInstance();

    XMINT2 delta = mouse.GetDeltaPos();

    if (delta.x != 0 || delta.y != 0)
    {
        float yaw = static_cast<float>(delta.x) * CAMERA_SENSIBILITY;
        float pitch = static_cast<float>(delta.y) * CAMERA_SENSIBILITY;

        m_Transform.AddYPR(yaw, pitch, 0.f);
    }

    if (keyboardInput.IsKey(VK_A))
        m_Transform.ResetRotation();
}

void hlt_Camera::DebugInput()
{
    hlt_Input::KeyboardInput& keyboardInput = HLT_KEYBOARD;

    if (keyboardInput.IsKey(VK_Z) || keyboardInput.IsKey(VK_W))
        m_Transform.Move(10.f * hlt_Time::GetInstance().GetDeltaTime());
    if (keyboardInput.IsKey(VK_S))
        m_Transform.Move(-10.f * hlt_Time::GetInstance().GetDeltaTime());
    if (keyboardInput.IsKey(VK_Q) || keyboardInput.IsKey(VK_A))
        m_Transform.Move(-10.f * hlt_Time::GetInstance().GetDeltaTime(), m_Transform.right);
    if (keyboardInput.IsKey(VK_D))
        m_Transform.Move(10.f * hlt_Time::GetInstance().GetDeltaTime(), m_Transform.right);
    if (keyboardInput.IsKey(VK_SPACE))
        m_Transform.pos.y += 10.f * hlt_Time::GetInstance().GetDeltaTime();
    if (keyboardInput.IsKey(VK_LSHIFT))
        m_Transform.pos.y -= 10.f * hlt_Time::GetInstance().GetDeltaTime();

    if (keyboardInput.IsKey(VK_A))
        m_Transform.ResetRotation();
}