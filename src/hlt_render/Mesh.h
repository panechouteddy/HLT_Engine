#pragma once
#include <array>
#include <unordered_map>
#include "DirectXMath.h"
#include "DirectXColors.h"

struct MeshGeometry;
class ColorBuffer;
struct Texture;
struct Vertex
{
    XMFLOAT3 Pos;
    XMFLOAT2 TexC;
};

class Mesh
{
protected:

    XMFLOAT4 m_Color = XMFLOAT4(Colors::White);
    MeshGeometry* m_pMesh = nullptr;

    bool m_IsVisible = true;
    std::string m_MeshName;

    Texture* m_pTexture = nullptr;
public:
    Mesh();
    void SetMesh(std::string meshName, XMFLOAT3 color);
    void SetColor(XMFLOAT4 color) { m_Color = color; }
    void SetColor(XMFLOAT3 color) { m_Color = { color.x,color.y,color.z ,1 }; }
    void SetTexture(std::string meshName);

    MeshGeometry* GetGeometry();
    Texture* GetTexture() { return m_pTexture; }
    bool MeshIsVisible() { return m_IsVisible; }
    void SetMeshVisibility(bool state) { m_IsVisible = state; }
    std::string GetMeshName() { return m_MeshName; }
    XMFLOAT4* GetColor() { return &m_Color; }
};

class MeshBox
{
protected:
    std::unordered_map<std::string, MeshGeometry*> m_BoxOfMesh;
public:
    MeshBox() {};
    bool IsAllreadyCreated(std::string form) { return m_BoxOfMesh.contains(form);}
    MeshGeometry* GetMesh(std::string form) { return m_BoxOfMesh.contains(form) ? m_BoxOfMesh[form] : nullptr; }
    void CreateAllMesh(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);

    void CreateMesh(std::string name, std::vector<Vertex>& vertexList, std::vector<uint16_t>& indexList);
    void CreatePyramid(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
    void CreateCube(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
    void  CreateRock(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
};


struct Map_Mesh
{
    std::vector<std::pair<Mesh*, hlt_Transform3D>> Meshs;
    std::vector<ColorBuffer*> ColorBuffers;
    std::vector<ConstantBuffer*> ConstantBuffers;
};
