#include "pch.h"
#include "D3DApp.h"

#include <array>
#include "Mesh.h"
#include "ConstantBuffer.h"

using namespace DirectX;

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

class InitDirect3DApp : public D3DApp
{
public:
	InitDirect3DApp(HINSTANCE hInstance);
	~InitDirect3DApp();

	virtual bool Initialize()override;
private:



	ComPtr<ID3D12RootSignature> m_RootSignature = nullptr;
	ComPtr<ID3D12DescriptorHeap> m_CbvHeap = nullptr;
	ComPtr<ID3D12PipelineState> m_PSO = nullptr;

	ComPtr<ID3DBlob> m_VsByteCode = nullptr;
	ComPtr<ID3DBlob> m_PsByteCode = nullptr;
	std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayout;


	std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;

	MeshManager* m_Box;
	Mesh* m_testMesh;
	ConstantBuffer* m_CBobject;


	virtual void OnResize()override;
	 void CreateAllMesh();
	virtual void Update(const GameTimer& gt)override;
	virtual void Draw(const GameTimer& gt)override;

	void BuildDescriptorHeaps();
	void BuildRootSignature();
	void BuildShadersAndInputLayout();
	void CreateMesh();
	void BuildPSO();
	void BuildBoxGeometry();

	XMFLOAT4X4 m_View = MathHelper::Identity4x4();
	XMFLOAT4X4 m_Proj = MathHelper::Identity4x4();

	float mTheta = 1.5f * XM_PI;
	float mPhi = XM_PIDIV4;
	float mRadius = 5.0f;

};



InitDirect3DApp::InitDirect3DApp(HINSTANCE hInstance)
	: D3DApp(hInstance)
{
}

InitDirect3DApp::~InitDirect3DApp()
{
}

bool InitDirect3DApp::Initialize()
{
	if (!D3DApp::Initialize())
		return false;

	BuildDescriptorHeaps();
	BuildRootSignature();
	BuildShadersAndInputLayout();
	BuildPSO();

	//m_DirectCmdListAlloc->Reset();
	//ThrowIfFailed(m_CommandList->Reset(m_DirectCmdListAlloc.Get(), nullptr));
	//ThrowIfFailed(m_CommandList->Close());
	//ID3D12CommandList* cmdsLists[] = { m_CommandList.Get() };
	//m_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	//FlushCommandQueue();

	m_DirectCmdListAlloc->Reset();
	ThrowIfFailed(m_CommandList->Reset(m_DirectCmdListAlloc.Get(), nullptr));

	CreateAllMesh();
	CreateMesh();
	m_CBobject = new ConstantBuffer(m_D3dDevice.Get());

	//m_CommandList->Close();
	ThrowIfFailed(m_CommandList->Close());
	ID3D12CommandList* cmdsLists2[] = { m_CommandList.Get() };
	m_CommandQueue->ExecuteCommandLists(_countof(cmdsLists2), cmdsLists2);

	// Wait until initialization is complete.
	FlushCommandQueue();
	return true;
}

void InitDirect3DApp::OnResize()
{
	D3DApp::OnResize();

}

void InitDirect3DApp::Update(const GameTimer& gt)
{
	// Convert Spherical to Cartesian coordinates.
	float x = mRadius * sinf(mPhi) * cosf(mTheta);
	float z = mRadius * sinf(mPhi) * sinf(mTheta);
	float y = mRadius * cosf(mPhi) + 2;

	XMVECTOR pos = XMVectorSet(x, y, z, 2.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); //cam

	XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&m_View, view);

	XMFLOAT4X4 CBworld = m_CBobject->GetWorldMatrix();
	XMMATRIX world = XMLoadFloat4x4(&CBworld);// objet

	float fovY = DirectX::XM_PIDIV4; float nearPlane = 0.01f; float farPlane = 100.f;
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(fovY, (float)m_ClientWidth / (float)m_ClientHeight, nearPlane, farPlane);
	DirectX::XMStoreFloat4x4(&m_Proj, proj);
	//XMMATRIX proj = XMLoadFloat4x4(&m_Proj); //cam
	XMMATRIX worldViewProj = world * view * proj;


	// Update the constant buffer with the latest worldViewProj matrix.
	CBfView objConstants;
	XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(worldViewProj));
	m_CBobject->GetBuffer()->CopyData(0, objConstants);


}

void InitDirect3DApp::Draw(const GameTimer& gt)
{
	// Reuse the memory associated with command recording.
	// We can only reset when the associated command lists have finished execution on the GPU.
	ThrowIfFailed(m_DirectCmdListAlloc->Reset());

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	ThrowIfFailed(m_CommandList->Reset(m_DirectCmdListAlloc.Get(), nullptr));

	// Indicate a state transition on the resource usage.
	auto barrierToRT = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_CommandList->ResourceBarrier(1, &barrierToRT);

	// Set the viewport and scissor rect.  This needs to be reset whenever the command list is reset.
	m_CommandList->RSSetViewports(1, &m_ScreenViewport);
	m_CommandList->RSSetScissorRects(1, &m_ScissorRect);

	// Clear the back buffer and depth buffer.

	D3D12_CPU_DESCRIPTOR_HANDLE currentBackBufferView = CurrentBackBufferView();
	m_CommandList->ClearRenderTargetView(currentBackBufferView, Colors::LightSteelBlue, 0, nullptr);

	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = DepthStencilView();
	m_CommandList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
	// Specify the buffers we are going to render to.
	m_CommandList->OMSetRenderTargets(1, &currentBackBufferView, true, &depthStencilView);

	ID3D12DescriptorHeap* descriptorHeaps[] = { m_CbvHeap.Get() };
	m_CommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	// Draw mesh

	m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
	m_CommandList->SetPipelineState(m_PSO.Get());

	D3D12_VERTEX_BUFFER_VIEW vertexBuffer = m_testMesh->GetGeometry()->VertexBufferView();
	m_CommandList->IASetVertexBuffers(0, 1, &vertexBuffer);
	D3D12_INDEX_BUFFER_VIEW indexBuffer = m_testMesh->GetGeometry()->IndexBufferView();
	m_CommandList->IASetIndexBuffer(&indexBuffer);
	m_CommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	// mCommandList->SetGraphicsRootDescriptorTable(0, mCbvHeap->GetGPUDescriptorHandleForHeapStart());
	m_CommandList->SetGraphicsRootConstantBufferView(0, m_CBobject->GetResource()->GetGPUVirtualAddress());
	m_CommandList->DrawIndexedInstanced(
		m_testMesh->GetGeometry()->DrawArgs["pyramid"].IndexCount,
		1, 0, 0, 0);

	//
// Indicate a state transition on the resource usage.
	auto barrierToPresent = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	m_CommandList->ResourceBarrier(1, &barrierToPresent);

	// Done recording commands.
	ThrowIfFailed(m_CommandList->Close());

	// Add the command list to the queue for execution.
	ID3D12CommandList* cmdsLists[] = { m_CommandList.Get() };
	m_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// swap the back and front buffers
	ThrowIfFailed(m_SwapChain->Present(0, 0));
	m_CurrBackBuffer = (m_CurrBackBuffer + 1) % SwapChainBufferCount;

	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	FlushCommandQueue();

}

void InitDirect3DApp::CreateAllMesh()
{
	m_Box = new MeshManager;
	m_Box->CreateAllMesh(m_D3dDevice.Get(), m_CommandList.Get());

}

inline void InitDirect3DApp::BuildDescriptorHeaps()
{

	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
	cbvHeapDesc.NumDescriptors = 1;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.NodeMask = 0;
	ThrowIfFailed(m_D3dDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&m_CbvHeap)));
}

inline void InitDirect3DApp::BuildRootSignature()
{
	// Root parameter can be a table, root descriptor or root constants.
	CD3DX12_ROOT_PARAMETER slotRootParameter[1];

	// Create a single descriptor table of CBVs.

	slotRootParameter[0].InitAsConstantBufferView(0); // 0 <- bo 

	// A root signature is an array of root parameters.
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0, nullptr,
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

	ThrowIfFailed(m_D3dDevice->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature)));
}

inline void InitDirect3DApp::BuildShadersAndInputLayout()
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

inline void InitDirect3DApp::CreateMesh()
{
	m_testMesh = new Mesh;
	m_testMesh->InitPyramidMesh(m_Box);

	m_CBobject = new ConstantBuffer(m_D3dDevice.Get());
}

inline void InitDirect3DApp::BuildPSO()
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
	psoDesc.RTVFormats[0] = m_BackBufferFormat;
	psoDesc.SampleDesc.Count = m_4xMsaaState ? 4 : 1;
	psoDesc.SampleDesc.Quality = m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;
	psoDesc.DSVFormat = m_DepthStencilFormat;
	ThrowIfFailed(m_D3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PSO)));
}

inline void InitDirect3DApp::BuildBoxGeometry()
{
	std::array<Vertex, 8> vertices =
	{
		Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White) }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black) }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) }),
		Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue) }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow) }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta) })
	};

	std::array<std::uint16_t, 36> indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	mBoxGeo = std::make_unique<MeshGeometry>();
	mBoxGeo->Name = "boxGeo";

	//ThrowIfFailed(D3DCreateBlob(vbByteSize, &mBoxGeo->VertexBufferCPU));
	//CopyMemory(mBoxGeo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);
	//
	//ThrowIfFailed(D3DCreateBlob(ibByteSize, &mBoxGeo->IndexBufferCPU));
	//CopyMemory(mBoxGeo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	mBoxGeo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(m_D3dDevice.Get(),
		m_CommandList.Get(), vertices.data(), vbByteSize, mBoxGeo->VertexBufferUploader);

	mBoxGeo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(m_D3dDevice.Get(),
		m_CommandList.Get(), indices.data(), ibByteSize, mBoxGeo->IndexBufferUploader);

	mBoxGeo->VertexByteStride = sizeof(Vertex);
	mBoxGeo->VertexBufferByteSize = vbByteSize;
	mBoxGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
	mBoxGeo->IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	mBoxGeo->DrawArgs["box"] = submesh;
}
