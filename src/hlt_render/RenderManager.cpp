#include "pch.h"
#include "RenderManager.h"
#include "UploadBuffer.h"

RenderManager::RenderManager(ID3D12GraphicsCommandList* commandList, ID3D12CommandAllocator* directCmdListAlloc)
{
    m_CommandList = commandList;
    m_DirectCmdListAlloc = directCmdListAlloc;
}

RenderManager::~RenderManager()
{
	/*for (Mesh* m : m_MeshToDrawList)
	{
		delete m;
	}*/
	for (ConstantBuffer* cb : m_ConstantBufferList)
	{
		delete cb;
	}
	/*for (hlt_Transform3D* mT : m_MeshTransform)
	{
		delete mT;
	}*/
	m_MapMesh->MeshContainer.clear();
}

void RenderManager::UpdateRender(hlt_Camera* camera, std::vector<Mesh*>& meshs, std::vector<hlt_Transform3D*>& transforms)
{
	UpdateColorBuffer(meshs);
	UpdateConstantBuffer(meshs, transforms);
	UpdateView(camera, meshs);
}

void RenderManager::UpdateColorBuffer(std::vector<Mesh*>& meshs)
{
	for (int i = 0; i <meshs.size(); i++)
	{
		if (i >= m_ColorBufferList.size())
			m_ColorBufferList.push_back(AddColorBuffer());
		
		
		XMVECTOR color =  XMLoadFloat4(meshs[i]->GetColor());
		ColorConstants colorConstants;
		XMStoreFloat4(&colorConstants.ObjectColor, color);
		m_ColorBufferList[i]->GetBuffer()->CopyData(0, colorConstants);
	}

	if (m_MapMesh != nullptr)
	{
		if (m_MapMesh->MapMesh_ColorBuffer.empty())
		{
			for (int i = 0; i < m_MapMesh->MeshContainer.size(); i++)
			{
				if (i >= m_MapMesh->MapMesh_ColorBuffer.size())
					m_MapMesh->MapMesh_ColorBuffer.push_back(AddColorBuffer());

				XMVECTOR color = XMLoadFloat4(m_MapMesh->MeshContainer[i].first->GetColor());
				ColorConstants colorConstants;
				XMStoreFloat4(&colorConstants.ObjectColor, color);
				m_MapMesh->MapMesh_ColorBuffer[i]->GetBuffer()->CopyData(0, colorConstants);
			}
		}
	}
}

void RenderManager::UpdateConstantBuffer(std::vector<Mesh*>& meshs, std::vector<hlt_Transform3D*>& transforms)
{
    for (int i = 0; i < meshs.size();i++)
    {
        if (i >= m_ConstantBufferList.size())
			m_ConstantBufferList.push_back(AddConstantBuffer());

		if (i >= transforms.size())
			m_ConstantBufferList[i]->SetWorldMatrix(MathHelper::Identity4x4());
		else
			m_ConstantBufferList[i]->SetWorldMatrix(transforms[i]->world);
    }

	if (m_MapMesh != nullptr)
	{
		if (m_MapMesh->MapMesh_ConstantBuffer.empty())
		{
			for (int i = 0; i < m_MapMesh->MeshContainer.size(); i++)
			{
				if (i >= m_MapMesh->MapMesh_ConstantBuffer.size())
					m_MapMesh->MapMesh_ConstantBuffer.push_back(AddConstantBuffer());

				m_MapMesh->MapMesh_ConstantBuffer[i]->SetWorldMatrix(m_MapMesh->MeshContainer[i].second->world);
			}
		}
	}
}

void RenderManager::UpdateView(hlt_Camera* camera, std::vector<Mesh*>& meshs)
{
	for (int i = 0; i < meshs.size(); i++)
	{
		XMFLOAT4X4 CBworld = m_ConstantBufferList[i]->GetWorldMatrix();
		XMMATRIX world = XMLoadFloat4x4(&CBworld);// objet
		XMMATRIX view  = XMLoadFloat4x4(&camera->m_View);
		XMMATRIX proj = XMLoadFloat4x4(&camera->m_Proj);
		XMMATRIX worldViewProj = world * view * proj ;

		// Update the constant buffer with the latest worldViewProj matrix.
		ObjectConstant objConstants;
		XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(worldViewProj));
		m_ConstantBufferList[i]->GetBuffer()->CopyData(0, objConstants);
	}
	if (m_MapMesh != nullptr)
	{
		for (int i = 0; i < m_MapMesh->MeshContainer.size(); i++)
		{
			XMFLOAT4X4 CBworld = m_MapMesh->MapMesh_ConstantBuffer[i]->GetWorldMatrix();
			XMMATRIX world = XMLoadFloat4x4(&CBworld);// objet
			XMMATRIX view = XMLoadFloat4x4(&camera->m_View);
			XMMATRIX proj = XMLoadFloat4x4(&camera->m_Proj);
			XMMATRIX worldViewProj = world * view * proj;

			// Update the constant buffer with the latest worldViewProj matrix.
			ObjectConstant objConstants;
			XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(worldViewProj));
			m_MapMesh->MapMesh_ConstantBuffer[i]->GetBuffer()->CopyData(0, objConstants);
		}
	}
}

void RenderManager::Draw(std::vector<Mesh*>& meshs)
{
	ID3D12DescriptorHeap* descriptorHeaps[] = { m_CbvHeap.Get() };
	m_CommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
	m_CommandList->SetPipelineState(m_Pso.Get());

	for (int i = 0;i < meshs.size();i++)
	{
		if (meshs[i] == nullptr)
			continue;

		if (!meshs[i]->MeshIsVisible())
			continue;

		if (meshs.size() > m_ColorBufferList.size() && meshs.size() > m_ConstantBufferList.size())
			continue;

		D3D12_VERTEX_BUFFER_VIEW vertexBuffer = meshs[i]->GetGeometry()->VertexBufferView();
		m_CommandList->IASetVertexBuffers(0, 1, &vertexBuffer);

		D3D12_INDEX_BUFFER_VIEW indexBuffer = meshs[i]->GetGeometry()->IndexBufferView();
		m_CommandList->IASetIndexBuffer(&indexBuffer);

		m_CommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->SetGraphicsRootConstantBufferView(0, m_ConstantBufferList[i]->GetResource()->GetGPUVirtualAddress());
		m_CommandList->SetGraphicsRootConstantBufferView(1, m_ColorBufferList[i]->GetResource()->GetGPUVirtualAddress());
		for (int t = 0;t < meshs[i]->GetGeometry()->DrawArgs.size();t++)
		{
			m_CommandList->DrawIndexedInstanced(
				meshs[i]->GetGeometry()->DrawArgs[meshs[i]->GetMeshName()].IndexCount,
				1, 0, 0, 0);
		}
	}
	if (m_MapMesh == nullptr)
		return;
	for (int i = 0; i < m_MapMesh->MeshContainer.size(); i++)
	{
		//if (!m_MeshToDrawList[i]->MeshIsVisible())
		//	continue;

		D3D12_VERTEX_BUFFER_VIEW vertexBuffer = m_MapMesh->MeshContainer[i].first->GetGeometry()->VertexBufferView();
		m_CommandList->IASetVertexBuffers(0, 1, &vertexBuffer);

		D3D12_INDEX_BUFFER_VIEW indexBuffer = m_MapMesh->MeshContainer[i].first->GetGeometry()->IndexBufferView();
		m_CommandList->IASetIndexBuffer(&indexBuffer);

		m_CommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->SetGraphicsRootConstantBufferView(0, m_MapMesh->MapMesh_ConstantBuffer[i]->GetResource()->GetGPUVirtualAddress());
		m_CommandList->SetGraphicsRootConstantBufferView(1, m_MapMesh->MapMesh_ColorBuffer[i]->GetResource()->GetGPUVirtualAddress());
		m_CommandList->DrawIndexedInstanced(
			m_MapMesh->MeshContainer[i].first->GetGeometry()->DrawArgs[m_MapMesh->MeshContainer[i].first->GetMeshName()].IndexCount,
				1, 0, 0, 0);

	}
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

	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
	cbvHeapDesc.NumDescriptors = 1;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.NodeMask = 0;
	ThrowIfFailed(device->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&m_CbvHeap)));
}

void RenderManager::BuildRootSignature(ID3D12Device* device)
{
	// Root parameter can be a table, root descriptor or root constants.
	CD3DX12_ROOT_PARAMETER slotRootParameter[2];

	// Create a single descriptor table of CBVs.

	slotRootParameter[0].InitAsConstantBufferView(0); // 0 <- bo 
	slotRootParameter[1].InitAsConstantBufferView(1); // 1 <- b1
	// A root signature is an array of root parameters.
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(2, slotRootParameter, 0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	ThrowIfFailed(hr);

	ThrowIfFailed(device->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature)));
}

 void RenderManager::BuildShadersAndInputLayout()
{
	HRESULT hr = S_OK;

	m_VsByteCode = d3dUtil::CompileShader(L"..\\..\\src\\hlt_render\\Shaders\\color.hlsl", nullptr, "VS", "vs_5_0");
	m_PsByteCode = d3dUtil::CompileShader(L"..\\..\\src\\hlt_render\\Shaders\\color.hlsl", nullptr, "PS", "ps_5_0");

	m_InputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
}

 void RenderManager::BuildPSO(ID3D12Device* device, bool _4xMsaaState, UINT _4xMsaaQuality)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { m_InputLayout.data(), (UINT)m_InputLayout.size() };
	psoDesc.pRootSignature = m_RootSignature.Get();
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(m_VsByteCode->GetBufferPointer()),
		m_VsByteCode->GetBufferSize()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(m_PsByteCode->GetBufferPointer()),
		m_PsByteCode->GetBufferSize()
	};
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = _4xMsaaState ? 4 : 1;
	psoDesc.SampleDesc.Quality = _4xMsaaState ? (_4xMsaaQuality - 1) : 0;
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	ThrowIfFailed(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_Pso)));
}