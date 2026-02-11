#include "hlt_ECS.h"

// MANAGE INTERN COMPONENT POOL
template<typename T>
inline T* hlt_ECS::ComponentPool<T>::Add(int ID)
{
	if (Have(ID)) return nullptr;

	if (ID >= entityID.capacity())
		entityID.resize(ID + 1, MISS_COMPONENT);

	component.push_back(T());
	int componentIndex = component.size() - 1;
	componentOwnerID.push_back(ID);

	entityID[ID] = componentIndex;

	return &component[entityID[ID]];
}

template<typename T>
inline bool hlt_ECS::ComponentPool<T>::Have(int ID)
{
	if (entityID.size() < ID) return false;

	return entityID[ID] == MISS_COMPONENT;
}

template<typename T>
inline T* hlt_ECS::ComponentPool<T>::Get(int ID)
{
	if (Have(ID) == false) return nullptr;

	return component[ID];
}

template<typename T>
inline std::vector<int>& hlt_ECS::ComponentPool<T>::GetComponentOwnersID()
{
	return componentOwnerID;
}

template<typename T>
inline void hlt_ECS::ComponentPool<T>::Remove(int ID)
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

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

// MANAGE ENTITY COMPONENT
template<typename T>
inline T* hlt_ECS::AddComponent(int ID)
{
	if (m_Components.contains(T::ID) == false)
		AddComponent<T>();

	ComponentPool<T>* componentPool = GetComponent<T>();
	return componentPool->Add(ID);
}

template<typename T>
inline T* hlt_ECS::GetComponent(int ID)
{
	if (m_Components.contains(T::ID) == false)
		return nullptr;

	return m_Components[T::ID]->Get(ID);
}

template<typename T>
inline void hlt_ECS::RemoveComponent(int ID)
{
	if (m_Components.contains(T::ID) == false)
		return;

	ComponentPool<T>* componentPool = GetComponent<T>();
	componentPool->Remove(ID);
}

// MANAGE COMPONENT POOL GLOBAL
template<typename T>
inline void hlt_ECS::AddComponent()
{
	if (m_Components.contains(T::ID))
		return;

	m_Components[T::ID] = new ComponentPool<T>();
}

template<typename T>
inline hlt_ECS::ComponentPool<T>* hlt_ECS::GetComponent()
{
	if (m_Components.contains(T::ID) == false)
		return nullptr;


	ComponentPool<T>* componentPool = dynamic_cast<ComponentPool<T>*>(m_Components[T::ID]);
	return componentPool;
}

template<typename T>
inline void hlt_ECS::RemoveComponent()
{
	if (m_Components.contains(T::ID))
		return;

	m_Components.erase(T::ID);
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

// MANAGE SYSTEM
template<typename T>
inline void hlt_ECS::AddSystem()
{
	for (hlt_System::hlt_SystemClass* system : m_pSystems)
	{
		T* t = dynamic_cast<T*>(system);
		if (t != nullptr)
			return;
	}
	m_pSystems.push_back(new T(this));
}

template<typename T>
inline void hlt_ECS::RemoveSystem()
{
	for (int i = 0; i < m_pSystems.size(); i++)
	{
		T* t = dynamic_cast<T*>(m_pSystems[i]);
		if (t != nullptr)
		{
			m_pSystems.erase(m_pSystems.begin() + i);
			return;
		}
	}
}