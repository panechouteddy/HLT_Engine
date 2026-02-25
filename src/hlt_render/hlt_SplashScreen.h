//#pragma once
//class hlt_SplashScreen : public hlt_D2DResource
//{
//public:
//	hlt_SplashScreen() = default;
//
//	void Initialize(ID3D11On12Device* d11On12,
//		ID2D1DeviceContext2* d2dCtx,
//		ID3D11DeviceContext* d11Ctx,
//		int swapChainBC,
//		ComPtr<ID3D12Resource>* swapChainBuffer,
//		ComPtr<ID3D11Resource>* wrappedBackBuffers) override;
//	
//	void Draw();
//	void DrawButton(D2D1_RECT_F rect, std::wstring label, bool isHovered);
//
//public:
//	bool m_Start = false;
//
//	ComPtr<IDWriteTextFormat> m_textFormatBody;
//	ComPtr<ID2D1SolidColorBrush> m_textBrush;
//};
//
