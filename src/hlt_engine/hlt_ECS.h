#pragma once

#define MISS_COMPONENT -1

namespace hlt_System {
	class hlt_SystemClass;
}

class hlt_ECS
{
private:
	// COMPONENT CONTAINER
	struct CPool
	{
		virtual ~CPool() = default;
		virtual void Recycle(int entityID, hlt_ECS* ecs) = 0;
	};

	template <typename T>
	struct Pool : public CPool
	{
		std::vector<T*> wastedComponents;

		void Recycle(int entityID, hlt_ECS* ecs) override;
	};

public:
	template <typename T>
	struct ComponentPool : public CPool
	{
		std::vector<int> entityID;			 // INDEX = entityID; VALUE = componentIndex; MISS_COMPONENT si l'entity n'a pas le component
		std::vector<T*> component;
		std::vector<int> componentOwnerID;	 // INDEX = componentIndex; VALUE = entityID

		~ComponentPool();

		T* Add(int ID);
		T* Add(int ID, T* pComponent);

		bool Have(int ID);
		T* Get(int ID);
		std::vector<int>& GetComponentOwnersID();

		T* Remove(int ID, bool toDelete = false);

		void Recycle(int entityID, hlt_ECS* ecs) override;
	};

private:
	std::unordered_map<int, CPool*> m_ActiveComponents;
	std::unordered_map<int, CPool*> m_InactiveComponents;
	std::unordered_map<int, CPool*> m_WastedComponents;
	std::vector<hlt_System::hlt_SystemClass*> m_pSystems;

public:	
	hlt_ECS() = default;
	~hlt_ECS();

	void Destroy();

	void Update();

	// TO COMPLETELY REMOVE AN ENTITY
	void RemoveEntity(int ID);

	// MANAGE ENTITY COMPONENT
	template <typename T>
	T* AddComponent(int ID);

	 template <typename T>
	 T* GetComponent(int ID);

	 template <typename T>
	 void SetComponentActive(int ID, bool active);

	 template <typename T>
	 bool IsComponentActive(int ID);
	
	template <typename T>
	void RemoveComponent(int ID);

	// MANAGE COMPONENT POOL GLOBAL
	template <typename T>
	void AddComponent();

	template <typename T>
	ComponentPool<T>* GetComponent();

	template <typename T>
	void SetComponentActive(bool active);

	template <typename T>
	void RemoveComponent();

	// MANAGE SYSTEM
	template <typename T>
	void AddSystem();

	template <typename T>
	void RemoveSystem();

private:
	template <typename T>
	T* MoveComponent(int ID, std::unordered_map<int, CPool*>* from, std::unordered_map<int, CPool*>* to);
};
