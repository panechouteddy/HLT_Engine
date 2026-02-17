#include "pch.h"
#include "RenderManager.h"


void RenderManager::UpdateConstantBuffer(const std::vector<ObjectConstant>& objects)
{
    for (int i = 0; i < objects.size();i++)
    {
        if (m_ConstantBufferList[i] == nullptr)
            AddConstantBuffer();

        m_ConstantBufferList[i]->SetWorldMatrix()
    }
}

void RenderManager::UpdateView(hlt_Camera* camera)
{
	m_MeshToDrawList.erase(std::remove(m_MeshToDrawList.begin(), m_MeshToDrawList.end(), nullptr), m_MeshToDrawList.end());

    float Theta = 1.5f * XM_PI;
    float Phi = XM_PIDIV4;
    float Radius = 5.0f;

    // Convert Spherical to Cartesian coordinates.
    float x = Radius * sinf(Phi) * cosf(Theta);
    float z = Radius * sinf(Phi) * sinf(Theta);
    float y = Radius * cosf(Phi) + 2;

    XMVECTOR pos = XMVectorSet(x, y, z, 2.0f);
    XMVECTOR target = XMVectorZero();
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); //cam

    XMMATRIX view = XMMatrixLookAtLH(pos, target, up);

    XMFLOAT4X4 camView = camera->m_View;
    XMStoreFloat4x4(&camView, view);
    float fovY = DirectX::XM_PIDIV4; float nearPlane = 0.01f; float farPlane = 100.f;
    
    DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(fovY,D3DApp::GetApp()->GetWindowRatio(), nearPlane, farPlane);
    DirectX::XMStoreFloat4x4(&camView, proj);

    for (int i  = 0 ; i< m_MeshToDrawList.size() ; i ++)
    {
        XMFLOAT4X4 CBworld = m_ConstantBufferList[i]->GetWorldMatrix();
        XMMATRIX world = XMLoadFloat4x4(&CBworld);// objet
        XMMATRIX worldViewProj = world * view * proj;

        // Update the constant buffer with the latest worldViewProj matrix.
        ObjectConstant objConstants;
        XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(worldViewProj));
        m_ConstantBufferList[i]->GetBuffer()->CopyData(0, objConstants);
    }
}

void RenderManager::Draw()
{

}

void RenderManager::AddConstantBuffer()
{
    ConstantBuffer* cb = D3DApp::GetApp()->CreateConstantBufferObject();
    m_ConstantBufferList.push_back(cb);
}
