#include "pch.h"

hlt_UI::hlt_UI()
{
	m_pTexture = nullptr;
	m_Pos = { 0.0f,0.0f,0.0f };
	m_Scale = { 0.0f,0.0f };
	m_Center = { 0.0f,0.0f };
}

void hlt_UI::SetCenter()
{
	if (m_pTexture)
	{
		D3D12_RESOURCE_DESC desc = m_pTexture->Resource.Get()->GetDesc();
		m_Center.x = desc.Width * 0.5;
		m_Center.y = desc.Height * 0.5;
	}
}
