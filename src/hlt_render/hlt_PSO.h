#pragma once

using Microsoft::WRL::ComPtr;

struct hlt_PSO
{
public:
	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_OpaquePsoDesc;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_TransparentPsoDesc;
	D3D12_RENDER_TARGET_BLEND_DESC m_TransparencyBlendDesc; 
	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_AlphaTestedPsoDesc;

	std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayout;
	ComPtr<ID3D12RootSignature> m_RootSignature;
	std::unordered_map<std::string, ComPtr<ID3DBlob>> m_Shaders;
	std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> m_PSOs;
	ComPtr<ID3D12PipelineState> mOpaquePSO = nullptr;

	void CreateOpaquePsoDesc(DXGI_FORMAT m_BackBufferFormat, bool m_4xMsaaState, UINT m_4xMsaaQuality, DXGI_FORMAT m_DepthStencilFormat, ComPtr<ID3D12Device> m_Device);
	void CreateTransparentPsoDesc(ComPtr<ID3D12Device> m_Device);
	void CreateAlphaTestedPsoDesc(ComPtr<ID3D12Device> m_Device);
};

