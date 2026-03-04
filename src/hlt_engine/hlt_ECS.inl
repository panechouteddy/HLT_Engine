#include "hlt_ECS.h"

// MANAGE RECYCLE POOL

template<typename T>
inline void hlt_ECS::Pool<T>::Recycle(int entityID, hlt_ECS* ecs)
{
}

// MANAGE INTERN COMPONENT POOL
template<typename T>
inline hlt_ECS::ComponentPool<T>::~ComponentPool()
{
	for (int i = 0; i < component.size(); i++)
	{
		delete component[i];
	}
	component.clear();
}

template<typename T>
inline T* hlt_ECS::ComponentPool<T>::Add(int ID)
{
	if (Have(ID)) return Get(ID);

	if (ID >= entityID.capacity())
		entityID.resize(ID + 1, MISS_COMPONENT);

	component.push_back(new T());
	componentOwnerID.push_back(ID);
	size_t componentIndex = component.size() - 1;

	entityID[ID] = (int)componentIndex;

	return component[entityID[ID]];
}

template<typename T>
inline T* hlt_ECS::ComponentPool<T>::Add(int ID, T* pComponent)
{
	if (Have(ID)) return Get(ID);

	if (ID >= entityID.capacity())
		entityID.resize(ID + 1, MISS_COMPONENT);

	component.push_back(pComponent);
	size_t componentIndex = component.size() - 1;
	componentOwnerID.push_back(ID);

	entityID[ID] = (int)componentIndex;

	return component[entityID[ID]];
}

template<typename T>
inline bool hlt_ECS::ComponentPool<T>::Have(int ID)
{
	if (entityID.size() <= ID) return false;

	return entityID[ID] != MISS_COMPONENT;
}

template<typename T>
inline T* hlt_ECS::ComponentPool<T>::Get(int ID)
{
	if (Have(ID) == false) return nullptr;

	return component[entityID[ID]];
}

template<typename T>
inline std::vector<int>& hlt_ECS::ComponentPool<T>::GetComponentOwnersID()
{
	return componentOwnerID;
}

template<typename T>
inline T* hlt_ECS::ComponentPool<T>::Remove(int ID, bool toDelete)
{
	if (Have(ID) == false) return nullptr;

	int componentIndex = entityID[ID];
	size_t componentSize = component.size() - 1;

	std::swap(component[componentIndex], component.back());
	T* removed = component.back();
	component.pop_back();

	std::swap(componentOwnerID[componentIndex], componentOwnerID.back());
	componentOwnerID.pop_back();

	entityID[componentOwnerID[componentIndex]] = componentIndex;

	entityID[ID] = MISS_COMPONENT;

	return removed;
}

template<typename T>
inline void hlt_ECS::ComponentPool<T>::Recycle(int entityID, hlt_ECS* ecs)
{
	ecs->RemoveComponent<T>(entityID);
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

// MANAGE ENTITY COMPONENT
template<typename T>
inline T* hlt_ECS::AddComponent(int ID)
{
	if (m_ActiveComponents.contains(T::ID) == false)
		AddComponent<T>();

	ComponentPool<T>* componentPool = GetComponent<T>();

	if(m_WastedComponents.contains(T::ID) == false)
		return componentPool->Add(ID);
	
	Pool<T>* pool = dynamic_cast<Pool<T>*>(m_WastedComponents[T::ID]);
	if (pool->wastedComponents.size() == 0)
		return componentPool->Add(ID);
	else
		return MoveComponent<T>(ID, &m_WastedComponents, &m_ActiveComponents);
}

template<typename T>
inline T* hlt_ECS::GetComponent(int ID)
{
	if (m_ActiveComponents.contains(T::ID) == false)
		return nullptr;

	ComponentPool<T>* componentPool = GetComponent<T>();
	return componentPool->Get(ID);
}

template<typename T>
inline void hlt_ECS::SetComponentActive(int ID, bool active)
{
	std::unordered_map<int, CPool*>& pOrigin = active ? m_InactiveComponents : m_ActiveComponents;
	std::unordered_map<int, CPool*>& pDest = active ? m_ActiveComponents : m_InactiveComponents;

	MoveComponent<T>(ID, &pOrigin, &pDest);
}

template<typename T>
inline bool hlt_ECS::IsComponentActive(int ID)
{
	if (m_ActiveComponents.contains(T::ID) == false)
		return false;

	return dynamic_cast<ComponentPool<T>*>(m_ActiveComponents[T::ID])->Have(ID);
}

template<typename T>
inline void hlt_ECS::RemoveComponent(int ID)
{
	if (m_ActiveComponents.contains(T::ID) == false)
		return;

	ComponentPool<T>* componentPool = GetComponent<T>();
	T* removed = componentPool->Remove(ID);

	if (m_WastedComponents.contains(T::ID) == false)
		m_WastedComponents[T::ID] = new Pool<T>();

	Pool<T>* wasted = dynamic_cast<Pool<T>*>(m_WastedComponents[T::ID]);
	wasted->wastedComponents.push_back(removed);
}

// MANAGE COMPONENT POOL GLOBAL
template<typename T>
inline void hlt_ECS::AddComponent()
{
	if (m_ActiveComponents.contains(T::ID))
		return;

	m_ActiveComponents[T::ID] = new ComponentPool<T>();
}

template<typename T>
inline hlt_ECS::ComponentPool<T>* hlt_ECS::GetComponent()
{
	if (m_ActiveComponents.contains(T::ID) == false)
		return nullptr;

	ComponentPool<T>* componentPool = dynamic_cast<ComponentPool<T>*>(m_ActiveComponents[T::ID]);
	return componentPool;
}

template<typename T>
inline void hlt_ECS::SetComponentActive(bool active)
{
	std::unordered_map<int, CPool*>& pOrigin = active ? m_InactiveComponents : m_ActiveComponents;
	std::unordered_map<int, CPool*>& pDest = active ? m_ActiveComponents : m_InactiveComponents;

	if (pOrigin.contains(T::ID) == false) return;

	ComponentPool<T>* pOriginPool = dynamic_cast<ComponentPool<T>*>(pOrigin[T::ID]);

	for (int i = 0; i < pOriginPool->componentOwnerID.size(); i++)
	{
		MoveComponent<T>(pOriginPool->componentOwnerID[0], &pOrigin, &pDest);
	}
}

template<typename T>
inline void hlt_ECS::RemoveComponent()
{
	if (m_ActiveComponents.contains(T::ID))
		return;

	m_ActiveComponents.erase(T::ID);
}

template<typename T>
inline T* hlt_ECS::MoveComponent(int ID, std::unordered_map<int, CPool*>* from, std::unordered_map<int, CPool*>* to)
{
	if (from == nullptr || to == nullptr) return nullptr;
	if (from->contains(T::ID) == false) return nullptr;

	ComponentPool<T>* fromPool = dynamic_cast<ComponentPool<T>*>((*from)[T::ID]);
	if (fromPool == nullptr) return nullptr;

	if (fromPool->Have(ID) == false) return nullptr;

	// 2. Récupérer les données existantes
	T* component = fromPool->Get(ID);

	// 3. S'assurer que le pool de destination existe
	if (to->contains(T::ID) == false)
		(*to)[T::ID] = new ComponentPool<T>();

	ComponentPool<T>* toPool = dynamic_cast<ComponentPool<T>*>((*to)[T::ID]);

	fromPool->Remove(ID);

	return toPool->Add(ID, component);
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
