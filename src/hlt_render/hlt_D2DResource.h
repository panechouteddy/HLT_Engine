#pragma once
#include <d3d11on12.h>
#include <d2d1_3.h>
#include <windows.foundation.h>
#include <dwrite.h>

class hlt_D2DResource
{
private:
	ABI::Windows::Foundation::Size m_logicalSize;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext2> m_D2DContext;

public:
	virtual ~hlt_D2DResource() = default;

	void Initialize(ID3D11On12Device* d11On12,
		ID2D1DeviceContext2* d2dCtx,
		ID3D11DeviceContext* d11Ctx,
		int swapChainBC,
		ComPtr<ID3D12Resource>* swapChainBuffer,
		ComPtr<ID3D11Resource>* wrappedBackBuffers,
		WCHAR* fontFamilyName,
		float fontSize,
		WCHAR* localName,
		D2D1_COLOR_F& fontColor);

	ABI::Windows::Foundation::Size GetLogicalSize() const { return m_logicalSize; }

	ID2D1DeviceContext2* GetD2DDeviceContext() const { return m_D2DContext.Get(); }
	void ReleaseResources(int swapChainBC, ComPtr<ID3D11Resource>* wrappedBackBuffers);

	void StartDraw(int m_CurrBackBuffer, ComPtr<ID3D11Resource>* wrappedBackBuffers);
	void EndDraw(int m_CurrBackBuffer, ComPtr<ID3D11Resource>* wrappedBackBuffers);

	void Draw();

	void SetText(std::wstring newText) { m_Text = newText; }
	void SetTextArea(D2D1_RECT_F newTextArea) { m_TextArea = newTextArea; }
	void SetColorRectangle(D2D1_RECT_F* newRect) { m_pRect = newRect; }

	D2D1_RECT_F MakeRectangle(float left, float top, float right, float bottom, D2D1::ColorF colorBrush, D2D1::ColorF colorRectEdge, float textOpacity);

public:
	UINT d3d11DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	ID3D11On12Device* m_d3d11On12Device = nullptr;
	ID2D1DeviceContext2* m_d2dContext = nullptr;
	ID3D11DeviceContext* m_d3d11DeviceContext = nullptr;

	ComPtr<IDWriteTextFormat> m_textFormatBody;
	ComPtr<ID2D1SolidColorBrush> m_textBrush;

	float m_TextOpacity = 1.f;

	XMINT2 m_Pos = { 0, 0 };
	std::wstring m_Text;
	D2D1_RECT_F m_TextArea;

	D2D1_RECT_F* m_pRect;
	ComPtr<ID2D1SolidColorBrush> m_RectColor;
};

