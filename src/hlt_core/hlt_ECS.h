#pragma once
#include <vector>
#include <unordered_map>
#include <memory>




#define MISS_COMPONENT -1

class hlt_System;
//struct CPool
//	{
//	};
//
//template <typename T>
//struct ComponentPool : public CPool
//	{
//		std::vector<int> entityID; // INDEX = entityID; VALUE = componentIndex
//		std::vector<T> component;
//		std::vector<int> componentOwnerID; // INDEX = componentIndex; VALUE = entityID
//
//		void Add(int ID)
//		{
//			if (Have(ID)) return;
//
//			if (ID >= entityID.capacity())
//				entityID.resize(ID + 1, MISS_COMPONENT);
//
//			component.push_back(T());
//			int componentIndex = component.size() - 1;
//			component[componentIndex].Start();
//			componentOwnerID.push_back(ID);
//
//			entityID[ID] = componentIndex;
//		}
//
//		bool Have(int ID)
//		{
//			if (entityID.size() >= ID) return false;
//
//			return entityID[ID] == MISS_COMPONENT;
//		}
//		T* Get(int ID)
//		{
//			if (Have(ID) == false) return nullptr;
//
//			return component[ID];
//		}
//
//		void Remove(int ID)
//		{
//			if (Have(ID) == false) return;
//
//			int componentIndex = entityID[ID];
//			int componentSize = component.size() - 1;
//
//			std::swap(component[componentIndex], component.back());
//			component.pop_back();
//			std::swap(componentOwnerID[componentIndex], componentOwnerID.back());
//			componentOwnerID.pop_back();
//
//			entityID[componentOwnerID[componentIndex]] = componentIndex;
//			entityID[ID] = MISS_COMPONENT;
//		}
//	};

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
	std::vector<hlt_System*> m_pSystems;

public:
	hlt_ECS() = default;
	~hlt_ECS() = default;

	void Update(float dt);

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