#pragma once

using Microsoft::WRL::ComPtr;
class RenderManager
{
private:
    std::vector<Mesh*> m_MeshToDrawList;
    std::vector<hlt_Transform3D*> m_MeshTransform;

    std::unordered_map<int, Mesh*> m;
    std::unordered_map<int, hlt_Transform3D*> tr;

    std::unordered_map<hlt_Transform3D*, Mesh*> m_Meshs;

    std::unordered_map<int, std::pair<Mesh*, hlt_Transform3D*>> t;

    std::vector<std::pair<hlt_Transform3D*, Mesh*>> a;
    

    std::vector<ConstantBuffer*> m_ConstantBufferList;
    std::vector<ColorBuffer*> m_ColorBufferList;
protected:

    ID3D12GraphicsCommandList* m_CommandList;
    ID3D12CommandAllocator * m_DirectCmdListAlloc;

    ComPtr<ID3D12RootSignature> m_RootSignature = nullptr;
    ComPtr<ID3D12DescriptorHeap> m_CbvHeap = nullptr;

    ComPtr<ID3DBlob> m_VsByteCode = nullptr;
    ComPtr<ID3DBlob> m_PsByteCode = nullptr;

    std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayout;

    ComPtr<ID3D12PipelineState> m_Pso = nullptr;


public:
    RenderManager(ID3D12GraphicsCommandList* commandList, ID3D12CommandAllocator* directCmdListAlloc);
    ~RenderManager();
    void UpdateRender(hlt_Camera* camera);
    void UpdateColorBuffer();
    void UpdateConstantBuffer();
    void UpdateView(hlt_Camera* camera);
    void Draw();

    /*void AddMeshToDraw(Mesh* mesh) { m_MeshToDrawList.push_back(mesh); }
    void AddMeshTransform(hlt_Transform3D* transform) { m_MeshTransform.push_back(transform);}*/
    void AddMeshTransform(hlt_Transform3D* transform, Mesh* mesh);

    void AddConstantBuffer();
    void AddColorBuffer();
    void BuildDescriptorHeaps(ID3D12Device* device);
    void BuildRootSignature(ID3D12Device* device);
    void BuildShadersAndInputLayout();
    void BuildPSO(ID3D12Device* device, bool _4xMsaaState, UINT _4xMsaaQuality);
};

