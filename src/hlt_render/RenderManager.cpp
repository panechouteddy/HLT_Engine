#include "pch.h"
#include "RenderManager.h"
#include "UploadBuffer.h"

RenderManager::RenderManager(ID3D12GraphicsCommandList* commandList, ID3D12CommandAllocator* directCmdListAlloc)
{
    m_CommandList = commandList;
    m_DirectCmdListAlloc = directCmdListAlloc;
	m_PsoManager = new hlt_PSO;
}

RenderManager::~RenderManager()
{
	for (ConstantBuffer* cb : m_ConstantBuffers)
	{
		delete cb;
	}
	for (ColorBuffer* cb : m_ColorBuffers)
	{
		delete cb;
	}
	m_MapMesh->Meshs.clear();
}

void RenderManager::UpdateRender(hlt_Camera* camera, std::vector<Mesh*>& meshs, std::vector<hlt_Transform3D*>& transforms)
{
	UpdateColorBuffer(meshs);
	UpdateConstantBuffer(meshs, transforms);
	UpdateRenderView(camera, meshs);
}

void RenderManager::UpdateColorBuffer(std::vector<Mesh*>& meshs)
{
	for (int i = 0; i <meshs.size(); i++)
	{
		if (i >= m_ColorBuffers.size())
			m_ColorBuffers.push_back(AddColorBuffer());
		
		
		XMVECTOR color =  XMLoadFloat4(meshs[i]->GetColor());
		ColorConstants colorConstants;
		XMStoreFloat4(&colorConstants.ObjectColor, color);
		m_ColorBuffers[i]->GetBuffer()->CopyData(0, colorConstants);
	}

	if (m_MapMesh != nullptr)
	{
			for (int i = 0; i < m_MapMesh->Meshs.size(); i++)
			{
				if (i >= m_MapMesh->ColorBuffers.size())
					m_MapMesh->ColorBuffers.push_back(AddColorBuffer());

				XMVECTOR color = XMLoadFloat4(m_MapMesh->Meshs[i].first->GetColor());
				ColorConstants colorConstants;
				XMStoreFloat4(&colorConstants.ObjectColor, color);
				m_MapMesh->ColorBuffers[i]->GetBuffer()->CopyData(0, colorConstants);
			}
	}
}

void RenderManager::UpdateConstantBuffer(std::vector<Mesh*>& meshs, std::vector<hlt_Transform3D*>& transforms)
{
    for (int i = 0; i < meshs.size();i++)
    {
        if (i >= m_ConstantBuffers.size())
			m_ConstantBuffers.push_back(AddConstantBuffer());

		if (i >= transforms.size())
			m_ConstantBuffers[i]->m_World = MathHelper::Identity4x4();
		else
			m_ConstantBuffers[i]->m_World = transforms[i]->world;
    }

	if (m_MapMesh != nullptr)
	{
			for (int i = 0; i < m_MapMesh->Meshs.size(); i++)
			{
				if (i >= m_MapMesh->ConstantBuffers.size())
					m_MapMesh->ConstantBuffers.push_back(AddConstantBuffer());

				m_MapMesh->ConstantBuffers[i]->m_World = m_MapMesh->Meshs[i].second->world;
			}
	}
}

void RenderManager::UpdateRenderView(hlt_Camera* camera, std::vector<Mesh*>& meshs)
{
	for (int i = 0; i < meshs.size(); i++)
	{
		UpdateView(camera, m_ConstantBuffers[i]);
	}
	if (m_MapMesh != nullptr)
	{
		for (int i = 0; i < m_MapMesh->Meshs.size(); i++)
		{
			UpdateView(camera, m_MapMesh->ConstantBuffers[i]);
		}
	}
}

void RenderManager::UpdateView(hlt_Camera* camera, ConstantBuffer* cb)
{
	XMFLOAT4X4 CBworld = cb->m_World;
	XMMATRIX world = XMLoadFloat4x4(&CBworld);
	XMMATRIX view = XMLoadFloat4x4(&camera->m_View);
	XMMATRIX proj = XMLoadFloat4x4(&camera->m_Proj);
	XMMATRIX worldViewProj = world * view * proj;

	ObjectConstant objConstants;

	XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(worldViewProj));
	XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));
	XMStoreFloat4x4(&objConstants.TexTransform, XMMatrixScaling(1.0f, 1.0f, 1.0f));
	cb->GetBuffer()->CopyData(0, objConstants);
}

void RenderManager::Draw(std::vector<Mesh*>& meshs)
{

	ID3D12DescriptorHeap* descriptorHeaps[] = { m_SrvDescriptorHeap.Get() };
	m_CommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	m_CommandList->SetGraphicsRootSignature(m_PsoManager->m_pRootSignature.Get());
	m_CommandList->SetPipelineState(m_PsoManager->m_PSOList["opaque"].Get());


	for (int i = 0;i < meshs.size();i++)
	{
		if (meshs[i] == nullptr)
			continue;

		if (!meshs[i]->MeshIsVisible())
			continue;

		if (meshs.size() > m_ColorBuffers.size() && meshs.size() > m_ConstantBuffers.size())
			continue;

		DrawMesh(meshs[i], m_ConstantBuffers[i], m_ColorBuffers[i]);
	}
	if (m_MapMesh == nullptr)
		return;
	for (int i = 0; i < m_MapMesh->Meshs.size(); i++)
	{

		if (!m_MapMesh->Meshs[i].first->MeshIsVisible())
			continue;

		if (m_MapMesh->Meshs.size() > m_MapMesh->ColorBuffers.size() && m_MapMesh->Meshs.size() > m_MapMesh->ConstantBuffers.size())
			continue;

		DrawMesh(m_MapMesh->Meshs[i].first, m_MapMesh->ConstantBuffers[i], m_MapMesh->ColorBuffers[i]);
	}
}

void RenderManager::DrawMesh(Mesh* mesh, ConstantBuffer* cb, ColorBuffer* colorb)
{
	UINT descriptorSize = D3DApp::GetApp()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_VERTEX_BUFFER_VIEW vertexBuffer = mesh->GetGeometry()->VertexBufferView();
	m_CommandList->IASetVertexBuffers(0, 1, &vertexBuffer);

	D3D12_INDEX_BUFFER_VIEW indexBuffer = mesh->GetGeometry()->IndexBufferView();
	m_CommandList->IASetIndexBuffer(&indexBuffer);

	Texture* texture = mesh->GetTexture();

	if (texture == nullptr)
	{
		texture = D3DApp::GetApp()->GetTextureBox()->GetTexture("bricks");
	}
	CD3DX12_GPU_DESCRIPTOR_HANDLE tex(
		m_SrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart(), texture->SrvHeapIndex, descriptorSize);

	m_CommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_CommandList->SetGraphicsRootConstantBufferView(0, cb->GetResource()->GetGPUVirtualAddress());
	m_CommandList->SetGraphicsRootConstantBufferView(1, colorb->GetResource()->GetGPUVirtualAddress());
	m_CommandList->SetGraphicsRootDescriptorTable(2, tex);
	m_CommandList->DrawIndexedInstanced(mesh->GetGeometry()->DrawArgs[mesh->GetMeshName()].IndexCount, 1, 0, 0, 0);

}

ConstantBuffer* RenderManager::AddConstantBuffer()
{
    ConstantBuffer* cb = D3DApp::GetApp()->CreateConstantBufferObject();
	return cb;
}

ColorBuffer* RenderManager::AddColorBuffer()
{
	ColorBuffer* colorB = D3DApp::GetApp()->CreateColorBufferObject();
	return colorB;
}


void RenderManager::BuildDescriptorHeaps(ID3D12Device* device)
{
	TextureBox* textureBox = D3DApp::GetApp()->GetTextureBox();
	UINT textureCount = (UINT)textureBox->GetAllTexture().size();

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = textureCount;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	ThrowIfFailed(device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_SrvDescriptorHeap)));

	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(m_SrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	m_CbvSrvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

	int index = 0;
	for (auto&pair : textureBox->GetAllTexture())
	{
		srvDesc.Format = pair.second->Resource->GetDesc().Format;
		srvDesc.Texture2D.MipLevels = pair.second->Resource->GetDesc().MipLevels;

		device->CreateShaderResourceView(pair.second->Resource.Get(), &srvDesc, hDescriptor);

		pair.second->SrvHeapIndex = index;
		index++;
		hDescriptor.Offset(1, m_CbvSrvDescriptorSize);
	}

}

void RenderManager::BuildRootSignature(ID3D12Device* device)
{
	const int count = 3;

	CD3DX12_ROOT_PARAMETER rootParameters[count];

	CD3DX12_DESCRIPTOR_RANGE srvRange;
	srvRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, m_CbvSrvDescriptorSize, 0);


	rootParameters[0].InitAsConstantBufferView(0);
	rootParameters[1].InitAsConstantBufferView(1);
	rootParameters[2].InitAsDescriptorTable(1, &srvRange);
	CD3DX12_STATIC_SAMPLER_DESC sampler(
		0, 
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP);

	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(
		count,
		rootParameters,
		1,
		&sampler,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	);

	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	ThrowIfFailed(hr);

	ThrowIfFailed(device->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&m_PsoManager->m_pRootSignature)));

}

 void RenderManager::BuildShadersAndInputLayout()
{
	HRESULT hr = S_OK;

	m_PsoManager->m_Shaders["standardVS"] = d3dUtil::CompileShader(L"..\\..\\src\\hlt_render\\Shaders\\color.hlsl", nullptr, "VS", "vs_5_0");
	m_PsoManager->m_Shaders["opaquePS"] = d3dUtil::CompileShader(L"..\\..\\src\\hlt_render\\Shaders\\color.hlsl", nullptr, "PS", "ps_5_0");

	m_PsoManager->m_InputLayout =
	{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
	  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },

	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,
	  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
}

 void RenderManager::BuildPSO(DXGI_FORMAT BackBufferFormat, ID3D12Device* device, bool _4xMsaaState, UINT _4xMsaaQuality, DXGI_FORMAT DepthStencilFormat)
{

	m_PsoManager->CreateOpaquePsoDesc( BackBufferFormat, _4xMsaaState, _4xMsaaQuality, DepthStencilFormat, device);
	/*m_PsoManager->CreateTransparentPsoDesc(device);
	m_PsoManager->CreateAlphaTestedPsoDesc(device);*/

}