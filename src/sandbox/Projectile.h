#pragma once

class Projectile
{
public:
	Projectile();
	~Projectile();

	void Update();
	void Move();

public:
	XMFLOAT3 m_pos;
	XMFLOAT3 m_dir;

	int m_ProjectileID = -1;
	bool m_IsDead;

private:
	hlt_ECS* ecs;

	hlt_Component::ConstantMove* oCMove;
	hlt_Component::BoxCollider3D* oBox;

	bool* oIsColliding = nullptr;
	bool m_IsActive = false;
};
