#pragma once
#include <array>
#include <unordered_map>
#include "DirectXMath.h"

struct MeshGeometry;
class ColorBuffer;
class MeshBox
{

protected:
    std::unordered_map<std::string, MeshGeometry*> m_BoxOfMesh;
public:
    MeshBox() {};
    MeshGeometry* GetMesh(std::string form) { return m_BoxOfMesh.contains(form) ? m_BoxOfMesh[form] : nullptr; }
    void CreateAllMesh(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);

    void CreatePyramid(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
    void CreateCube(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
};

class Mesh
{
protected:

    XMFLOAT4 m_Color = { 1.f,1.f,1.f,1.f };
    MeshGeometry* m_pMesh = nullptr;
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
    XMFLOAT4* GetColor() { return &m_Color; }
};

