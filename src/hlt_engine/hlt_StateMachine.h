#pragma once

template <typename T>
struct hlt_StateMachineComponent
{
	struct StateLogic
	{
		hlt_Function pEnter;
		hlt_Function pUpdate;
		hlt_Function pExit;
	};

	// DIFFERENT STATES METHODS
	static inline std::vector<hlt_StateLogic> m_StatesFunctions;

	// STATE
	T m_CurrentState;
	T m_PreviousState;
	float m_Time = 0.f;
};

namespace hlt_System
{
	template <typename T>
	class StateMachine : public hlt_SystemClass
	{
	public:
		StateMachine(hlt_ECS* ecs) : hlt_SystemClass(ecs) { ; }

		void Update() override
		{
			hlt_ECS::ComponentPool<hlt_StateMachineComponent<T>>* stateMachines = m_pECS->GetComponent<hlt_StateMachineComponent<T>>();

			if (stateMachines == nullptr) return;

			std::vector<hlt_StateMachineComponent<T>::StateLogic>& stateLogics = hlt_StateMachineComponent<T>::StateLogic;

			for (int i = 0; i < stateMachines->componentOwnerID.size(); i++)
			{
				hlt_StateMachineComponent<T>& machine = stateMachines->component[i];
				int entityID = stateMachines->componentOwnerID[i];

				if (machine.m_StatesFunctions.size() < T::COUNT + 1)
				{
					std::cout << "State Machine Error: StatesFunctions doesn't match size with " << typeid(T).name() << std::endl;
					continue;
				}

				if (HaveChanged(machine))
				{
					if(machine.m_StatesFunctions[machine.m_CurrentState].pExit.m_pWrapper != nullptr)
						machine.m_StatesFunctions[machine.m_CurrentState].pExit.Execute();

					if (machine.m_StatesFunctions[machine.m_CurrentState].pStart.m_pWrapper != nullptr)
						machine.m_StatesFunctions[machine.m_CurrentState].pStart.Execute();
				}

				machine.m_Time += HLT_TIME.GetDeltaTime();
				if (machine.m_StatesFunctions[machine.m_CurrentState].pUpdate.m_pWrapper != nullptr)
					machine.m_StatesFunctions[machine.m_CurrentState].pUpdate.Execute();

				comp.m_Time += dt;
				size_t currIdx = static_cast<size_t>(comp.m_CurrentState);
				if (currIdx < allLogics.size() && allLogics[currIdx].update) {
					allLogics[currIdx].update(entityID, dt);
			}
		}

	private:
		void ResetTime(hlt_StateMachineComponent<T>& stateMachine) { stateMachine.m_State.m_Time = 0.f; }

		bool HaveChanged(hlt_StateMachineComponent<T>& stateMachine) { return stateMachine.m_State.m_CurrentState != stateMachine.m_State.m_PreviousState; }
		void SetState(hlt_StateMachineComponent<T>& stateMachine, T newState)
		{
			stateMachine.m_State.m_PreviousState = stateMachine.m_State.m_CurrentState;
			stateMachine.m_State.m_CurrentState = newState;
			ResetTime();
		}
	};
}
{
	auto* pool = ecs.GetComponent<hlt_StateMachineComponent<T>>();
	if (!pool) return;

	const auto& allLogics = hlt_StateMachineComponent<T>::m_StatesFunctions;

	for (size_t i = 0; i < pool->component.size(); ++i) {
		auto& comp = pool->component[i];
		int entityID = pool->componentOwnerID[i];

		// --- GESTION DU CHANGEMENT D'ÉTAT ---
		if (comp.m_CurrentState != comp.m_PreviousState)
		{
			// 1. Appeler le "End" de l'ancien état
			size_t prevIdx = static_cast<size_t>(comp.m_PreviousState);
			if (prevIdx < allLogics.size() && allLogics[prevIdx].end) {
				allLogics[prevIdx].end(entityID, dt);
			}

			// 2. Appeler le "Start" du nouvel état
			size_t currIdx = static_cast<size_t>(comp.m_CurrentState);
			if (currIdx < allLogics.size() && allLogics[currIdx].start) {
				allLogics[currIdx].start(entityID, dt);
			}

			// 3. Synchroniser
			comp.m_PreviousState = comp.m_CurrentState;
		}

		// --- UPDATE CLASSIQUE ---
		comp.m_Time += dt;
		size_t currIdx = static_cast<size_t>(comp.m_CurrentState);
		if (currIdx < allLogics.size() && allLogics[currIdx].update) {
			allLogics[currIdx].update(entityID, dt);
		}
	}
}