#pragma once
class Enemy
{
public:
	Enemy();
	~Enemy();

	void Update(int m_PlayerID, std::vector<Enemy*>* enemys);
	void Move();

public:
	XMFLOAT3 m_pos;
	XMFLOAT3 m_dir;

	int m_EnemyID = -1;
	bool m_IsDead;
	bool m_CollideOther = false;
	bool m_CollidePlayer = false;

	std::string m_Tag;

	std::vector<int> m_vCollideWith;

private:
	hlt_ECS* ecs;

	hlt_Component::ConstantMove* oCMove;
	hlt_Component::BoxCollider3D* oBox;

	bool oIsColliding = false;
	bool m_IsActive = false;
};

