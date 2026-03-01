#pragma once
class mesh;
class Material;
class MeshMap
{
	std::vector<Material*> m_MeshMaterialList;
	std::vector<XMFLOAT4*> m_MeshColorList;
public :
	void CreateSandBoxMap();

};

