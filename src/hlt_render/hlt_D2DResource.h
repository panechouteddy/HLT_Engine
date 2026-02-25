#pragma once


class hlt_D2DResource
{
private:
	ABI::Windows::Foundation::Size m_logicalSize;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext2> m_D2DContext;

public:
	virtual ~hlt_D2DResource() = default;

	ABI::Windows::Foundation::Size GetLogicalSize() const { return m_logicalSize; }

	ID2D1DeviceContext2* GetD2DDeviceContext() const { return m_D2DContext.Get(); }
	void StartDraw(int m_CurrBackBuffer, ComPtr<ID3D11Resource>* wrappedBackBuffers);
	void EndDraw(int m_CurrBackBuffer, ComPtr<ID3D11Resource>* wrappedBackBuffers);
	void ReleaseResources(int swapChainBC, ComPtr<ID3D11Resource>* wrappedBackBuffers);

	virtual void Initialize(ID3D11On12Device* d11On12,
		ID2D1DeviceContext2* d2dCtx,
		ID3D11DeviceContext* d11Ctx,
		int swapChainBC,
		ComPtr<ID3D12Resource>* swapChainBuffer,
		ComPtr<ID3D11Resource>* wrappedBackBuffers) = 0;

public:
	UINT d3d11DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	ID3D11On12Device* m_d3d11On12Device = nullptr;
	ID2D1DeviceContext2* m_d2dContext = nullptr;
	ID3D11DeviceContext* m_d3d11DeviceContext = nullptr;
};

