#include "pch.h"
#include "main.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};
//
//struct ObjectConstants
//{
//	XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
//	XMFLOAT4X4 ViewProj = MathHelper::Identity4x4();
//
//	XMFLOAT4 PulseColor = { 1.0f, 1.0f, 1.0f, 1.0f }; // Nouvelle variable
//};
//
//class main : public D3DApp
//{
//public:
//	main(HINSTANCE hInstance);
//	~main();
//
//	bool Initialize();
//	void BuildDescriptorHeaps();
//	void BuildConstantBuffers();
//	void BuildRootSignature();
//	void BuildShadersAndInputLayout();
//	void BuildBoxGeometry();
//	void BuildPSO();
//	void UpdateLight(const GameTimer& gt);
//	void LoadTextures();
//
//private:
//
//	virtual void OnResize()override;
//	virtual void Update(const GameTimer& gt)override;
//	virtual void Draw(const GameTimer& gt)override;
//
//	virtual void OnMouseDown(WPARAM btnState, int x, int y)override;
//	virtual void OnMouseUp(WPARAM btnState, int x, int y)override;
//	virtual void OnMouseMove(WPARAM btnState, int x, int y)override;
//
//	ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;
//
//	/*std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;
//	std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB2 = nullptr;*/
//
//	ComPtr<ID3DBlob> mvsByteCode = nullptr;
//	ComPtr<ID3DBlob> mpsByteCode = nullptr;
//
//	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;
//
//	std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;
//	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures;
//	std::unordered_map<std::string, ComPtr<ID3DBlob>> mShaders;
//
//	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
//
//	ComPtr<ID3D12DescriptorHeap> mSrvDescriptorHeap = nullptr;
//	
//	ComPtr<ID3D12PipelineState> mPSO = nullptr;
//	std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> mPSOs;
//
//	//FrameResource* mCurrFrameResource = nullptr;
//
//	XMFLOAT4X4 mWorld = MathHelper::Identity4x4();
//
//	POINT mLastMousePos;
//
//	float mTheta = 1.5f * XM_PI;
//	float mPhi = XM_PIDIV4;
//	float mRadius = 5.0f;
//
//	hlt_Material m_material;
//	hlt_Light m_light;
//	hlt_Camera m_camera;
//	hlt_PSO m_pso;
//};

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int cmdShow)
{
	Test_GameManagerXRender();

	return 0;
}

//main::main(HINSTANCE hInstance)
//	: D3DApp(hInstance)
//{
//}
//
//main::~main()
//{
//}
//
//bool main::Initialize()
//{
//	if (!D3DApp::Initialize())
//		return false;
//
//	// Reset the command list to prep for initialization commands.
//	ThrowIfFailed(m_CommandList->Reset(m_DirectCmdListAlloc.Get(), nullptr));
//
//	m_material.m_Color = { 1.0f,0.0f,0.0f, 0.f };
//
//	LoadTextures();
//	BuildDescriptorHeaps();
//	BuildConstantBuffers();
//	BuildRootSignature();
//	BuildShadersAndInputLayout();
//	BuildBoxGeometry();
//	BuildPSO();
//
//	// Execute the initialization commands.
//	ThrowIfFailed(m_CommandList->Close());
//	ID3D12CommandList* cmdsLists[] = { m_CommandList.Get() };
//	m_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
//
//	// Wait until initialization is complete.
//	FlushCommandQueue();
//
//	return true;
//}
//
//void main::BuildDescriptorHeaps()
//{
//	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
//	cbvHeapDesc.NumDescriptors = 1;
//	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
//	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
//	cbvHeapDesc.NodeMask = 0;
//	ThrowIfFailed(m_Device->CreateDescriptorHeap(&cbvHeapDesc,
//		IID_PPV_ARGS(&mCbvHeap)));
//
//	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(mSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
//
//	auto woodCrateTex = mTextures["woodCrateTex"]->Resource;
//
//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//	srvDesc.Format = woodCrateTex->GetDesc().Format;
//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
//	srvDesc.Texture2D.MostDetailedMip = 0;
//	srvDesc.Texture2D.MipLevels = woodCrateTex->GetDesc().MipLevels;
//	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
//
//	m_Device->CreateShaderResourceView(woodCrateTex.Get(), &srvDesc, hDescriptor);
//}
//
//void main::BuildConstantBuffers()
//{
//	/*mObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(m_Device.Get(), 1, true);
//	mObjectCB2 = std::make_unique<UploadBuffer<ObjectConstants>>(m_Device.Get(), 1, true);*/
//
//	//UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
//
//	//D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mObjectCB->Resource()->GetGPUVirtualAddress();
// //   // Offset to the ith object constant buffer in the buffer.
// //   int boxCBufIndex = 0;
//	//cbAddress += boxCBufIndex*objCBByteSize;
//
//	//D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
//	//cbvDesc.BufferLocation = cbAddress;
//	//cbvDesc.SizeInBytes = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
//
//	//md3dDevice->CreateConstantBufferView(
//	//	&cbvDesc,
//	//	mCbvHeap->GetCPUDescriptorHandleForHeapStart());
//}
//
//void main::BuildRootSignature()
//{
//	// Shader programs typically require resources as input (constant buffers,
//	// textures, samplers).  The root signature defines the resources the shader
//	// programs expect.  If we think of the shader programs as a function, and
//	// the input resources as function parameters, then the root signature can be
//	// thought of as defining the function signature.  
//
//	// Root parameter can be a table, root descriptor or root constants.
//	CD3DX12_ROOT_PARAMETER slotRootParameter[1];
//
//	// Create a single descriptor table of CBVs.
//	CD3DX12_DESCRIPTOR_RANGE cbvTable;
//	cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
//	slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable);
//
//	// A root signature is an array of root parameters.
//	const int count = 2;
//	CD3DX12_ROOT_PARAMETER slotRootParameters[count];
//	slotRootParameters[0].InitAsConstantBufferView(0); // b0 for object
//	slotRootParameters[1].InitAsConstantBufferView(1); // b1 for pass
//	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc = CD3DX12_ROOT_SIGNATURE_DESC(count, slotRootParameters, 0, nullptr,
//		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
//
//	// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
//	ComPtr<ID3DBlob> serializedRootSig = nullptr;
//	ComPtr<ID3DBlob> errorBlob = nullptr;
//	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
//		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());
//
//	if (errorBlob != nullptr)
//	{
//		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
//	}
//	ThrowIfFailed(hr);
//
//	ThrowIfFailed(m_Device->CreateRootSignature(
//		0,
//		serializedRootSig->GetBufferPointer(),
//		serializedRootSig->GetBufferSize(),
//		IID_PPV_ARGS(&mRootSignature)));
//}
//
//void main::BuildShadersAndInputLayout()
//{
//	mShaders["standardVS"] = d3dUtil::CompileShader(L"src\\d3dx12.h", nullptr, "VS", "vs_5_0");
//	mShaders["opaquePS"] = d3dUtil::CompileShader(L"src\\d3dx12.h", defines, "PS", "ps_5_0");
//	mShaders["alphaTestedPS"] = d3dUtil::CompileShader(L"src\\d3dx12.h", alphaTestDefines, "PS", "ps_5_0");
//
//	mInputLayout =
//	{
//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
//		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
//		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
//	};
//}
//
//void main::BuildBoxGeometry()
//{
//	std::array<Vertex, 8> vertices =
//	{
//		Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(m_material.m_Color) }),
//		Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(m_material.m_Color) }),
//		Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(m_material.m_Color) }),
//		Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(m_material.m_Color) }),
//		Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(m_material.m_Color) }),
//		Vertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow) }),
//		Vertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan) }),
//		Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta) })
//	};
//
//
//	std::array<std::uint16_t, 36> indices =
//	{
//		// front face
//		0, 1, 2,
//		0, 2, 3,
//
//		// back face
//		4, 6, 5,
//		4, 7, 6,
//
//		// left face
//		4, 5, 1,
//		4, 1, 0,
//
//		// right face
//		3, 2, 6,
//		3, 6, 7,
//
//		// top face
//		1, 5, 6,
//		1, 6, 2,
//
//		// bottom face
//		4, 0, 3,
//		4, 3, 7
//	};
//
//	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
//	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);
//
//	mBoxGeo = std::make_unique<MeshGeometry>();
//	mBoxGeo->Name = "boxGeo";
//
//	ThrowIfFailed(D3DCreateBlob(vbByteSize, &mBoxGeo->VertexBufferCPU));
//	CopyMemory(mBoxGeo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);
//
//	ThrowIfFailed(D3DCreateBlob(ibByteSize, &mBoxGeo->IndexBufferCPU));
//	CopyMemory(mBoxGeo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);
//
//	mBoxGeo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(m_Device.Get(),
//		m_CommandList.Get(), vertices.data(), vbByteSize, mBoxGeo->VertexBufferUploader);
//
//	mBoxGeo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(m_Device.Get(),
//		m_CommandList.Get(), indices.data(), ibByteSize, mBoxGeo->IndexBufferUploader);
//
//	mBoxGeo->VertexByteStride = sizeof(Vertex);
//	mBoxGeo->VertexBufferByteSize = vbByteSize;
//	mBoxGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
//	mBoxGeo->IndexBufferByteSize = ibByteSize;
//
//	SubmeshGeometry submesh;
//	submesh.IndexCount = (UINT)indices.size();
//	submesh.StartIndexLocation = 0;
//	submesh.BaseVertexLocation = 0;
//
//	mBoxGeo->DrawArgs["box"] = submesh;
//}
//
//void main::BuildPSO()
//{
//	m_pso.m_InputLayout = mInputLayout;
//	m_pso.m_RootSignature = mRootSignature;
//	m_pso.m_Shaders = mShaders;
//	m_pso.m_PSOs = mPSOs;
//
//	m_pso.CreateOpaquePsoDesc(m_BackBufferFormat, m_4xMsaaState, m_4xMsaaQuality, m_DepthStencilFormat, m_Device);
//	m_pso.CreateTransparentPsoDesc(m_Device);
//	m_pso.CreateAlphaTestedPsoDesc(m_Device);
//}
//
//void main::OnResize()
//{
//	D3DApp::OnResize();
//
//	// The window resized, so update the aspect ratio and recompute the projection matrix.
//	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
//	XMStoreFloat4x4(&m_camera.m_Proj, P);
//}
//
//void main::Update(const GameTimer& gt)
//{
//	// Convert Spherical to Cartesian coordinates.
//	m_camera.m_width = mRadius * sinf(mPhi) * cosf(mTheta);
//	m_camera.m_z = mRadius * sinf(mPhi) * sinf(mTheta);
//	m_camera.m_height = mRadius * cosf(mPhi);
//
//	// Build the view matrix.
//	float aspectRatio = AspectRatio();
//	m_camera.Update(aspectRatio);
//
//	XMMATRIX newWorld = XMMatrixTranslation(0, 0, 3);
//
//	XMMATRIX world = XMLoadFloat4x4(&mWorld);
//	XMMATRIX worldViewProj = world * XMLoadFloat4x4(&m_camera.m_ViewProj);
//
//	XMMATRIX worldViewProj2 = newWorld * XMLoadFloat4x4(&m_camera.m_ViewProj);
//
//	// Update the constant buffer with the latest worldViewProj matrix.
//	ObjectConstants objConstants;
//	XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(worldViewProj));
//
//	ObjectConstants objConstants2;
//	XMStoreFloat4x4(&objConstants2.WorldViewProj, XMMatrixTranspose(worldViewProj2));
//
//	UpdateLight(gt);
//	//mObjectCB->CopyData(0, objConstants);
//
//	//mObjectCB2->CopyData(0, objConstants2);
//}
//
//void main::Draw(const GameTimer& gt)
//{
//	// Reuse the memory associated with command recording.
//	// We can only reset when the associated command lists have finished execution on the GPU.
//	ThrowIfFailed(m_DirectCmdListAlloc->Reset());
//
//	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
//	// Reusing the command list reuses memory.
//	ThrowIfFailed(m_CommandList->Reset(m_DirectCmdListAlloc.Get(), mPSO.Get()));
//
//	m_CommandList->RSSetViewports(1, &m_ScreenViewport);
//	m_CommandList->RSSetScissorRects(1, &m_ScissorRect);
//
//	// Indicate a state transition on the resource usage.
//	CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
//		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
//	m_CommandList->ResourceBarrier(1, &transition);
//
//	// Clear the back buffer and depth buffer.
//	XMVECTORF32 dynamicColor = { sinf(mTheta), cosf(mPhi), 0.5f, 1.0f };
//
//	D3D12_CPU_DESCRIPTOR_HANDLE currentBackBufferView = CurrentBackBufferView();
//	m_CommandList->ClearRenderTargetView(currentBackBufferView, dynamicColor, 0, nullptr);
//
//	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = DepthStencilView();
//	m_CommandList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
//
//	// Specify the buffers we are going to render to.
//	m_CommandList->OMSetRenderTargets(1, &currentBackBufferView, true, &depthStencilView);
//
//	ID3D12DescriptorHeap* descriptorHeaps[] = { mCbvHeap.Get() };
//	m_CommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
//
//	// debut obj
//	m_CommandList->SetGraphicsRootSignature(mRootSignature.Get());
//
//	D3D12_VERTEX_BUFFER_VIEW vertex = mBoxGeo->VertexBufferView();
//	m_CommandList->IASetVertexBuffers(0, 1, &vertex);
//	D3D12_INDEX_BUFFER_VIEW index = mBoxGeo->IndexBufferView();
//	m_CommandList->IASetIndexBuffer(&index);
//	m_CommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	//m_CommandList->SetGraphicsRootConstantBufferView(0, mObjectCB->Resource()->GetGPUVirtualAddress());
//
//	m_CommandList->DrawIndexedInstanced(
//		mBoxGeo->DrawArgs["box"].IndexCount,
//		1, 0, 0, 0);
//
//	// fin obj
//
//	//m_CommandList->SetGraphicsRootConstantBufferView(0, mObjectCB2->Resource()->GetGPUVirtualAddress());
//
//	m_CommandList->DrawIndexedInstanced(
//		mBoxGeo->DrawArgs["box"].IndexCount,
//		1, 0, 0, 0);
//
//	///////////////
//
//	// Indicate a state transition on the resource usage.
//	CD3DX12_RESOURCE_BARRIER transition1 = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
//		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
//	m_CommandList->ResourceBarrier(1, &transition1);
//
//	// Done recording commands.
//	ThrowIfFailed(m_CommandList->Close());
//
//	// Add the command list to the queue for execution.
//	ID3D12CommandList* cmdsLists[] = { m_CommandList.Get() };
//	m_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
//
//	// swap the back and front buffers
//	ThrowIfFailed(m_SwapChain->Present(0, 0));
//	m_CurrBackBuffer = (m_CurrBackBuffer + 1) % SwapChainBufferCount;
//
//	// Wait until frame commands are complete.  This waiting is inefficient and is
//	// done for simplicity.  Later we will show how to organize our rendering code
//	// so we do not have to wait per frame.
//	FlushCommandQueue();
//}
//
//void main::OnMouseDown(WPARAM btnState, int x, int y)
//{
//	mLastMousePos.x = x;
//	mLastMousePos.y = y;
//
//	SetCapture(m_hMainWnd);
//}
//
//void main::OnMouseUp(WPARAM btnState, int x, int y)
//{
//	ReleaseCapture();
//}
//
//void main::OnMouseMove(WPARAM btnState, int x, int y)
//{
//	if ((btnState & MK_LBUTTON) != 0)
//	{
//		// Make each pixel correspond to a quarter of a degree.
//		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePos.x));
//		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePos.y));
//
//		// Update angles based on input to orbit camera around box.
//		mTheta += dx;
//		mPhi += dy;
//
//		// Restrict the angle mPhi.
//		mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi - 0.1f);
//	}
//	else if ((btnState & MK_RBUTTON) != 0)
//	{
//		// Make each pixel correspond to 0.005 unit in the scene.
//		float dx = 0.005f * static_cast<float>(x - mLastMousePos.x);
//		float dy = 0.005f * static_cast<float>(y - mLastMousePos.y);
//
//		// Update the camera radius based on input.
//		mRadius += dx - dy;
//
//		// Restrict the radius.
//		mRadius = MathHelper::Clamp(mRadius, 3.0f, 15.0f);
//	}
//
//	mLastMousePos.x = x;
//	mLastMousePos.y = y;
//}
//
//void main::UpdateLight(const GameTimer& gt)
//{
//	m_light.m_Direction = { 0.57735f, -0.57735f, 0.57735f };
//	m_light.m_Strength = { 0.6f, 0.6f, 0.6f };
//
//	//auto currPassCB = mCurrFrameResource->PassCB.get();
//	//currPassCB->CopyData(0, m_light);
//}
//
//void main::LoadTextures()
//{
//	auto woodCrateTex = std::make_unique<Texture>();
//	woodCrateTex->Name = "woodCrateTex";
//	woodCrateTex->Filename = L"../../Textures/WoodCrate01.dds";
//	ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(m_Device.Get(),
//		m_CommandList.Get(), woodCrateTex->Filename.c_str(),
//		woodCrateTex->Resource, woodCrateTex->UploadHeap));
//
//	mTextures[woodCrateTex->Name] = std::move(woodCrateTex);
//}
