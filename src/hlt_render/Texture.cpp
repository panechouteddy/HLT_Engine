#include "pch.h"
#include "Texture.h"


void TextureBox::CreateAllTexture(std::vector<std::pair<std::string, std::wstring>>& fileList)
{
	for (std::pair<std::string, std::wstring> list : fileList)
	{
		CreateTexture(list.first, list.second);
	}
}

void TextureBox::CreateTexture(std::string name, std::wstring fileName)
{
	
	Texture* texture = new Texture;
	texture->Name = name;
	texture->Filename = fileName;
	ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(D3DApp::GetApp()->GetDevice(),D3DApp::GetApp()->GetCommandList(), texture->Filename.c_str(), texture->Resource, texture->UploadHeap));

	UINT descriptorSize =D3DApp::GetApp()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	texture->SrvHeapIndex = m_CurrentSrvIndex++;

	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(
		D3DApp::GetApp()->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
		2, // <- IMPORTANT : slot 2
		descriptorSize);


	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = texture->Resource->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texture->Resource->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

	D3DApp::GetApp()->GetDevice()->CreateShaderResourceView(texture->Resource.Get(), &srvDesc, hDescriptor);
	m_TextureBox.insert(std::make_pair(name, texture));
}

void TextureBox::CreateDefaultTexture()
{
	Texture* texture = new Texture;
	texture->Name = "Default";
	UINT whitePixel[1] = { 0xffffffff };

	CD3DX12_RESOURCE_DESC texDesc =CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM,1, 1);

	CD3DX12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	ThrowIfFailed(D3DApp::GetApp()->GetDevice()->CreateCommittedResource(&heapProperties,D3D12_HEAP_FLAG_NONE,&texDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,nullptr,IID_PPV_ARGS(&texture->Resource)));

	UINT descriptorSize =D3DApp::GetApp()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	texture->SrvHeapIndex = m_CurrentSrvIndex++;

	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(D3DApp::GetApp()->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),texture->SrvHeapIndex,   
	descriptorSize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

	D3DApp::GetApp()->GetDevice()->CreateShaderResourceView(
		texture->Resource.Get(),
		&srvDesc,
		hDescriptor);

	m_TextureBox.insert(std::make_pair(texture->Name, texture));

}
