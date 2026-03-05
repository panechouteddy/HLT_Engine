#include "pch.h"
#include "Texture.h"


void TextureBox::LoadAllTexture()
{

    for (const auto& entry : std::filesystem::directory_iterator(L"..\\..\\res\\Textures"))
    {
        if (!entry.is_regular_file())
            continue;


        std::filesystem::path path = entry.path();   
        std::string name = path.stem().string();     

        std::string ext = path.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        if (ext != ".dds")
            continue;
        CreateTexture(name, path.wstring());

    }
}

void TextureBox::CreateTexture(std::string name, std::wstring fileName)
{
	
	Texture* texture = new Texture;
	texture->Name = name;
	texture->Filename = fileName;

	ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(D3DApp::GetApp()->GetDevice(),D3DApp::GetApp()->GetCommandList(), texture->Filename.c_str(), texture->Resource, texture->UploadHeap));

    m_TextureBox.insert(std::make_pair(texture->Name, texture));
}

