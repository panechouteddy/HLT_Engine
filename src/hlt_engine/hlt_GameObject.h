#pragma once


class hlt_GameObject final
{
private:
	static hlt_ECS* m_pECS;

	bool m_IsInit = false;
	int m_EntityID;

	float m_BornHour;

public:
	hlt_GameObject(int eID);
	~hlt_GameObject() = default;

	void Init();
	bool IsInit() { return m_IsInit; }

	float GetAliveSince();

	void Update();

	void SetPosition(XMFLOAT3 newPos);
	XMFLOAT3 GetPosition();

	void SetScale(XMFLOAT3 newScale);
	XMFLOAT3 GetScale();

	template <typename Component>
	Component* GetComponent();
};

template <typename Component>
inline Component* hlt_GameObject::GetComponent()
{
	if (m_IsInit == false)
		Init();
	if (m_pECS == nullptr)
		return nullptr;

	return m_pECS->GetComponent<Component>(m_EntityID);
}