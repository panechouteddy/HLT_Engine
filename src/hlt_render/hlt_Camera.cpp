#include "pch.h"

#define DEGREE_TO_RADIAN PI / 360.0f
#define PITCH_MAX 89.0f * DEGREE_TO_RADIAN
#define PITCH_MIN -89.0f * DEGREE_TO_RADIAN

hlt_Camera::hlt_Camera()
{
    m_Transform.pos.z = -0;
	m_height = 5.0f;
	m_width = 2.0f;
	m_z = 0.0f;
}

void hlt_Camera::Update()
{
    if (DEBUG) DebugMoveInput();
    
    if (m_IsMouseCamera) MouseRotation();

    m_Transform.UpdateWorld();
    XMMATRIX view = XMLoadFloat4x4(&m_Transform.world);

    XMVECTOR determinant;
    view = XMMatrixInverse(&determinant, view);

    XMStoreFloat4x4(&m_View, view);

    float fovY = DirectX::XM_PIDIV4; float nearPlane = 0.01f; float farPlane = 100.f;

    DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(fovY, D3DApp::GetApp()->GetWindowRatio(), nearPlane, farPlane);
    DirectX::XMStoreFloat4x4(&m_Proj, proj);
}

void hlt_Camera::MouseRotation()
{
    hlt_Input::KeyboardInput& keyboardInput = HLT_KEYBOARD;

    hlt_Input::MouseInput& mouse = hlt_Input::MouseInput::GetInstance();

    XMINT2 delta = mouse.GetDeltaPos();

    if (delta.x != 0 || delta.y != 0)
    {
        float yaw = static_cast<float>(delta.x) * CAMERA_SENSIBILITY;
        float pitch = static_cast<float>(delta.y) * CAMERA_SENSIBILITY;

        float newPitch = m_currentPitch + pitch;

        newPitch = Clamp(newPitch, PITCH_MIN, PITCH_MAX);

        float allowedPitchDelta = newPitch - m_currentPitch;

        m_currentPitch = newPitch;

        m_Transform.AddYPR(yaw, allowedPitchDelta, 0.f, true);

        if(DEBUG) m_Transform.AddYPR(yaw, pitch, 0.f, true);
    }

    if (keyboardInput.IsKey(VK_A))
        m_Transform.ResetRotation();
}

void hlt_Camera::DebugMoveInput()
{
   
}

float hlt_Camera::Clamp(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}
