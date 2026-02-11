#pragma once

#define MISS_COMPONENT -1

namespace hlt_System {
	class hlt_SystemClass;
}

class hlt_ECS
{
public:
	// COMPONENT CONTAINER
	struct CPool
	{
		virtual ~CPool() = default;
	};

	template <typename T>
	struct ComponentPool : public CPool
	{
		std::vector<int> entityID; // INDEX = entityID; VALUE = componentIndex; MISS_COMPONENT si l'entity n'a pas le component
		std::vector<T> component;
		std::vector<int> componentOwnerID; // INDEX = componentIndex; VALUE = entityID

		T* Add(int ID);

		bool Have(int ID);
		T* Get(int ID);
		std::vector<int>& GetComponentOwnersID();

		void Remove(int ID);
	};


private:
	std::unordered_map<int, CPool*> m_Components;
	std::vector<hlt_System::hlt_SystemClass*> m_pSystems;

public:
	hlt_ECS() = default;
	~hlt_ECS() = default;

	void Destroy();

	void Update();

	// MANAGE ENTITY COMPONENT
	template <typename T>
	T* AddComponent(int ID);

	 template <typename T>
	 T* GetComponent(int ID);
	
	template <typename T>
	void RemoveComponent(int ID);

	// MANAGE COMPONENT POOL GLOBAL
	template <typename T>
	void AddComponent();

	template <typename T>
	ComponentPool<T>* GetComponent();

	template <typename T>
	void RemoveComponent();

	// MANAGE SYSTEM
	template <typename T>
	void AddSystem();

	template <typename T>
	void RemoveSystem();
};