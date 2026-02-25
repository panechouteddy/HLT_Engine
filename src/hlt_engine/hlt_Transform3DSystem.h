#pragma once

namespace hlt_System
{
	class hlt_Transform3DSystem : public hlt_SystemClass
	{
	public:
		hlt_Transform3DSystem(hlt_ECS* ecs) : hlt_SystemClass(ecs) { ; }

		void Update() override;
	};
}
