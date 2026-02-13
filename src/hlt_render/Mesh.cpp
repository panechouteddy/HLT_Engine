#include "pch.h"
#include "Mesh.h"
#include <unordered_map>

#include <DirectXColors.h>

#include "D3DApp.h"
#include "d3dUtil.h"
using namespace DirectX;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

void Mesh::InitPyramidMesh(MeshBox* manager)
{
	m_Mesh = manager->GetMesh("Pyramid");
}

MeshGeometry* Mesh::GetGeometry()
{
	return m_Mesh;
}



void MeshBox::CreateAllMesh(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
	CreatePyramid(device,commandList);
}




void MeshBox::CreatePyramid(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
	std::array<Vertex, 5> vertices =
	{
		Vertex({ XMFLOAT3(-1.0f, 0.0f, -1.0f), XMFLOAT4(Colors::White) }),//A
		Vertex({ XMFLOAT3(1.0f, 0.0f, -1.0f), XMFLOAT4(Colors::Black) }), //B
		Vertex({ XMFLOAT3(0.0f, +2.0f, 0.0f), XMFLOAT4(Colors::Red) }),//C
		Vertex({ XMFLOAT3(-1.0f, 0.0f, +1.0f), XMFLOAT4(Colors::Green) }),//D
		Vertex({ XMFLOAT3(+1.0f, 0.0f, +1.0f), XMFLOAT4(Colors::Blue) }),//E

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
	boxGeomety->Name = "Pyramid";
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

	m_BoxOfMesh.insert(std::make_pair("Pyramid", boxGeomety));
	//m_BoxMesh["pyramid"] = boxmesh;
}
