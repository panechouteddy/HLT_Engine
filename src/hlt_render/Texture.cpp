#include "pch.h"
#include "Texture.h"

void TextureBox::LoadAllTexture()
{

    for (const auto& entry : std::filesystem::directory_iterator(L"..\\..\\res\\Textures"))
    {
        if (!entry.is_regular_file())
            continue;


        std::filesystem::path path = entry.path();   
        std::string name = path.stem().string();     

        std::string ext = path.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        if (ext != ".dds")
            continue;
        CreateTexture(name, path.wstring());

    }
}

void TextureBox::CreateTexture(std::string name, std::wstring fileName)
{
	
	Texture* texture = new Texture;
	texture->Name = name;
	texture->Filename = fileName;

	ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(D3DApp::GetApp()->GetDevice(),D3DApp::GetApp()->GetCommandList(), texture->Filename.c_str(), texture->Resource, texture->UploadHeap));

    m_TextureBox.insert(std::make_pair(texture->Name, texture));
}

void TextureBox::CreateDefaultTexture()
{

    //Texture* texture = new Texture;
    //texture->Name = "Default";
    //texture->Filename = L"__default_white__";

    //UINT whitePixel[1] = { 0xFFFFFFFF };

    //CD3DX12_RESOURCE_DESC texDesc = CD3DX12_RESOURCE_DESC::Tex2D(
    //    DXGI_FORMAT_R8G8B8A8_UNORM,
    //    1, 
    //    1 
    //);

    //CD3DX12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    //ThrowIfFailed(D3DApp::GetApp()->GetDevice()->CreateCommittedResource(
    //    &heapProperties,
    //    D3D12_HEAP_FLAG_NONE,
    //    &texDesc,
    //    D3D12_RESOURCE_STATE_COPY_DEST,
    //    nullptr,
    //    IID_PPV_ARGS(&texture->Resource)));

    //// 5️⃣ Création Upload Heap
    //UINT64 uploadBufferSize =
    //    GetRequiredIntermediateSize(texture->Resource.Get(), 0, 1);

    //CD3DX12_RESOURCE_DESC ressourceDesk = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);
    //ThrowIfFailed(D3DApp::GetApp()->GetDevice()->CreateCommittedResource(
    //    &heapProperties,
    //    D3D12_HEAP_FLAG_NONE,
    //    &ressourceDesk,
    //    D3D12_RESOURCE_STATE_GENERIC_READ,
    //    nullptr,
    //    IID_PPV_ARGS(&texture->UploadHeap)));

    //// 6️⃣ Préparer subresource
    //D3D12_SUBRESOURCE_DATA subResourceData = {};
    //subResourceData.pData = whitePixel;
    //subResourceData.RowPitch = sizeof(UINT);
    //subResourceData.SlicePitch = sizeof(UINT);

    //// 7️⃣ Copier vers GPU
    //UpdateSubresources(D3DApp::GetApp()->GetCommandList(),
    //    texture->Resource.Get(),
    //    texture->UploadHeap.Get(),
    //    0, 0, 1,
    //    &subResourceData);

    //CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
    //    texture->Resource.Get(),
    //    D3D12_RESOURCE_STATE_COPY_DEST,
    //    D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    //// 8️⃣ Transition vers shader resource

    //D3DApp::GetApp()->GetCommandList()->ResourceBarrier(1,&barrier);

    //// 9️⃣ Créer SRV
    //UINT descriptorSize = D3DApp::GetApp()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    //texture->SrvHeapIndex = m_CurrentSrvIndex++;

    //CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(
    //    m_pSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
    //    texture->SrvHeapIndex, // slot dynamique
    //    descriptorSize);

    //D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    //srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    //srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    //srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    //srvDesc.Texture2D.MostDetailedMip = 0;
    //srvDesc.Texture2D.MipLevels = 1;
    //srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

    //D3DApp::GetApp()->GetDevice()->CreateShaderResourceView(
    //    texture->Resource.Get(),
    //    &srvDesc,
    //    hDescriptor);

    //// 10️⃣ Ajouter dans le map
    //m_TextureBox.insert(std::make_pair(texture->Name, texture));
}
