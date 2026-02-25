#include "pch.h"

void hlt_UI::Initialize(ID3D11On12Device* d11On12,
	ID2D1DeviceContext2* d2dCtx,
	ID3D11DeviceContext* d11Ctx,
	int swapChainBC,
	ComPtr<ID3D12Resource>* swapChainBuffer,
	ComPtr<ID3D11Resource>* wrappedBackBuffers)
{
	m_d3d11On12Device = d11On12;
	m_d2dContext = d2dCtx;
	m_d3d11DeviceContext = d11Ctx;

	D3D11_RESOURCE_FLAGS d3d11Flags = { D3D11_BIND_RENDER_TARGET };
	for (UINT i = 0; i < (UINT)swapChainBC; i++)
	{
		if (wrappedBackBuffers[i] == nullptr) {
			ThrowIfFailed(m_d3d11On12Device->CreateWrappedResource(
				swapChainBuffer[i].Get(),
				&d3d11Flags,
				D3D12_RESOURCE_STATE_PRESENT,
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				IID_PPV_ARGS(&wrappedBackBuffers[i])
			));
		}
	}

	ComPtr<IDWriteFactory> writeFactory;
	ThrowIfFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &writeFactory));

	ThrowIfFailed(writeFactory->CreateTextFormat(
		L"Consolas", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		20.0f, L"en-us", &m_textFormatBody
	));

	ThrowIfFailed(m_d2dContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_textBrush));
}

void hlt_UI::Draw(float WindowWidthMiddle, std::wstring stats)
{
	D2D1_RECT_F layoutRect = D2D1::RectF(50.0f, 50.0f, 300.0f, 300.0f);
	
	m_d2dContext->DrawText(
		stats.c_str(),
		(UINT32)stats.length(),
		m_textFormatBody.Get(),
		layoutRect,
		m_textBrush.Get()
	);
}
