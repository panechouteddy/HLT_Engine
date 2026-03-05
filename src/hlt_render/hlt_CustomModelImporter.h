#pragma once
#include "D3DApp.h"

struct face
{
    int vertexID[3];
    int uvID[3];
    int normalID[3];
};

namespace hlt_ModelImporter
{
	static void ImportOBJ(const std::string& path)
    {
        std::ifstream file(path);
        std::string line;

        std::vector<XMFLOAT3> vertexPos;
        std::vector<XMFLOAT2> uv;
        std::vector<XMFLOAT3> normal;
        std::vector<face> faces;

        while (std::getline(file, line))
        {
            std::istringstream readLine(line);
            std::string head;
            readLine >> head;

            if (head == "v")
            {
                XMFLOAT3 vPos;
                readLine >> vPos.x >> vPos.y >> vPos.z;
                vertexPos.push_back(vPos);
            }
            else if (head == "vt")
            {
                XMFLOAT2 vUV;
                readLine >> vUV.x >> vUV.y;
                uv.push_back(vUV);
            }
            else if (head == "vn")
            {
                XMFLOAT3 vNormal;
                readLine >> vNormal.x >> vNormal.y >> vNormal.z;
                normal.push_back(vNormal);
            }

            else if (head == "f")
            {
                std::string vertexData;
                face currentFace{};
                int vertexIndex = 0;
                while (readLine >> vertexData) {
                    // On remplace les '/' par des espaces pour lire facilement avec un stringstream
                    for (char& c : vertexData) if (c == '/') c = ' ';

                    std::stringstream vss(vertexData);
                    int vertexID = -1, uvID = -1, normalID = -1;
                    vss >> vertexID >> uvID >> normalID;
                    if (vertexID >= 0)
                    {
                        currentFace.vertexID[vertexIndex] = static_cast<uint16_t>(vertexID);
                        if (uvID >= 0)
                        {
                            currentFace.uvID[vertexIndex] = static_cast<uint16_t>(uvID);
                            if (normalID >= 0)
                            {
                                currentFace.normalID[vertexIndex] = static_cast<uint16_t>(normalID);
                                vertexIndex++;
                            }
                        }
                    }
                }
                faces.push_back(currentFace);
            }
            
        }
        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;

        for (int i = 0; i < faces.size(); i++)
        {
            for (int j = 0; j < 3; j++)
            {
                int posID = faces[i].vertexID[j], uvID = faces[i].uvID[j], normalID = faces[i].normalID[j];

                Vertex vertex;
                vertex.Pos = vertexPos[posID - 1];
                vertex.TexC = uv[uvID - 1];
                //vertex.normal = normal[normalID - 1];

                vertices.push_back(vertex);
                indices.push_back(vertices.size() - 1);
            }
            /*face faceID = faces[i];

            Vertex vertex;
            vertex.Pos = vertexPos[faceID.vertexID[0] - 1];
            vertex.TexC = uv[faceID.vertexID[0] - 1];
            vertices.push_back(vertex);
            indices.push_back(vertices.size() - 1);
            vertex;
            vertex.Pos = vertexPos[faceID.vertexID[2] - 1];
            vertex.TexC = uv[faceID.vertexID[2] - 1];
            vertices.push_back(vertex);
            indices.push_back(vertices.size() - 1);
            vertex;
            vertex.Pos = vertexPos[faceID.vertexID[1] - 1];
            vertex.TexC = uv[faceID.vertexID[1] - 1];
            vertices.push_back(vertex);
            indices.push_back(vertices.size() - 1);*/
        }

        // Utiliser GetApp() (avec parenthèses) pour récupérer le pointeur vers l'application.
        // Corrige l'erreur E0153 (expression doit avoir un type classe mais elle a le type "D3DApp *")
        if (D3DApp::GetApp() != nullptr)
        {
            D3DApp::GetApp()->CreateOriginalMesh("path", vertices, indices);
        }

        return;
    }
}