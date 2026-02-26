#pragma once

namespace hlt_Prefab
{
	class GameObject
	{
	public:
		static int CreateCube();
		static int CreateCube(XMFLOAT3 pos, XMFLOAT3 sca);
		static int CreateRock();
		static int CreateGround();
		static int CreatePyramid();
	};
	class MeshObject
	{
	public:
		static Mesh* CreateCube();
		static Mesh* CreateCube(XMFLOAT3 pos, XMFLOAT3 sca);
		static Mesh* CreateRock();
		static Mesh* CreateGround();
		static Mesh* CreatePyramid();

	};
}