#include "pch.h"

void hlt_PSO::CreateOpaquePsoDesc(DXGI_FORMAT m_BackBufferFormat, bool m_4xMsaaState, UINT m_4xMsaaQuality, DXGI_FORMAT m_DepthStencilFormat,
	ComPtr<ID3D12Device> m_Device)
{
	//PSO Opaque
	ZeroMemory(&m_OpaquePsoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	m_OpaquePsoDesc.InputLayout = { m_InputLayout.data(), (UINT)m_InputLayout.size() };
	m_OpaquePsoDesc.pRootSignature = m_RootSignature.Get();
	m_OpaquePsoDesc.VS =
	{
		reinterpret_cast<BYTE*>(m_Shaders["standardVS"]->GetBufferPointer()),
		m_Shaders["standardVS"]->GetBufferSize()
	};
	m_OpaquePsoDesc.PS =
	{
		reinterpret_cast<BYTE*>(m_Shaders["opaquePS"]->GetBufferPointer()),
		m_Shaders["opaquePS"]->GetBufferSize()
	};
	m_OpaquePsoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	m_OpaquePsoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	m_OpaquePsoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	m_OpaquePsoDesc.SampleMask = UINT_MAX;
	m_OpaquePsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	m_OpaquePsoDesc.NumRenderTargets = 1;
	m_OpaquePsoDesc.RTVFormats[0] = m_BackBufferFormat;
	m_OpaquePsoDesc.SampleDesc.Count = m_4xMsaaState ? 4 : 1;
	m_OpaquePsoDesc.SampleDesc.Quality = m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;
	m_OpaquePsoDesc.DSVFormat = m_DepthStencilFormat;
	ThrowIfFailed(m_Device->CreateGraphicsPipelineState(&m_OpaquePsoDesc, IID_PPV_ARGS(&m_PSOs["opaque"])));
}

void hlt_PSO::CreateTransparentPsoDesc(ComPtr<ID3D12Device> m_Device)
{
	//PSO transparent
	m_TransparentPsoDesc = m_OpaquePsoDesc;

	m_TransparencyBlendDesc.BlendEnable = true;
	m_TransparencyBlendDesc.LogicOpEnable = false;
	m_TransparencyBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	m_TransparencyBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	m_TransparencyBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	m_TransparencyBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	m_TransparencyBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	m_TransparencyBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	m_TransparencyBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
	m_TransparencyBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	m_TransparentPsoDesc.BlendState.RenderTarget[0] = m_TransparencyBlendDesc;
	ThrowIfFailed(m_Device->CreateGraphicsPipelineState(&m_TransparentPsoDesc, IID_PPV_ARGS(&m_PSOs["transparent"])));
}

void hlt_PSO::CreateAlphaTestedPsoDesc(ComPtr<ID3D12Device> m_Device)
{
	//PSO Alpha test
	m_AlphaTestedPsoDesc = m_OpaquePsoDesc;
	m_AlphaTestedPsoDesc.PS =
	{
		reinterpret_cast<BYTE*>(m_Shaders["alphaTestedPS"]->GetBufferPointer()),
		m_Shaders["alphaTestedPS"]->GetBufferSize()
	};
	m_AlphaTestedPsoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	ThrowIfFailed(m_Device->CreateGraphicsPipelineState(&m_AlphaTestedPsoDesc, IID_PPV_ARGS(&m_PSOs["alphaTested"])));
}
