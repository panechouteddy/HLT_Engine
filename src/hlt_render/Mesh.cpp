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
	m_pMesh = D3DApp::GetApp()->GetMeshBox()->GetMesh(meshName);
	if (m_pMesh == nullptr)
		m_MeshName = "nullptr";
	else
		m_MeshName = m_pMesh->Name;
}

void Mesh::InitPyramidMesh()
{
	m_pMesh = D3DApp::GetApp()->GetMeshBox()->GetMesh("Pyramid");
	m_MeshName = "Pyramid";
}

MeshGeometry* Mesh::GetGeometry()
{
	return m_pMesh;
}



void MeshBox::CreateAllMesh(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
	CreatePyramid(device,commandList);
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

	boxGeomety->DrawArgs["Pyramid"] = submesh;

	m_BoxOfMesh.insert(std::make_pair("Pyramid", boxGeomety));
	//m_BoxMesh["pyramid"] = boxmesh;
}
