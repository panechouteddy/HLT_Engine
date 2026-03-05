#pragma once


class SwitchDirectionObject
{
public:
	SwitchDirectionObject(hlt_ECS* ecs);
	~SwitchDirectionObject() = default;

	void Update();

	void SetNewDirection(XMFLOAT3 newDir) { m_NewDirection = newDir; }

private:
	hlt_ECS* m_pECS = nullptr;
	int m_ID = -1;

	XMFLOAT3 m_NewDirection = { 0, 0, 0 };

	hlt_Component::BoxCollider3D* m_pCollider = nullptr;

public:
	hlt_Component::Transform3D* m_pTransform = nullptr;
};

