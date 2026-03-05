#pragma once
#include <hlt_render/Mesh.h>

namespace hlt_Component
{
	struct Mesh
	{
		static const int ID = 5;

		::Mesh mesh;

		void Reset()
		{
			mesh = ::Mesh();
		}
	};
}