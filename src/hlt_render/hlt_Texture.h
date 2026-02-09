#pragma once
class hlt_Texture
{
public:
	//
	////installer DirectXTK12 pour la class DDSTextureLoader, pour la fonction CreateDDSTextureFromFile12;////
	//

	std::string name;
	std::wstring file;
	
	Microsoft::WRL::ComPtr<ID3D12Resource> Resource = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadHeap = nullptr;

	hlt_Texture();
};

