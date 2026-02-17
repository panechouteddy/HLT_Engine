#pragma once
class RenderManager
{
private:
    std::vector<Mesh*> m_MeshToDrawList;
    std::vector<ConstantBuffer*> m_ConstantBufferList;
public:

    void UpdateConstantBuffer(const std::vector<ObjectConstant>& objects);
    void UpdateView(hlt_Camera* Camera);
    void Draw();
    void AddMeshToDraw(Mesh* mesh) { m_MeshToDrawList.push_back(mesh); }
    void AddConstantBuffer();
};

