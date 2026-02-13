#pragma once
class hlt_D2DResource
{
private:
	Windows::Foundation::Size m_logicalSize;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext2> m_D2DContext;

public:
	Windows::Foundation::Size GetLogicalSize() const { return m_logicalSize; }

	ID2D1DeviceContext2* GetD2DDeviceContext() const { return m_D2DContext.Get(); }
};

