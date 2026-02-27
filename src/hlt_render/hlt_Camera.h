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
};

