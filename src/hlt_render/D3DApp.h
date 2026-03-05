#pragma once

//#if defined(DEBUG) || defined(_DEBUG)
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
//#include <d3d11on12.h>
//#include <d2d1_3.h>
//#include <windows.foundation.h>
//#include <dwrite.h>
//#endif

// Link necessary d3d12 libraries.
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

class ConstantBuffer;
class Mesh;
struct TextureBox;
class hlt_Camera;
class RenderManager;
class hlt_Window;
class hlt_UI;
class hlt_SplashScreen;

class ID3D11On12Device;
class ID2D1DeviceContext2;
class ID3D11Device;
class ID3D11DeviceContext;
class ID3D11Resource;

class D3DApp
{
public:
	D3DApp(hlt_Window* window);
	D3DApp(const D3DApp& rhs) = delete;
	D3DApp& operator=(const D3DApp& rhs) = delete;
	virtual ~D3DApp();

	static D3DApp* GetApp();

	HINSTANCE AppInst()const;
	HWND      MainWnd()const;
	float     AspectRatio()const;

	bool Get4xMsaaState()const;
	void Set4xMsaaState(bool value);
	void SetSize(XMINT2 newSize) { m_WindowSize = newSize; }

	void SetLoading(bool newStatus) { m_IsLoading = newStatus; }

	virtual void OnResize();

	virtual void DrawRender(std::vector<Mesh*>& meshs);
	virtual void Draw3D(std::vector<Mesh*>& meshs);
	virtual void StartDraw3D();
	virtual void EndDraw3D();
	virtual void Draw2D();
	virtual void Update(std::vector<Mesh*>& meshs, std::vector<hlt_Transform3D*>& transforms);
	ID3D12GraphicsCommandList* GetCommandList() { return m_CommandList.Get();}
	ID3D12Device* GetDevice() { return m_Device.Get(); }
	virtual bool Initialize();
	//virtual LRESULT MsgProc(HWND& hwnd, UINT& msg, WPARAM& wParam, LPARAM& lParam);
	ConstantBuffer* CreateConstantBufferObject()const;
	ColorBuffer* CreateColorBufferObject()const;
	float GetWindowRatio()const;
	//float GetWindowWidth()const;
	//float GetWindowHeight()const;
	void CreateOriginalMesh(std::string name, std::vector<Vertex>& vertexList, std::vector<uint16_t>& indexList);
	MeshBox* GetMeshBox() const;
	TextureBox* GetTextureBox() const;
	hlt_Camera* GetCamera() { return m_Camera; }
	RenderManager* GetRenderManager() { return m_RenderManager; }

	void AddMap(Map_Mesh* map);

	void AddTextToDraw(std::wstring text, XMFLOAT2 position) { ; } //{ m_TextToDraw.push_back(std::pair<std::wstring, XMFLOAT2>{text, position}); }
	void AddTextToDraw(std::wstring text, float x, float y) { ; } //{ m_TextToDraw.push_back(std::pair<std::wstring, XMFLOAT2>{text, XMFLOAT2{x,y}});}

	void OpenCommandList();
	void CloseCommandList();

	std::wstring m_TextToDraw;
	std::wstring m_TextLife;

protected:
	virtual void CreateRtvAndDsvDescriptorHeaps();
	
	// Convenience overrides for handling mouse input.
	virtual void OnMouseDown(WPARAM btnState, int x, int y) {}
	virtual void OnMouseUp(WPARAM btnState, int x, int y) {}
	virtual void OnMouseMove(WPARAM btnState, int x, int y) {}

protected:

	bool InitDirect3D();
	void InitDirect3DDraw();
	bool InitD3D11On12();
	void CreateCommandObjects();
	void CreateSwapChain();

	void FlushCommandQueue();

	ID3D12Resource* CurrentBackBuffer()const;
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;

	void CalculateFrameStats();

	void LogAdapters();
	void LogAdapterOutputs(IDXGIAdapter* adapter);
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);
	void CreateMeshBox();
	
	void ScreenSplash();
	
protected:
	static D3DApp* m_App;

	hlt_Window* m_pWindow = nullptr;
	bool m_IsLoading = true;
	bool m_IsOpacity = true;
	bool m_4xMsaaState = false;
	UINT m_4xMsaaQuality = 0;

	//GameTimer m_Timer;

	Microsoft::WRL::ComPtr<IDXGIFactory4> m_DxgiFactory;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
	Microsoft::WRL::ComPtr<ID3D12Device> m_Device;

	//Fence 
	Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence;
	UINT64 m_currentFence = 0;


	//CommandQueue & list
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CommandQueue;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_DirectCmdListAlloc;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandList;

	//Swapchain
	static const int SwapChainBufferCount = 2;
	int m_CurrBackBuffer = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_SwapChainBuffer[SwapChainBufferCount];
	Microsoft::WRL::ComPtr<ID3D12Resource> m_DepthStencilBuffer;

	// DescriptorHead
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_RtvHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DsvHeap;


	UINT m_RtvDescriptorSize = 0;
	UINT m_DsvDescriptorSize = 0;
	UINT m_CbvSrvDescriptorSize = 0;

	D3D12_VIEWPORT m_ScreenViewport ;
	D3D12_RECT m_ScissorRect;

	//Mesh
	

	//Camera
	hlt_Camera* m_Camera;

	// WINDOW DIMENSIONS
	XMINT2 m_WindowSize;

	//Draw
	RenderManager* m_RenderManager;
	MeshBox* m_MeshBox;
	TextureBox* m_TextureBox;
	 
	//Ui
	std::vector<hlt_D2DResource*> m_pUI;
	hlt_SplashScreen* m_pSplashScreen = nullptr;

	const WCHAR* m_FontFamily = L"Consolas";
	const WCHAR* m_LocalName = L"en-us";

	hlt_UI* m_UI;
	hlt_SplashScreen* m_SplashScreen;

	//4XMAA
	DXGI_FORMAT m_BackBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
	std::wstring m_MainWndCaption = L"d3d App";
	D3D_DRIVER_TYPE m_d3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
	DXGI_FORMAT m_DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//D3D11
	ComPtr<ID3D11Device> m_d3d11Device;
	ComPtr<ID3D11On12Device> m_d3d11On12Device;
	ComPtr<ID2D1DeviceContext2> m_d2dContext;
	ComPtr<ID3D11DeviceContext> m_d3d11DeviceContext;
	ComPtr<ID3D11Resource> m_wrappedBackBuffers[SwapChainBufferCount];
};

