#pragma once
struct Texture
{
    std::string Name;

    std::wstring Filename;

    Microsoft::WRL::ComPtr<ID3D12Resource> Resource = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource> UploadHeap = nullptr;

    int SrvHeapIndex = 0;
};

struct TextureBox
{
protected:
    std::unordered_map<std::string, Texture*> m_TextureBox;
    int m_CurrentSrvIndex = 2;
    ID3D12DescriptorHeap* m_pSrvDescriptorHeap = nullptr;
public:
    TextureBox() {};
    bool IsAllreadyCreated(std::string texture) { return m_TextureBox.contains(texture); }
    Texture* GetTexture(std::string texture) { return m_TextureBox.contains(texture) ? m_TextureBox[texture] : nullptr; }
    void LoadAllTexture();
    std::unordered_map<std::string, Texture*>& GetAllTexture() { return m_TextureBox; }
    void CreateTexture(std::string name, std::wstring fileName);
};

