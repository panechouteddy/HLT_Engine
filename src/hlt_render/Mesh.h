#pragma once

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

class Mesh
{
	std::vector<Vertex> m_VertexList;
	std::vector<uint16_t> m_IndexList;
public:

    void CreatePyramid();
	void AddVertex(Vertex VertexAdded);
	void AddIndex(std::vector<uint16_t> IndexAdded);
	std::vector<Vertex> GetVertex();
	std::vector<uint16_t> GetIndex();
};


