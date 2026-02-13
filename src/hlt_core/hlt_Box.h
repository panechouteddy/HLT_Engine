#pragma once
#include "hlt_Transform3D.h"

namespace hlt_Box
{
	class Box3D_AABB
	{
	public:
		Box3D_AABB() { Zero(); }
		Box3D_AABB(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b);
		~Box3D_AABB() = default;

		void Zero();
		DirectX::XMFLOAT3 Size();

		bool Contains(DirectX::XMFLOAT3 p);
		bool Contains(Box3D_AABB box);

		Box3D_AABB operator+(hlt_Transform3D boxPos);

	public:
		DirectX::XMFLOAT3 m_Min;
		DirectX::XMFLOAT3 m_Max;
	};

	/*class Box3D_OBB
	{
	public:
		Box3D_OBB() { Zero(); }
		Box3D_OBB(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b);
		~Box3D_OBB() = default;

		void Zero();

		bool Contains(DirectX::XMFLOAT3 p);


	public:
		DirectX::XMFLOAT3 m_Min;
		DirectX::XMFLOAT3 m_Max;
	};*/

	class Box2D
	{
	public:

		Box2D() = default;
		Box2D(DirectX::XMFLOAT2 min, DirectX::XMFLOAT2 max);
		~Box2D() = default;

		void Zero();

		bool Contains(DirectX::XMFLOAT2 p);

	public:
		DirectX::XMFLOAT2 m_Min;
		DirectX::XMFLOAT2 m_Max;
	};
}

