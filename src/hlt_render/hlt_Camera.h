#pragma once
#define CAMERA_SENSIBILITY 0.005f

class hlt_Camera
{
public:
	hlt_Transform3D m_Transform;
	hlt_Transform3D* m_pTargetPos = nullptr;

	float m_height;
	float m_width;
	float m_z;
	float m_currentPitch = 0.f;

	bool m_IsMouseCamera = false;

	XMFLOAT4X4 m_View;
	XMFLOAT4X4 m_Proj;
	XMFLOAT4X4 m_ViewProj;

public:
	hlt_Camera();

	void Update();
	void MouseCamera();

private:
	void DebugInput();

	static float Clamp(float value, float min = 0.0f, float max = 1.0f);
};

