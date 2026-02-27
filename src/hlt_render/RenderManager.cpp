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
	for (Mesh* m : m_MeshToDrawList)
	{
		delete m;
	}
	for (ConstantBuffer* cb : m_ConstantBufferList)
	{
		delete cb;
	}
	for (hlt_Transform3D* mT : m_MeshTransform)
	{
		delete mT;
	}
	m_MapMesh->MeshContainer.clear();
}

void RenderManager::UpdateRender(hlt_Camera* camera)
{
	UpdateColorBuffer();
	UpdateConstantBuffer();
	UpdateView(camera);
}

void RenderManager::UpdateColorBuffer()
{
	for (int i = 0; i <m_MeshToDrawList.size(); i++)
	{
		if (i >= m_ColorBufferList.size())
			m_ColorBufferList.push_back(AddColorBuffer());
		
		
		XMVECTOR color =  XMLoadFloat4(m_MeshToDrawList[i]->GetColor());
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

void RenderManager::UpdateConstantBuffer()
{


    for (int i = 0; i < m_MeshToDrawList.size();i++)
    {
        if (i >= m_ConstantBufferList.size())
			m_ConstantBufferList.push_back(AddConstantBuffer());

		if (i >= m_MeshTransform.size())
			m_ConstantBufferList[i]->SetWorldMatrix(MathHelper::Identity4x4());
		else
			m_ConstantBufferList[i]->SetWorldMatrix(m_MeshTransform[i]->world);
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

void RenderManager::UpdateView(hlt_Camera* camera)
{
	for (int i = 0; i < m_MeshToDrawList.size(); i++)
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

void RenderManager::Draw()
{

	ID3D12DescriptorHeap* descriptorHeaps[] = { m_CbvHeap.Get() };
	m_CommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
	m_CommandList->SetPipelineState(m_Pso.Get());

	for (int i = 0;i < m_MeshToDrawList.size();i++)
	{

		if (m_MeshToDrawList[i] == nullptr)
			continue;

		if (m_MeshToDrawList.size() > m_ColorBufferList.size() && m_MeshToDrawList.size() > m_ConstantBufferList.size())
			continue;
		//if (!m_MeshToDrawList[i]->MeshIsVisible())
		//	continue;

		D3D12_VERTEX_BUFFER_VIEW vertexBuffer = m_MeshToDrawList[i]->GetGeometry()->VertexBufferView();
		m_CommandList->IASetVertexBuffers(0, 1, &vertexBuffer);

		D3D12_INDEX_BUFFER_VIEW indexBuffer = m_MeshToDrawList[i]->GetGeometry()->IndexBufferView();
		m_CommandList->IASetIndexBuffer(&indexBuffer);

		m_CommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->SetGraphicsRootConstantBufferView(0, m_ConstantBufferList[i]->GetResource()->GetGPUVirtualAddress());
		m_CommandList->SetGraphicsRootConstantBufferView(1, m_ColorBufferList[i]->GetResource()->GetGPUVirtualAddress());
		for (int t = 0;t < m_MeshToDrawList[i]->GetGeometry()->DrawArgs.size();t++)
		{
			m_CommandList->DrawIndexedInstanced(
				m_MeshToDrawList[i]->GetGeometry()->DrawArgs[m_MeshToDrawList[i]->GetMeshName()].IndexCount,
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

void RenderManager::AddMeshTransform(hlt_Transform3D* transform, Mesh* mesh)
{
	m_MeshToDrawList.push_back(mesh);
	m_MeshTransform.push_back(transform);
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
	const int count = 3;
	CD3DX12_ROOT_PARAMETER slotRootParameter[3];

	// Create a single descriptor table of CBVs.
	for (int i = 0; i < count; i++)
	{
		slotRootParameter[i].InitAsConstantBufferView(0); // 0 <- bo 
	}
	// 1 <- b1
	// 2 <- b2
	// A root signature is an array of root parameters.
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(count, slotRootParameter, 0, nullptr,
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
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
}

 void RenderManager::BuildPSO(DXGI_FORMAT BackBufferFormat, ID3D12Device* device, bool _4xMsaaState, UINT _4xMsaaQuality, DXGI_FORMAT DepthStencilFormat)
{


	 m_PsoManager = new hlt_PSO;
	m_PsoManager->m_InputLayout = m_InputLayout;
	m_PsoManager->m_RootSignature = m_RootSignature;

	m_PsoManager->CreateOpaquePsoDesc(BackBufferFormat, _4xMsaaState, _4xMsaaQuality, DepthStencilFormat,device);
	m_PsoManager->CreateTransparentPsoDesc(device);
	m_PsoManager->CreateAlphaTestedPsoDesc(device);

}