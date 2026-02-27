#pragma once

class Projectile
{
public:
	Projectile();
	~Projectile();

public:
	XMFLOAT3 pos;
	XMFLOAT3 dis;

	bool m_IsDead;
};
