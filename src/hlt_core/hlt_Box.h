#pragma once
#include "hlt_Transform3D.h"

namespace hlt_Box
{
	class Box3D
	{
	public:
		Box3D() { Zero(); }
		Box3D(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b);
		~Box3D() = default;

		void Zero();

		bool Contains(DirectX::XMFLOAT3 p);


	public:
		DirectX::XMFLOAT3 m_Min;
		DirectX::XMFLOAT3 m_Max;
	};

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

