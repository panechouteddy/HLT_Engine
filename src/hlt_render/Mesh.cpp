#include "pch.h"
#include "Mesh.h"
#include <unordered_map>

#include <DirectXColors.h>

#include "D3DApp.h"
#include "d3dUtil.h"
#include "ColorBuffer.h"
using namespace DirectX;




Mesh::Mesh()
{
	m_pTexture = D3DApp::GetApp()->GetTextureBox()->GetTexture("Default");
}

void Mesh::SetMesh(std::string meshName, XMFLOAT3 color)
{
	std::transform(meshName.begin(), meshName.end(), meshName.begin(), std::tolower);

	m_pMesh = D3DApp::GetApp()->GetMeshBox()->GetMesh(meshName);
	if (m_pMesh == nullptr)
		m_MeshName = "nullptr";
	else
		m_MeshName = m_pMesh->Name;
	SetColor(color);
}

void Mesh::SetTexture(std::string TextName)
{
	std::transform(TextName.begin(), TextName.end(), TextName.begin(), std::tolower);

	m_pTexture = D3DApp::GetApp()->GetTextureBox()->GetTexture(TextName);
}

MeshGeometry* Mesh::GetGeometry()
{
	return m_pMesh;
}



void MeshBox::CreateAllMesh(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
	//CreatePyramid(device,commandList);
	CreateCube(device,commandList);
	//CreateRock(device, commandList);
	//CreateGround(device, commandList);
}




void MeshBox::CreateMesh(std::string name ,std::vector<Vertex>& vertexList, std::vector<uint16_t>& indexList)
{
	if (IsAllreadyCreated(name))
		return;

	const UINT vbByteSize = (UINT)vertexList.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indexList.size() * sizeof(std::uint16_t);

	ID3D12GraphicsCommandList * commandList = D3DApp::GetApp()->GetCommandList();
	ID3D12Device * device = D3DApp::GetApp()->GetDevice();
	MeshGeometry* boxGeomety = new MeshGeometry;
	boxGeomety->Name = name;
	//ThrowIfFailed(D3DCreateBlob(vbByteSize,&boxGeomety->VertexBufferCPU));
	//CopyMemory(boxGeomety->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);
	//
	//ThrowIfFailed(D3DCreateBlob(ibByteSize,&boxGeomety->IndexBufferCPU));
	//CopyMemory(boxGeomety->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	boxGeomety->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(device, commandList, vertexList.data(), vbByteSize, boxGeomety->VertexBufferUploader);

	boxGeomety->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(device, commandList, indexList.data(), ibByteSize, boxGeomety->IndexBufferUploader);

	boxGeomety->VertexByteStride = sizeof(Vertex);
	boxGeomety->VertexBufferByteSize = vbByteSize;
	boxGeomety->IndexFormat = DXGI_FORMAT_R16_UINT;
	boxGeomety->IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)indexList.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	boxGeomety->DrawArgs[name] = submesh;

	m_BoxOfMesh.insert(std::make_pair(name, boxGeomety));
}

void MeshBox::CreatePyramid(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
	std::array<Vertex, 5> vertices =
	{
		Vertex({ XMFLOAT3(-1.0f, 0.0f, -1.0f)}),//A
		Vertex({ XMFLOAT3(1.0f, 0.0f, -1.0f)}), //B
		Vertex({ XMFLOAT3(0.0f, +2.0f, 0.0f)}),//C
		Vertex({ XMFLOAT3(-1.0f, 0.0f, +1.0f)}),//D
		Vertex({ XMFLOAT3(+1.0f, 0.0f, +1.0f)}),//E

	};
	std::array<std::uint16_t, 18> indices =
	{
		// front face
		2, 1, 0,

		// back face
		2, 3, 4,

		// left face
		2, 0, 3,

		// right face
		2, 4, 1,

		// bottom face
		0, 1, 3,
		1, 4, 3
	};
	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	MeshGeometry* boxGeomety = new MeshGeometry;
	boxGeomety->Name = "pyramid";
	//ThrowIfFailed(D3DCreateBlob(vbByteSize,&boxGeomety->VertexBufferCPU));
	//CopyMemory(boxGeomety->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);
	//
	//ThrowIfFailed(D3DCreateBlob(ibByteSize,&boxGeomety->IndexBufferCPU));
	//CopyMemory(boxGeomety->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	boxGeomety->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(device,commandList, vertices.data(), vbByteSize,boxGeomety->VertexBufferUploader);

	boxGeomety->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(device,commandList, indices.data(), ibByteSize, boxGeomety->IndexBufferUploader);

	boxGeomety->VertexByteStride = sizeof(Vertex);
	boxGeomety->VertexBufferByteSize = vbByteSize;
	boxGeomety->IndexFormat = DXGI_FORMAT_R16_UINT;
	boxGeomety->IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	boxGeomety->DrawArgs["pyramid"] = submesh;

	m_BoxOfMesh.insert(std::make_pair("pyramid", boxGeomety));
	//m_BoxMesh["pyramid"] = boxmesh;
}

void MeshBox::CreateCube(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
	std::array<Vertex, 24> vertices =
	{
		// Front face
		Vertex({ -1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f }),
		Vertex({ -1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f }),
		Vertex({  1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f }),
		Vertex({  1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f }),

		// Back face
		Vertex({ -1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f }),
		Vertex({  1.0f, -1.0f,  1.0f }, { 0.0f, 1.0f }),
		Vertex({  1.0f,  1.0f,  1.0f }, { 0.0f, 0.0f }),
		Vertex({ -1.0f,  1.0f,  1.0f }, { 1.0f, 0.0f }),

		// Top face
		Vertex({ -1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f }),
		Vertex({ -1.0f,  1.0f,  1.0f }, { 0.0f, 0.0f }),
		Vertex({  1.0f,  1.0f,  1.0f }, { 1.0f, 0.0f }),
		Vertex({  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f }),

		// Bottom face
		Vertex({ -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f }),
		Vertex({  1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f }),
		Vertex({  1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f }),
		Vertex({ -1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f }),

		// Left face
		Vertex({ -1.0f, -1.0f,  1.0f }, { 0.0f, 1.0f }),
		Vertex({ -1.0f,  1.0f,  1.0f }, { 0.0f, 0.0f }),
		Vertex({ -1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f }),
		Vertex({ -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f }),

		// Right face
		Vertex({  1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f }),
		Vertex({  1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f }),
		Vertex({  1.0f,  1.0f,  1.0f }, { 1.0f, 0.0f }),
		Vertex({  1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f }),
	};

	std::array<std::uint16_t, 36> indices =
	{
		0,1,2,  0,2,3,
		4,5,6,  4,6,7,
		8,9,10, 8,10,11,
		12,13,14, 12,14,15,
		16,17,18, 16,18,19,
		20,21,22, 20,22,23
	};
	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	MeshGeometry* boxGeomety = new MeshGeometry;
	boxGeomety->Name = "cube";
	//ThrowIfFailed(D3DCreateBlob(vbByteSize,&boxGeomety->VertexBufferCPU));
	//CopyMemory(boxGeomety->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);
	//
	//ThrowIfFailed(D3DCreateBlob(ibByteSize,&boxGeomety->IndexBufferCPU));
	//CopyMemory(boxGeomety->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	boxGeomety->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(device, commandList, vertices.data(), vbByteSize, boxGeomety->VertexBufferUploader);

	boxGeomety->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(device, commandList, indices.data(), ibByteSize, boxGeomety->IndexBufferUploader);

	boxGeomety->VertexByteStride = sizeof(Vertex);
	boxGeomety->VertexBufferByteSize = vbByteSize;
	boxGeomety->IndexFormat = DXGI_FORMAT_R16_UINT;
	boxGeomety->IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	boxGeomety->DrawArgs["cube"] = submesh;

	m_BoxOfMesh.insert(std::make_pair("cube", boxGeomety));	
}

void MeshBox::CreateRock(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
	std::array<Vertex, 9> vertices =
	{
		Vertex({ XMFLOAT3(-2.0f, -0.0f, -2.0f) }),
		Vertex({ XMFLOAT3(-2.0f,  0.0f, -1.0f) }),
		Vertex({ XMFLOAT3(+2.0f, +1.0f, -1.0f) }),
		Vertex({ XMFLOAT3(+2.0f, -1.0f, -1.0f) }),
		Vertex({ XMFLOAT3(-2.0f, -1.0f, +1.0f) }),
		Vertex({ XMFLOAT3(-2.0f, +1.0f, +1.0f) }),
		Vertex({ XMFLOAT3(+2.0f, +1.0f, +1.0f) }),
		Vertex({ XMFLOAT3(+2.0f, -1.0f, +1.0f) }),
		Vertex({ XMFLOAT3( 1.0f, +3.0f,  0.0f) })
	};

	std::array<std::uint16_t, 42> indices =
	{
		// Front 
		0,1,2,
		0,2,3,

		// Back 
		4,6,5,
		4,7,6,

		// Left 
		4,5,1,
		4,1,0,

		// Right 
		3,2,6,
		3,6,7,

		// Bottom 
		4,0,3,
		4,3,7,

		// Roof 
		1,8,2,
		2,8,6,
		6,8,5,
		5,8,1
	};
	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	MeshGeometry* boxGeomety = new MeshGeometry;
	boxGeomety->Name = "rock";
	//ThrowIfFailed(D3DCreateBlob(vbByteSize,&boxGeomety->VertexBufferCPU));
	//CopyMemory(boxGeomety->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);
	//
	//ThrowIfFailed(D3DCreateBlob(ibByteSize,&boxGeomety->IndexBufferCPU));
	//CopyMemory(boxGeomety->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	boxGeomety->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(device, commandList, vertices.data(), vbByteSize, boxGeomety->VertexBufferUploader);

	boxGeomety->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(device, commandList, indices.data(), ibByteSize, boxGeomety->IndexBufferUploader);

	boxGeomety->VertexByteStride = sizeof(Vertex);
	boxGeomety->VertexBufferByteSize = vbByteSize;
	boxGeomety->IndexFormat = DXGI_FORMAT_R16_UINT;
	boxGeomety->IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	boxGeomety->DrawArgs["rock"] = submesh;

	m_BoxOfMesh.insert(std::make_pair("rock", boxGeomety));
}

void MeshBox::CreateGround(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
	std::array<Vertex, 8> vertices =
	{
		Vertex({ XMFLOAT3(-10.0f, -2.0f, -10.0f) }),
		Vertex({ XMFLOAT3(-10.0f,  0.f , -10.0f) }),
		Vertex({ XMFLOAT3(+10.0f,  0.f , -10.0f) }),
		Vertex({ XMFLOAT3(+10.0f, -2.0f, -10.0f) }),
		Vertex({ XMFLOAT3(-10.0f, -2.0f, +10.0f) }),
		Vertex({ XMFLOAT3(-10.0f,  0.f , +10.0f) }),
		Vertex({ XMFLOAT3(+10.0f,  0.f , +10.0f) }),
		Vertex({ XMFLOAT3(+10.0f, -2.0f, +10.0f) })
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

	MeshGeometry* boxGeomety = new MeshGeometry;
	boxGeomety->Name = "ground";
	//ThrowIfFailed(D3DCreateBlob(vbByteSize,&boxGeomety->VertexBufferCPU));
	//CopyMemory(boxGeomety->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);
	//
	//ThrowIfFailed(D3DCreateBlob(ibByteSize,&boxGeomety->IndexBufferCPU));
	//CopyMemory(boxGeomety->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	boxGeomety->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(device, commandList, vertices.data(), vbByteSize, boxGeomety->VertexBufferUploader);

	boxGeomety->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(device, commandList, indices.data(), ibByteSize, boxGeomety->IndexBufferUploader);

	boxGeomety->VertexByteStride = sizeof(Vertex);
	boxGeomety->VertexBufferByteSize = vbByteSize;
	boxGeomety->IndexFormat = DXGI_FORMAT_R16_UINT;
	boxGeomety->IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	boxGeomety->DrawArgs["ground"] = submesh;

	m_BoxOfMesh.insert(std::make_pair("ground", boxGeomety));
}
