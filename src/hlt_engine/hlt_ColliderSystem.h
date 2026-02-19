#pragma once


namespace hlt_System
{
	class BoxCollider : public hlt_SystemClass
	{
	public:
		BoxCollider(hlt_ECS* ecs) : hlt_SystemClass(ecs) { ; }

		void Update() override;
	};
}

