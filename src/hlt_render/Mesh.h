#pragma once
#include <array>
#include <unordered_map>
#include "DirectXMath.h"

struct MeshGeometry;

class MeshBox
{

protected:
    std::unordered_map<std::string, MeshGeometry*> m_BoxOfMesh;
public:
    MeshBox() {};
    MeshGeometry* GetMesh(std::string form) { return m_BoxOfMesh.contains(form) ? m_BoxOfMesh[form] : nullptr; }
    void CreateAllMesh(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
    void CreatePyramid(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);

};

class Mesh
{
protected:
    MeshGeometry* m_Mesh = nullptr;
    bool m_IsVisible = false ;
    std::string m_MeshName;

public :
    Mesh() {};
    void SetMesh(std::string meshName);
    void InitPyramidMesh();
    MeshGeometry* GetGeometry();
    bool MeshIsVisible() { return m_IsVisible ;}
    void SetMeshVisibility(bool state) { m_IsVisible = state; }
    std::string GetMeshName() { return m_MeshName;}
};

