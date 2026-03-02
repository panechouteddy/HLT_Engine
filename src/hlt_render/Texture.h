#pragma once
struct Texture
{
    // Unique material name for lookup.
    std::string Name;

    std::wstring Filename;

    Microsoft::WRL::ComPtr<ID3D12Resource> Resource = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource> UploadHeap = nullptr;

    int SrvHeapIndex = 0;
};

struct TextureBox
{
    // Unique material name for lookup.
protected:
    std::unordered_map<std::string, Texture*> m_TextureBox;
    int m_CurrentSrvIndex = 2;
public:
    TextureBox() {};
    bool IsAllreadyCreated(std::string texture) { return m_TextureBox.contains(texture); }
    Texture* GetTexture(std::string texture) { return m_TextureBox.contains(texture) ? m_TextureBox[texture] : nullptr; }
    void CreateAllTexture(std::vector<std::pair<std::string, std::wstring>>& fileList);
    void CreateTexture(std::string name, std::wstring fileName);
    void CreateDefaultTexture();
};

