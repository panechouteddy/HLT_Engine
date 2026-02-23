#include "pch.h"
#include "Mesh.h"
#include <unordered_map>

#include <DirectXColors.h>

#include "D3DApp.h"
#include "d3dUtil.h"
#include "ColorBuffer.h"
using namespace DirectX;

struct Vertex
{
	XMFLOAT3 Pos;
};


void Mesh::SetMesh(std::string meshName)
{
	std::transform(meshName.begin(), meshName.end(), meshName.begin(), std::tolower);

	m_pMesh = D3DApp::GetApp()->GetMeshBox()->GetMesh(meshName);
	if (m_pMesh == nullptr)
		m_MeshName = "nullptr";
}

void Mesh::InitPyramidMesh()
{
	m_Mesh = D3DApp::GetApp()->GetMeshBox()->GetMesh("pyramid");
	m_MeshName = "pyramid";
}

MeshGeometry* Mesh::GetGeometry()
{
	return m_pMesh;
}



void MeshBox::CreateAllMesh(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
	CreatePyramid(device,commandList);
	CreateCube(device,commandList);
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
