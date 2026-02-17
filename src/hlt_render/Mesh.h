#pragma once
#include <array>
#include <unordered_map>
#include "DirectXMath.h"
#include "d3dUtil.h"
class MeshBox
{

protected:
    std::unordered_map<std::string, MeshGeometry*> m_BoxOfMesh;
public:
    MeshBox() {};
    MeshGeometry* GetMesh(std::string form) { return m_BoxOfMesh[form]; }
    void CreateAllMesh(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
    void CreatePyramid(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);

};
class Mesh
{
    MeshGeometry* m_Mesh = nullptr;
    bool IsVisible = true ;

public :
    Mesh() {};
    void InitPyramidMesh(MeshBox * Manager);
    MeshGeometry* GetGeometry();
};

