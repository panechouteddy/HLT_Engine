#pragma once
#include <vector>
#include <unordered_map>
#include <memory>

#define MISS_COMPONENT -1

struct CPool
	{
	};

template <typename T>
struct ComponentPool : public CPool
	{
		std::vector<int> entityID; // INDEX = entityID; VALUE = componentIndex
		std::vector<T> component;
		std::vector<int> componentOwnerID; // INDEX = componentIndex; VALUE = entityID

		void Add(int ID)
		{
			if (Have(ID)) return;

			if (ID >= entityID.capacity())
				entityID.resize(ID + 1, MISS_COMPONENT);

			component.push_back(T());
			int componentIndex = component.size() - 1;
			component[componentIndex].Start();
			componentOwnerID.push_back(ID);

			entityID[ID] = componentIndex;
		}

		bool Have(int ID)
		{
			if (entityID.size() >= ID) return false;

			return entityID[ID] == MISS_COMPONENT;
		}
		T* Get(int ID)
		{
			if (Have(ID) == false) return nullptr;

			return component[ID];
		}

		void Remove(int ID)
		{
			if (Have(ID) == false) return;

			int componentIndex = entityID[ID];
			int componentSize = component.size() - 1;

			std::swap(component[componentIndex], component.back());
			component.pop_back();
			std::swap(componentOwnerID[componentIndex], componentOwnerID.back());
			componentOwnerID.pop_back();

			entityID[componentOwnerID[componentIndex]] = componentIndex;
			entityID[ID] = MISS_COMPONENT;
		}
	};

class ECS
{
private:
	std::unordered_map<int, CPool*> m_Components;

public:
	ECS() = default;
	~ECS() = default;

	template <typename T>
	void AddComponent(int ID);

	 template <typename T>
	 T* GetComponent(int ID);
	
	template <typename T>
	void RemoveComponent(int ID);

	template <typename T>
	void CreateComponent();

	template <typename T>
	CPool* GetComponent();

	template <typename T>
	void DeleteComponent();
};

template<typename T>
inline void ECS::AddComponent(int ID)
{
	if (m_Components.contains(T::ID) == false)
		CreateComponent<T>();

	m_Components[T::ID]->Add(ID);
}

template<typename T>
inline T* ECS::GetComponent(int ID)
{
	if (m_Components.contains(T::ID) == false)
		return nullptr;

	return m_Components[T::ID]->Get(ID);
}

template<typename T>
inline void ECS::RemoveComponent(int ID)
{
	if (m_Components.contains(T::ID) == false)
		return;

	m_Components[T::ID]->Remove(ID);
}

template<typename T>
inline void ECS::CreateComponent()
{
	if (m_Components.contains(T::ID))
		return;

	m_Components[T::ID] = new ComponentPool<T>();
}

template<typename T>
inline CPool* ECS::GetComponent()
{
	if (m_Components.contains(T::ID) == false)
		return nullptr;

	return m_Components[T::ID];
}

template<typename T>
inline void ECS::DeleteComponent()
{
	if (m_Components.contains(T::ID))
		return;

	m_Components.erase(T::ID);
}
