#include "pch.h"
#include "Mesh.h"
#include "D3DApp.h"
#include "InitDirecX3DApp.hpp"
void Mesh::CreatePyramid()
{
	int number = m_VertexList.size();

	
		// front face
		m_IndexList.push_back(2 + number), m_IndexList.push_back(1 + number), m_IndexList.push_back(0 + number), //C B A

		// back face
		m_IndexList.push_back(2 + number), m_IndexList.push_back(3 + number), m_IndexList.push_back(4 + number),  //C D E

		// left face
		m_IndexList.push_back(2 + number), m_IndexList.push_back(0 + number), m_IndexList.push_back(3 + number), // C A D

		// right face
		m_IndexList.push_back(2 + number), m_IndexList.push_back(4 + number), m_IndexList.push_back(1 + number), //C E B

		// bottom face
		m_IndexList.push_back(0 + number), m_IndexList.push_back(1 + number), m_IndexList.push_back(3 + number),// A B D
		m_IndexList.push_back(1 + number), m_IndexList.push_back(4 + number), m_IndexList.push_back(3 + number),// B  E D


	// Point du Triangle
	AddVertex(Vertex({ XMFLOAT3(-1.0f, 0.0f, -1.0f), XMFLOAT4(Colors::White) }));//A
	AddVertex(Vertex({ XMFLOAT3(1.0f, 0.0f, -1.0f), XMFLOAT4(Colors::Black) })); //B
	AddVertex(Vertex({ XMFLOAT3(0.0f, +2.0f, 0.0f), XMFLOAT4(Colors::Red) }));//C
	AddVertex(Vertex({ XMFLOAT3(-1.0f, 0.0f, +1.0f), XMFLOAT4(Colors::Green) }));//D
	AddVertex(Vertex({ XMFLOAT3(+1.0f, 0.0f, +1.0f), XMFLOAT4(Colors::Blue) }));//E
	
}

void Mesh::AddVertex(Vertex VertexAdded)
{
	m_VertexList.push_back(VertexAdded);
}

void Mesh::AddIndex(std::vector<uint16_t> IndexAdded)
{

}


std::vector<Vertex> Mesh::GetVertex()
{
	return m_VertexList;
}

std::vector<uint16_t> Mesh::GetIndex()
{
	return m_IndexList;
}
