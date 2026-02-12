#include "pch.h"
#include "D3DApp.h"

#include <array>
#include "Mesh.h"
#include "ConstantBuffer.h"

using namespace DirectX;


class InitDirect3DApp : public D3DApp
{
public:
	InitDirect3DApp(HINSTANCE hInstance);
	~InitDirect3DApp();
	
	virtual bool Initialize()override;
private:
	virtual void OnResize()override;

	virtual void Update(const GameTimer& gt)override;
	virtual void Draw(const GameTimer& gt)override;
	MeshManager* m_Box;
	Mesh* m_testMesh;
	ConstantBuffer* m_CBobject;
	virtual void CreateAllMesh();
	void CreateMesh();


	XMFLOAT4X4 mView = MathHelper::Identity4x4();
	XMFLOAT4X4 mProj = MathHelper::Identity4x4();

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

	CreateAllMesh();
	CreateMesh();

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
	XMStoreFloat4x4(&mView, view);

		XMFLOAT4X4 CBworld = m_CBobject->GetWorldMatrix();
		XMMATRIX world = XMLoadFloat4x4(&CBworld);// objet
		XMMATRIX proj = XMLoadFloat4x4(&mProj); //cam
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
				// Draw mesh

	D3D12_VERTEX_BUFFER_VIEW vertexBuffer = m_testMesh->GetGeometry()->VertexBufferView();
		m_CommandList->IASetVertexBuffers(0, 1, &vertexBuffer);
		D3D12_INDEX_BUFFER_VIEW indexBuffer = m_testMesh->GetGeometry()->IndexBufferView();
		m_CommandList->IASetIndexBuffer(&indexBuffer);
		m_CommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// mCommandList->SetGraphicsRootDescriptorTable(0, mCbvHeap->GetGPUDescriptorHandleForHeapStart());
		m_CommandList->SetGraphicsRootConstantBufferView(0,m_CBobject->GetResource()->GetGPUVirtualAddress());
	m_CommandList->DrawIndexedInstanced(
		m_testMesh->GetGeometry()->DrawArgs["box"].IndexCount,
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
	m_Box->CreateAllMesh(m_Device.Get(), m_CommandList.Get());
	
}

inline void InitDirect3DApp::CreateMesh()
{
	m_testMesh = new Mesh;
	m_testMesh->InitPyramidMesh(m_Box);

	m_CBobject = new ConstantBuffer(m_Device.Get());
}

