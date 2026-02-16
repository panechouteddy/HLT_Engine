#include "pch.h"
#include "main.h"
#include <d3d11on12.h>
#include <hlt_render/InitDirecX3DApp.hpp>

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

struct ObjectConstants
{
	XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
	XMFLOAT4X4 ViewProj = MathHelper::Identity4x4();

	XMFLOAT4 PulseColor = { 1.0f, 1.0f, 1.0f, 1.0f }; // Nouvelle variable
};

class main : public D3DApp
{
public:
	main(HINSTANCE hInstance);
	~main();

	bool Initialize();
	void BuildDescriptorHeaps();
	void BuildConstantBuffers();
	void BuildRootSignature();
	void BuildShadersAndInputLayout();
	void BuildBoxGeometry();
	void BuildPSO();
	void UpdateLight(const GameTimer& gt);
	void LoadTextures();

private:

	virtual void OnResize()override;
	virtual void Update(const GameTimer& gt)override;
	virtual void Draw(const GameTimer& gt)override;

	virtual void OnMouseDown(WPARAM btnState, int x, int y)override;
	virtual void OnMouseUp(WPARAM btnState, int x, int y)override;
	virtual void OnMouseMove(WPARAM btnState, int x, int y)override;

	ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;

	/*std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;
	std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB2 = nullptr;*/

	ComPtr<ID3DBlob> mvsByteCode = nullptr;
	ComPtr<ID3DBlob> mpsByteCode = nullptr;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;
	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures;
	std::unordered_map<std::string, ComPtr<ID3DBlob>> mShaders;

	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

	ComPtr<ID3D12DescriptorHeap> mSrvDescriptorHeap = nullptr;
	
	ComPtr<ID3D12PipelineState> mPSO = nullptr;
	std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> mPSOs;

	//FrameResource* mCurrFrameResource = nullptr;
	std::shared_ptr<hlt_D2DResource> m_DeviceResources;
	ComPtr<IDWriteTextFormat> m_textFormatBody; 
	ComPtr<ID2D1SolidColorBrush> m_textBrush;

	ComPtr<ID3D11Device> m_d3d11Device;
	ComPtr<ID3D11DeviceContext> m_d3d11DeviceContext;
	ComPtr<ID3D11On12Device> m_d3d11On12Device;
	ComPtr<ID2D1DeviceContext2> m_d2dContext;

	// Ressources enveloppées (une par back buffer)
	ComPtr<ID3D11Resource> m_wrappedBackBuffers[SwapChainBufferCount];

	XMFLOAT4X4 mWorld = MathHelper::Identity4x4();

	POINT mLastMousePos;

	float mTheta = 1.5f * XM_PI;
	float mPhi = XM_PIDIV4;
	float mRadius = 5.0f;

	hlt_Material m_material;
	hlt_Light m_light;
	hlt_Camera m_camera;
	hlt_PSO m_pso;
};

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int cmdShow)
{
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	try
	{
		InitDirect3DApp theApp(hInstance);
		if (!theApp.Initialize())
			return 0;

		return theApp.Run();
	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}

main::main(HINSTANCE hInstance)
	: D3DApp(hInstance)
{
}

main::~main()
{
}

bool main::Initialize()
{
	if (!D3DApp::Initialize())
		return false;

	// Réinitialiser la liste de commandes pour enregistrer les commandes d'initialisation (textures, etc.)
	ThrowIfFailed(m_CommandList->Reset(m_DirectCmdListAlloc.Get(), nullptr));

	// 2. Initialisation du Pont D3D11on12
	// On crée un device D3D11 "virtuel" qui utilise la file de commande DX12
	UINT d3d11DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(DEBUG) || defined(_DEBUG)
	d3d11DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	ThrowIfFailed(D3D11On12CreateDevice(
		m_Device.Get(),
		d3d11DeviceFlags,
		nullptr, 0,
		reinterpret_cast<IUnknown**>(m_CommandQueue.GetAddressOf()),
		1,
		0,
		&m_d3d11Device,
		&m_d3d11DeviceContext,
		nullptr
	));

	// Obtenir l'interface 11on12 pour créer les ressources enveloppées
	ThrowIfFailed(m_d3d11Device.As(&m_d3d11On12Device));

	// 3. Création des Wrapped Resources pour Direct2D
	// On crée une vue DX11 pour chaque Back Buffer de la SwapChain DX12
	D3D11_RESOURCE_FLAGS d3d11Flags = { D3D11_BIND_RENDER_TARGET };

	for (UINT i = 0; i < SwapChainBufferCount; i++)
	{
		ThrowIfFailed(m_d3d11On12Device->CreateWrappedResource(
			m_SwapChainBuffer[i].Get(),
			&d3d11Flags,
			D3D12_RESOURCE_STATE_PRESENT,     // État d'entrée
			D3D12_RESOURCE_STATE_RENDER_TARGET, // État de sortie après usage D2D
			IID_PPV_ARGS(&m_wrappedBackBuffers[i])
		));
	}

	// 4. Initialisation de Direct2D et DirectWrite
	D2D1_FACTORY_OPTIONS options = {};
#if defined(DEBUG) || defined(_DEBUG)
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

	ComPtr<ID2D1Factory3> d2dFactory;
	ThrowIfFailed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3), &options, &d2dFactory));

	ComPtr<IDXGIDevice> dxgiDevice;
	ThrowIfFailed(m_d3d11On12Device.As(&dxgiDevice));

	ComPtr<ID2D1Device2> d2dDevice;
	ThrowIfFailed(d2dFactory->CreateDevice(dxgiDevice.Get(), &d2dDevice));
	ThrowIfFailed(d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_d2dContext));

	// 5. Création des ressources de texte (DirectWrite)
	ComPtr<IDWriteFactory> writeFactory;
	ThrowIfFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &writeFactory));

	ThrowIfFailed(writeFactory->CreateTextFormat(
		L"Consolas", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		20.0f, L"en-us", &m_textFormatBody
	));

	ThrowIfFailed(m_d2dContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &m_textBrush));

	// 6. Chargement de vos ressources 3D (votre code original)
	m_material.m_Color = { 1.0f, 0.0f, 0.0f, 1.0f };
	LoadTextures();
	BuildDescriptorHeaps();
	BuildConstantBuffers();
	BuildRootSignature();
	BuildShadersAndInputLayout();
	BuildBoxGeometry();
	BuildPSO();

	// Exécuter et finaliser l'initialisation DX12
	ThrowIfFailed(m_CommandList->Close());
	ID3D12CommandList* cmdsLists[] = { m_CommandList.Get() };
	m_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	FlushCommandQueue();

	return true;
}

void main::BuildDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
	cbvHeapDesc.NumDescriptors = 1;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.NodeMask = 0;
	ThrowIfFailed(m_Device->CreateDescriptorHeap(&cbvHeapDesc,
		IID_PPV_ARGS(&mCbvHeap)));

	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(mSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	auto woodCrateTex = mTextures["woodCrateTex"]->Resource;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = woodCrateTex->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = woodCrateTex->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

	m_Device->CreateShaderResourceView(woodCrateTex.Get(), &srvDesc, hDescriptor);
}

void main::BuildConstantBuffers()
{
	/*mObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(m_Device.Get(), 1, true);
	mObjectCB2 = std::make_unique<UploadBuffer<ObjectConstants>>(m_Device.Get(), 1, true);*/

	//UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));

	//D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mObjectCB->Resource()->GetGPUVirtualAddress();
 //   // Offset to the ith object constant buffer in the buffer.
 //   int boxCBufIndex = 0;
	//cbAddress += boxCBufIndex*objCBByteSize;

	//D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	//cbvDesc.BufferLocation = cbAddress;
	//cbvDesc.SizeInBytes = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));

	//md3dDevice->CreateConstantBufferView(
	//	&cbvDesc,
	//	mCbvHeap->GetCPUDescriptorHandleForHeapStart());
}

void main::BuildRootSignature()
{
	// Shader programs typically require resources as input (constant buffers,
	// textures, samplers).  The root signature defines the resources the shader
	// programs expect.  If we think of the shader programs as a function, and
	// the input resources as function parameters, then the root signature can be
	// thought of as defining the function signature.  

	// Root parameter can be a table, root descriptor or root constants.
	CD3DX12_ROOT_PARAMETER slotRootParameter[1];

	// Create a single descriptor table of CBVs.
	CD3DX12_DESCRIPTOR_RANGE cbvTable;
	cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable);

	// A root signature is an array of root parameters.
	const int count = 2;
	CD3DX12_ROOT_PARAMETER slotRootParameters[count];
	slotRootParameters[0].InitAsConstantBufferView(0); // b0 for object
	slotRootParameters[1].InitAsConstantBufferView(1); // b1 for pass
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc = CD3DX12_ROOT_SIGNATURE_DESC(count, slotRootParameters, 0, nullptr,
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

	ThrowIfFailed(m_Device->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(&mRootSignature)));
}

void main::BuildShadersAndInputLayout()
{
	mShaders["standardVS"] = d3dUtil::CompileShader(L"src\\d3dx12.h", nullptr, "VS", "vs_5_0");
	mShaders["opaquePS"] = d3dUtil::CompileShader(L"src\\d3dx12.h", defines, "PS", "ps_5_0");
	mShaders["alphaTestedPS"] = d3dUtil::CompileShader(L"src\\d3dx12.h", alphaTestDefines, "PS", "ps_5_0");

	mInputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
}

void main::BuildBoxGeometry()
{
	std::array<Vertex, 8> vertices =
	{
		Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(m_material.m_Color) }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(m_material.m_Color) }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(m_material.m_Color) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(m_material.m_Color) }),
		Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(m_material.m_Color) }),
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

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &mBoxGeo->VertexBufferCPU));
	CopyMemory(mBoxGeo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &mBoxGeo->IndexBufferCPU));
	CopyMemory(mBoxGeo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	mBoxGeo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(m_Device.Get(),
		m_CommandList.Get(), vertices.data(), vbByteSize, mBoxGeo->VertexBufferUploader);

	mBoxGeo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(m_Device.Get(),
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

void main::BuildPSO()
{
	m_pso.m_InputLayout = mInputLayout;
	m_pso.m_RootSignature = mRootSignature;
	m_pso.m_Shaders = mShaders;
	m_pso.m_PSOs = mPSOs;

	m_pso.CreateOpaquePsoDesc(m_BackBufferFormat, m_4xMsaaState, m_4xMsaaQuality, m_DepthStencilFormat, m_Device);
	m_pso.CreateTransparentPsoDesc(m_Device);
	m_pso.CreateAlphaTestedPsoDesc(m_Device);
}

void main::OnResize()
{
	D3DApp::OnResize();

	// The window resized, so update the aspect ratio and recompute the projection matrix.
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&m_camera.m_Proj, P);
}

void main::Update(const GameTimer& gt)
{
	// Convert Spherical to Cartesian coordinates.
	m_camera.m_width = mRadius * sinf(mPhi) * cosf(mTheta);
	m_camera.m_z = mRadius * sinf(mPhi) * sinf(mTheta);
	m_camera.m_height = mRadius * cosf(mPhi);

	// Build the view matrix.
	float aspectRatio = AspectRatio();
	m_camera.Update(aspectRatio);

	XMMATRIX newWorld = XMMatrixTranslation(0, 0, 3);

	XMMATRIX world = XMLoadFloat4x4(&mWorld);
	XMMATRIX worldViewProj = world * XMLoadFloat4x4(&m_camera.m_ViewProj);

	XMMATRIX worldViewProj2 = newWorld * XMLoadFloat4x4(&m_camera.m_ViewProj);

	// Update the constant buffer with the latest worldViewProj matrix.
	ObjectConstants objConstants;
	XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(worldViewProj));

	ObjectConstants objConstants2;
	XMStoreFloat4x4(&objConstants2.WorldViewProj, XMMatrixTranspose(worldViewProj2));

	UpdateLight(gt);
	//mObjectCB->CopyData(0, objConstants);

	//mObjectCB2->CopyData(0, objConstants2);
}

void main::Draw(const GameTimer& gt)
{
	// 1. Initialisation de la liste de commandes DX12
	ThrowIfFailed(m_DirectCmdListAlloc->Reset());
	ThrowIfFailed(m_CommandList->Reset(m_DirectCmdListAlloc.Get(), mPSO.Get()));

	// 2. Setup standard (Viewport, Scissor, Barrière)
	m_CommandList->RSSetViewports(1, &m_ScreenViewport);
	m_CommandList->RSSetScissorRects(1, &m_ScissorRect);

	auto barrierToTarget = CD3DX12_RESOURCE_BARRIER::Transition(
		CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_CommandList->ResourceBarrier(1, &barrierToTarget);

	// 3. Rendu de la scène 3D (votre Cube)
	D3D12_CPU_DESCRIPTOR_HANDLE rtv = CurrentBackBufferView();
	D3D12_CPU_DESCRIPTOR_HANDLE dsv = DepthStencilView();
	m_CommandList->ClearRenderTargetView(rtv, Colors::Red, 0, nullptr);
	m_CommandList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	m_CommandList->OMSetRenderTargets(1, &rtv, true, &dsv);

	// --- (Ici vos appels DrawIndexed pour le cube) ---
	m_CommandList->SetGraphicsRootSignature(mRootSignature.Get());
	D3D12_VERTEX_BUFFER_VIEW vertex = mBoxGeo->VertexBufferView();
	m_CommandList->IASetVertexBuffers(0, 1, &vertex);
	D3D12_INDEX_BUFFER_VIEW index = mBoxGeo->IndexBufferView();
	m_CommandList->IASetIndexBuffer(&index);
	m_CommandList->DrawIndexedInstanced(mBoxGeo->DrawArgs["box"].IndexCount, 1, 0, 0, 0);

	// 4. SYNCHRONISATION : On ferme la liste DX12 pour laisser D2D prendre la main
	ThrowIfFailed(m_CommandList->Close());
	ID3D12CommandList* cmdsLists[] = { m_CommandList.Get() };
	m_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);


	ComPtr<IDXGISurface> surface;
	ThrowIfFailed(m_wrappedBackBuffers[m_CurrBackBuffer].As(&surface));

	// Créer un bitmap D2D pointant sur cette surface
	D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	ComPtr<ID2D1Bitmap1> d2dTargetBitmap;
	ThrowIfFailed(m_d2dContext->CreateBitmapFromDxgiSurface(surface.Get(), &bitmapProperties, &d2dTargetBitmap));

	// Définir la cible et dessiner
	m_d2dContext->SetTarget(d2dTargetBitmap.Get());
	m_d3d11On12Device->AcquireWrappedResources(m_wrappedBackBuffers[m_CurrBackBuffer].GetAddressOf(), 1);

	m_d2dContext->BeginDraw();

	m_d2dContext->Clear(D2D1::ColorF(D2D1::ColorF::Red, 0.2f)); // Teinte l'écran en rouge si D2D marche

	m_d2dContext->DrawRectangle(
		D2D1::RectF(50.0f, 50.0f, 300.0f, 300.0f),
		m_textBrush.Get(),
		2.0f // Épaisseur de ligne
	);

	// Dessin du texte
	std::wstring stats = L"FPS: " + std::to_wstring(1.0f / gt.DeltaTime());
	m_d2dContext->DrawText(
		stats.c_str(),
		(UINT32)stats.length(),
		m_textFormatBody.Get(),
		D2D1::RectF(15.0f, 15.0f, 500.0f, 100.0f),
		m_textBrush.Get()
	);

	m_d2dContext->EndDraw();

	// 1. Libérer la ressource UNE SEULE FOIS
	m_d3d11On12Device->ReleaseWrappedResources(m_wrappedBackBuffers[m_CurrBackBuffer].GetAddressOf(), 1);

	// 2. Retirer la cible pour libérer le lien avec le buffer
	m_d2dContext->SetTarget(nullptr);

	// 3. Envoyer les commandes D3D11 au GPU (Indispensable)
	m_d3d11DeviceContext->Flush();

	// 4. Présentation
	ThrowIfFailed(m_SwapChain->Present(0, 0));
	m_CurrBackBuffer = (m_CurrBackBuffer + 1) % SwapChainBufferCount;

	// 5. On n'appelle FlushCommandQueue() qu'APRES le Present pour ne pas bloquer le CPU
	FlushCommandQueue();
}

void main::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(m_hMainWnd);
}

void main::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void main::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePos.y));

		// Update angles based on input to orbit camera around box.
		mTheta += dx;
		mPhi += dy;

		// Restrict the angle mPhi.
		mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi - 0.1f);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to 0.005 unit in the scene.
		float dx = 0.005f * static_cast<float>(x - mLastMousePos.x);
		float dy = 0.005f * static_cast<float>(y - mLastMousePos.y);

		// Update the camera radius based on input.
		mRadius += dx - dy;

		// Restrict the radius.
		mRadius = MathHelper::Clamp(mRadius, 3.0f, 15.0f);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void main::UpdateLight(const GameTimer& gt)
{
	m_light.m_Direction = { 0.57735f, -0.57735f, 0.57735f };
	m_light.m_Strength = { 0.6f, 0.6f, 0.6f };

	//auto currPassCB = mCurrFrameResource->PassCB.get();
	//currPassCB->CopyData(0, m_light);
}

void main::LoadTextures()
{
	auto woodCrateTex = std::make_unique<Texture>();
	woodCrateTex->Name = "woodCrateTex";
	woodCrateTex->Filename = L"../../Textures/WoodCrate01.dds";
	ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(m_Device.Get(),
		m_CommandList.Get(), woodCrateTex->Filename.c_str(),
		woodCrateTex->Resource, woodCrateTex->UploadHeap));

	mTextures[woodCrateTex->Name] = std::move(woodCrateTex);
}
