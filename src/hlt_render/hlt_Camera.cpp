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
    if (DEBUG) DebugInput();

    m_Transform.UpdateWorld();
    XMMATRIX view = XMLoadFloat4x4(&m_Transform.world);

    // View is inverse of world
    XMVECTOR determinant;
    view = XMMatrixInverse(&determinant, view);

    XMStoreFloat4x4(&m_View, view);

    float fovY = DirectX::XM_PIDIV4; float nearPlane = 0.01f; float farPlane = 100.f;

    DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(fovY, D3DApp::GetApp()->GetWindowRatio(), nearPlane, farPlane);
    DirectX::XMStoreFloat4x4(&m_Proj, proj);
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
        //m_Transform.Move(10.f * hlt_Time::GetInstance().GetDeltaTime(), m_Transform.up);
    if (keyboardInput.IsKey(VK_LCONTROL))
        m_Transform.pos.y -= 10.f * hlt_Time::GetInstance().GetDeltaTime();
        //m_Transform.Move(-10.f * hlt_Time::GetInstance().GetDeltaTime(), m_Transform.up);

    // ROTATION TO DO
    hlt_Input::MouseInput& mouse = hlt_Input::MouseInput::GetInstance();

    // On récupère le delta de mouvement de la souris
    XMINT2 delta = mouse.GetDeltaPos();

    if (delta.x != 0 || delta.y != 0)
    {
        // x de la souris = Yaw (rotation autour de l'axe Up)
        // y de la souris = Pitch (rotation autour de l'axe Right)
        float yaw = static_cast<float>(delta.x) * CAMERA_SENSIBILITY;
        float pitch = static_cast<float>(delta.y) * CAMERA_SENSIBILITY;

        // On utilise ta fonction existante dans hlt_Transform3D
        // AddYPR(yaw, pitch, roll)
        m_Transform.AddYPR(yaw, pitch, 0.f);
    }

    if (keyboardInput.IsKey(VK_A))
        m_Transform.ResetRotation();
}