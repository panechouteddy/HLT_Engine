#pragma once

using Microsoft::WRL::ComPtr;
class RenderManager
{
private:
    std::vector<Mesh*> m_MeshToDrawList;
    std::vector<ConstantBuffer*> m_ConstantBufferList;
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
    void UpdateConstantBuffer(const std::vector<XMFLOAT4X4*>& objects);
    void UpdateView(XMFLOAT4X4 m_View);
    void Draw();
    void AddMeshToDraw(Mesh* mesh) { m_MeshToDrawList.push_back(mesh); }
    void AddConstantBuffer();
    void BuildDescriptorHeaps(ID3D12Device* device);
    void BuildRootSignature(ID3D12Device* device);
    void BuildShadersAndInputLayout();
    void BuildPSO(ID3D12Device* device, bool _4xMsaaState, UINT _4xMsaaQuality);
};

