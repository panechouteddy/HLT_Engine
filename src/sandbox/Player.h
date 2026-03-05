#pragma once


class Player
{
public:
	Player(hlt_ECS* ecs);
	~Player() { if (m_pECS != nullptr) m_pECS->RemoveEntity(m_ID); }

	void Update();

	void TakeDamage();

	bool HaveCollidedWith(int eID);

	int GetHP() { return m_HP; }
	void SetHP(int hp) { m_HP = hp; }

private:
	int m_HP = 10;
	int m_Score = 0;

	bool m_IsDead = false;

private:
	float m_DamageCooldown = 1.f;
	float m_TimeSinceLastHit = 0.f;
	bool* m_IsColliding = nullptr;

private:
	hlt_ECS* m_pECS = nullptr;

public:
	int m_ID = -1;
	hlt_Component::Transform3D* m_pTransform = nullptr;
};

