#pragma once
#include "D3DApp.h"

namespace hlt_ModelImporter
{
	static void ImportOBJ(const std::string& path)
    {
        std::ifstream file(path);
        std::string line;

        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;

        while (std::getline(file, line))
        {
            std::istringstream readLine(line);
            std::string head;
            readLine >> head;

            if (head == "v")
            {
                Vertex v{};
                readLine >> v.Pos.x >> v.Pos.y >> v.Pos.z;
                vertices.push_back(v);
            }
            else if (head == "f")
            {
                std::string vertexData;
                while (readLine >> vertexData) {
                    // On remplace les '/' par des espaces pour lire facilement avec un stringstream
                    for (char& c : vertexData) if (c == '/') c = ' ';

                    std::stringstream vss(vertexData);
                    int indice = -1;
                    vss >> indice;
                    if (indice >= 0)
                        indices.push_back(static_cast<uint16_t>(indice));
                }
            }
        }

        // Utiliser GetApp() (avec parenthèses) pour récupérer le pointeur vers l'application.
        // Corrige l'erreur E0153 (expression doit avoir un type classe mais elle a le type "D3DApp *")
        if (D3DApp::GetApp() != nullptr)
        {
            D3DApp::GetApp()->CreateOriginalMesh(path, vertices, indices);
        }

        return;
    }
}