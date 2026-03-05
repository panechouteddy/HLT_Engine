#pragma once
#include <hlt_core/hlt_Box.h>

namespace hlt_Component
{
	struct BoxCollider3D
	{
		static const int ID = 4;

		enum BoxType {
			AABB,
			OBB,

			COUNT
		};

		BoxType boxType = BoxType::COUNT;

		hlt_Box::Box3D_AABB boxAABB = hlt_Box::Box3D_AABB();
		hlt_Box::Box3D_OBB boxOBB = hlt_Box::Box3D_OBB();
		bool isColliding = false;

		std::vector<int> collideWith;

		void Reset()
		{
			boxType = BoxType::COUNT;
			boxAABB = hlt_Box::Box3D_AABB();
			boxOBB = hlt_Box::Box3D_OBB();
			isColliding = false;
		}
	};
}