#pragma once

class Projectile
{
public:
	Projectile();
	~Projectile();

	void Update();

public:
	XMFLOAT3 pos;
	XMFLOAT3 dis;

	bool m_IsDead;
};
