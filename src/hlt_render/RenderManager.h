#pragma once

using Microsoft::WRL::ComPtr;
struct hlt_PSO;
class RenderManager
{
private:
    Map_Mesh* m_MapMesh = nullptr;

    /*std::vector<Mesh*> m_MeshToDrawList;
    std::vector<hlt_Transform3D*> m_MeshTransform;*/

    std::vector<ConstantBuffer*> m_ConstantBufferList;
    std::vector<ColorBuffer*> m_ColorBufferList;
    Texture* m_pBasicTexture;
protected:

    ID3D12GraphicsCommandList* m_CommandList;
    ID3D12CommandAllocator * m_DirectCmdListAlloc;

    ComPtr<ID3D12DescriptorHeap> m_SrvDescriptorHeap = nullptr;

    hlt_PSO* m_PsoManager = nullptr;

    UINT m_CbvSrvDescriptorSize = 0;

public:
    RenderManager(ID3D12GraphicsCommandList* commandList, ID3D12CommandAllocator* directCmdListAlloc);
    ~RenderManager();
    void UpdateRender(hlt_Camera* camera, std::vector<Mesh*>& meshs, std::vector<hlt_Transform3D*>& transforms);
    void UpdateColorBuffer(std::vector<Mesh*>& meshs);
    void UpdateConstantBuffer(std::vector<Mesh*>& meshs, std::vector<hlt_Transform3D*>& transforms);
    void UpdateView(hlt_Camera* camera, std::vector<Mesh*>& meshs);
    void Draw(std::vector<Mesh*>& meshs);

    ConstantBuffer* AddConstantBuffer();
    ColorBuffer* AddColorBuffer();
    void BuildDescriptorHeaps(ID3D12Device* device );
    void BuildRootSignature(ID3D12Device* device);
    void BuildShadersAndInputLayout();
    void BuildPSO(DXGI_FORMAT BackBufferFormat ,ID3D12Device* device, bool _4xMsaaState, UINT _4xMsaaQuality , DXGI_FORMAT DepthStencilFormat);
    void AddMapToRender(Map_Mesh* Map) { m_MapMesh = Map; }
};

