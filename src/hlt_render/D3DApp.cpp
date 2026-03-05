#include "pch.h"

#include <WindowsX.h>

using Microsoft::WRL::ComPtr;
using namespace std;
using namespace DirectX;

D3DApp* D3DApp::m_App = nullptr;

D3DApp::D3DApp(hlt_Window* window)
    : m_pWindow(window)
{
    // Only one D3DApp can be constructed.
    assert(m_App == nullptr);
    m_App = this;
}

D3DApp::~D3DApp()
{
    if (m_Device != nullptr)
        FlushCommandQueue();
}


D3DApp* D3DApp::GetApp()
{
    return m_App;
}

HINSTANCE D3DApp::AppInst() const
{
    return m_pWindow->GetWndInstance();
}

HWND D3DApp::MainWnd() const
{
    return m_pWindow->GetWnd();
}

float D3DApp::AspectRatio() const
{
    XMINT2 clientSize = m_pWindow->GetWndSize();
    return static_cast<float>(clientSize.x) / clientSize.y;
}

bool D3DApp::Get4xMsaaState() const
{
    return m_4xMsaaState;
}

void D3DApp::Set4xMsaaState(bool value)
{
    if (m_4xMsaaState != value)
    {
        m_4xMsaaState = value;
        CreateSwapChain();
    }
}
bool D3DApp::Initialize()
{

    if (!InitDirect3D())
        return false;

    if (!InitD3D11On12())
        return false;

    hlt_D2DResource::m_d2dContext = m_d2dContext.Get();
    hlt_D2DResource::m_d3d11DeviceContext = m_d3d11DeviceContext.Get();
    hlt_D2DResource::m_d3d11On12Device = m_d3d11On12Device.Get();

    //m_pSplashScreen = new hlt_SplashScreen(SwapChainBufferCount, m_SwapChainBuffer, m_wrappedBackBuffers);
    //m_pSplashScreen->Initialize((WCHAR*)L"Consolas", 20.f, (WCHAR*)L"en-us", D2D1::ColorF(D2D1::ColorF::Black));

    m_TextureBox = new TextureBox;
    // Do the initial resize code.
    OnResize();

    m_Camera = new hlt_Camera;

    InitDirect3DDraw();

    m_IsLoading = false;

    return true;
}

void D3DApp::Update(std::vector<Mesh*>& meshs, std::vector<hlt_Transform3D*>& transforms)
{
    if (m_IsLoading)
        m_pSplashScreen->Update();

    m_Camera->Update();
    m_RenderManager->UpdateRender(m_Camera, meshs, transforms);
}

void D3DApp::Draw3D(std::vector<Mesh*>& meshs)
{
    StartDraw3D();

    if (!m_IsLoading)
    {
        m_RenderManager->Draw(meshs);
    }
    EndDraw3D();
}

void D3DApp::StartDraw3D()
{
    ThrowIfFailed(m_DirectCmdListAlloc->Reset());

    ThrowIfFailed(m_CommandList->Reset(m_DirectCmdListAlloc.Get(), nullptr));

    FlushCommandQueue();

    auto barrierToRT = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    m_CommandList->ResourceBarrier(1, &barrierToRT);

    m_CommandList->RSSetViewports(1, &m_ScreenViewport);
    m_CommandList->RSSetScissorRects(1, &m_ScissorRect);

    D3D12_CPU_DESCRIPTOR_HANDLE currentBackBufferView = CurrentBackBufferView();
    m_CommandList->ClearRenderTargetView(currentBackBufferView, Colors::LightSteelBlue, 0, nullptr);

    D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = DepthStencilView();
    m_CommandList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
    // Specify the buffers we are going to render to.
    m_CommandList->OMSetRenderTargets(1, &currentBackBufferView, true, &depthStencilView);
}

void D3DApp::EndDraw3D()
{

    auto barrierToPresent = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    m_CommandList->ResourceBarrier(1, &barrierToPresent);

    // Done recording commands.
    ThrowIfFailed(m_CommandList->Close());

    // Add the command list to the queue for execution.
    ID3D12CommandList* cmdsLists[] = { m_CommandList.Get() };
    m_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    FlushCommandQueue();
}

void D3DApp::StartDraw2D()
{
    //hlt_D2DResource::StartDraw(m_CurrBackBuffer, m_wrappedBackBuffers);
}

void D3DApp::Draw2D()
{
    /*if (m_IsLoading)
    {
        m_pSplashScreen->Draw();
    }
    else
    {
        for (hlt_D2DResource* resource : m_pUI)
            resource->Draw();
    }*/

    /*if ( m_SplashScreen->m_Opacity > 0)
        m_SplashScreen->m_Opacity -= 0.01f;
    else if (m_IsOpacity && m_SplashScreen->m_Opacity <= 0)
        m_IsOpacity = false;

        m_UI->StartDraw(m_CurrBackBuffer, m_wrappedBackBuffers);
        for (int i = 0; i < m_TextToDraw.size(); i++)
        {
            m_UI->Draw(m_TextToDraw[i]);
        }
        m_UI->EndDraw(m_CurrBackBuffer, m_wrappedBackBuffers);

        m_TextToDraw.clear();

    if (m_IsOpacity)
    {
        m_SplashScreen->StartDraw(m_CurrBackBuffer, m_wrappedBackBuffers);

        m_SplashScreen->Draw(m_pWindow->GetWndSize().x * 0.5f, m_pWindow->GetWndSize().y * 0.5f);

        m_SplashScreen->EndDraw(m_CurrBackBuffer, m_wrappedBackBuffers);
    }*/


    ThrowIfFailed(m_SwapChain->Present(0, 0));
    m_CurrBackBuffer = (m_CurrBackBuffer + 1) % SwapChainBufferCount;

    FlushCommandQueue();
}

void D3DApp::EndDraw2D()
{
    hlt_D2DResource::EndDraw(m_CurrBackBuffer, m_wrappedBackBuffers);
}

hlt_D2DResource* D3DApp::CreateResource2D()
{
    hlt_D2DResource* new2D = new hlt_D2DResource(SwapChainBufferCount, m_SwapChainBuffer, m_wrappedBackBuffers);

    m_pUI.push_back(new2D);

    return new2D;
}

void D3DApp::OnResize()
{
    for (hlt_D2DResource* ui : m_pUI)
        ui->ReleaseResources(SwapChainBufferCount, m_wrappedBackBuffers);

    //m_pSplashScreen->ReleaseResources(SwapChainBufferCount, m_wrappedBackBuffers);

    assert(m_Device);
    assert(m_SwapChain);
    assert(m_DirectCmdListAlloc);

    FlushCommandQueue();
    
    ThrowIfFailed(m_CommandList->Reset(m_DirectCmdListAlloc.Get(), nullptr));

    if (m_d2dContext) m_d2dContext->SetTarget(nullptr);

    for (int i = 0; i < SwapChainBufferCount; ++i) {
        m_wrappedBackBuffers[i].Reset();
        m_SwapChainBuffer[i].Reset();
    }

    m_DepthStencilBuffer.Reset();

    XMINT2 clientSize = m_pWindow->GetWndSize();
    ThrowIfFailed(m_SwapChain->ResizeBuffers(SwapChainBufferCount, clientSize.x, clientSize.y, m_BackBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

    m_CurrBackBuffer = 0;

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_RtvHeap->GetCPUDescriptorHandleForHeapStart());
    for (UINT i = 0; i < SwapChainBufferCount; i++)
    {
        ThrowIfFailed(m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_SwapChainBuffer[i])));
        m_Device->CreateRenderTargetView(m_SwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
        rtvHeapHandle.Offset(1, m_RtvDescriptorSize);
    }

    D3D12_RESOURCE_DESC depthStencilDesc;
    depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthStencilDesc.Alignment = 0;
    depthStencilDesc.Width = clientSize.x;
    depthStencilDesc.Height = clientSize.y;
    depthStencilDesc.DepthOrArraySize = 1;
    depthStencilDesc.MipLevels = 1;


    depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

    depthStencilDesc.SampleDesc.Count = m_4xMsaaState ? 4 : 1;
    depthStencilDesc.SampleDesc.Quality = m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;
    depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE optClear;
    optClear.Format = m_DepthStencilFormat;
    optClear.DepthStencil.Depth = 1.0f;
    optClear.DepthStencil.Stencil = 0;

    CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_DEFAULT);
    ThrowIfFailed(m_Device->CreateCommittedResource(&heapProperties,D3D12_HEAP_FLAG_NONE,&depthStencilDesc,D3D12_RESOURCE_STATE_COMMON,&optClear,IID_PPV_ARGS(m_DepthStencilBuffer.GetAddressOf())));
   
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Format = m_DepthStencilFormat;
    dsvDesc.Texture2D.MipSlice = 0;
    m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &dsvDesc, DepthStencilView());

    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_DepthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
   
    m_CommandList->ResourceBarrier(1,&barrier);

    ThrowIfFailed(m_CommandList->Close());
    ID3D12CommandList* cmdsLists[] = { m_CommandList.Get() };
    m_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    FlushCommandQueue();

    m_ScreenViewport.TopLeftX = 0;
    m_ScreenViewport.TopLeftY = 0;
    m_ScreenViewport.Width = static_cast<float>(clientSize.x);
    m_ScreenViewport.Height = static_cast<float>(clientSize.y);
    m_ScreenViewport.MinDepth = 0.0f;
    m_ScreenViewport.MaxDepth = 1.0f;

    m_ScissorRect = { 0, 0, clientSize.x, clientSize.y };

    hlt_D2DResource::m_d2dContext = m_d2dContext.Get();
    hlt_D2DResource::m_d3d11DeviceContext = m_d3d11DeviceContext.Get();
    hlt_D2DResource::m_d3d11On12Device = m_d3d11On12Device.Get();

    //for (hlt_D2DResource* ui : m_pUI)
    //{
    //    ui->Regenerate(SwapChainBufferCount, m_SwapChainBuffer, m_wrappedBackBuffers);
    //    ui->Reinitialize();
    //}

  //  m_pSplashScreen->Regenerate(SwapChainBufferCount, m_SwapChainBuffer, m_wrappedBackBuffers);
   // m_pSplashScreen->Initialize((WCHAR*)L"Consolas", 20.f, (WCHAR*)L"en-us", D2D1::ColorF(D2D1::ColorF::Black));
}

bool D3DApp::InitDirect3D()
{
    ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_DxgiFactory)));

    HRESULT hardwareResult = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device));

    if (FAILED(hardwareResult))
    {
        ComPtr<IDXGIAdapter> pWarpAdapter;
        ThrowIfFailed(m_DxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));
        D3D12CreateDevice(pWarpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device));
    }
    ThrowIfFailed(m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));

    m_RtvDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    m_DsvDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    m_CbvSrvDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
    msQualityLevels.Format = m_BackBufferFormat;
    msQualityLevels.SampleCount = 4;
    msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
    msQualityLevels.NumQualityLevels = 0;
    ThrowIfFailed(m_Device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQualityLevels, sizeof(msQualityLevels)));
    m_4xMsaaQuality = msQualityLevels.NumQualityLevels;
    assert(m_4xMsaaQuality > 0 && "Unexpected MSAA quality level.");

#ifdef _DEBUG
    LogAdapters();
#endif

    CreateCommandObjects();
    CreateSwapChain();
    CreateRtvAndDsvDescriptorHeaps();

    return true;
}
bool D3DApp::InitD3D11On12() 
{
    ThrowIfFailed(D3D11On12CreateDevice(
        m_Device.Get(),
        D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        nullptr, 0,
        reinterpret_cast<IUnknown**>(m_CommandQueue.GetAddressOf()),
        1, 0,
        &m_d3d11Device,
        &m_d3d11DeviceContext,
        nullptr
    ));
    ThrowIfFailed(m_d3d11Device.As(&m_d3d11On12Device));

    ComPtr<IDXGIDevice> dxgiDevice;
    ThrowIfFailed(m_d3d11On12Device.As(&dxgiDevice));

    D2D1_FACTORY_OPTIONS options = {};
    ComPtr<ID2D1Factory3> d2dFactory;
    ThrowIfFailed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3), &options, &d2dFactory));

    ComPtr<ID2D1Device2> d2dDevice;
    ThrowIfFailed(d2dFactory->CreateDevice(dxgiDevice.Get(), &d2dDevice));
    ThrowIfFailed(d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_d2dContext));

    return true;
}

void D3DApp::InitDirect3DDraw()
{
    ThrowIfFailed(m_CommandList->Reset(m_DirectCmdListAlloc.Get(), nullptr));
    m_DirectCmdListAlloc->Reset();

    m_RenderManager = new RenderManager(m_CommandList.Get(),m_DirectCmdListAlloc.Get());
    m_TextureBox->LoadAllTexture();

    m_RenderManager->BuildDescriptorHeaps(m_Device.Get());
    m_RenderManager->BuildRootSignature(m_Device.Get());
    m_RenderManager->BuildShadersAndInputLayout();
    m_RenderManager->BuildPSO(m_BackBufferFormat,m_Device.Get(),m_4xMsaaState,m_4xMsaaQuality, m_DepthStencilFormat);

    CreateMeshBox();
    m_TextureBox->CreateDefaultTexture();

    ThrowIfFailed(m_CommandList->Close());
    ID3D12CommandList* cmdsLists2[] = { m_CommandList.Get()};
    m_CommandQueue->ExecuteCommandLists(_countof(cmdsLists2), cmdsLists2);

   FlushCommandQueue();
}

void D3DApp::CreateCommandObjects()
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};

    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

    ThrowIfFailed(m_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CommandQueue)));
    ThrowIfFailed(m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_DirectCmdListAlloc.GetAddressOf())));
    ThrowIfFailed(m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_DirectCmdListAlloc.Get(), nullptr, IID_PPV_ARGS(m_CommandList.GetAddressOf())));

    m_CommandList->Close();
}
void D3DApp::CreateSwapChain()
{
    XMINT2 clientSize = m_pWindow->GetWndSize();

    m_SwapChain.Reset();
    DXGI_SWAP_CHAIN_DESC sd;
    sd.BufferDesc.Width = clientSize.x;
    sd.BufferDesc.Height = clientSize.y;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Format = m_BackBufferFormat;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.SampleDesc.Count = m_4xMsaaState ? 4 : 1;
    sd.SampleDesc.Quality = m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = SwapChainBufferCount;
    sd.OutputWindow = m_pWindow->GetWnd();
    sd.Windowed = true;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    ThrowIfFailed(m_DxgiFactory->CreateSwapChain(m_CommandQueue.Get(), &sd, m_SwapChain.GetAddressOf()));
}

void D3DApp::CreateRtvAndDsvDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
    rtvHeapDesc.NumDescriptors = SwapChainBufferCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    rtvHeapDesc.NodeMask = 0;
    ThrowIfFailed(m_Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(m_RtvHeap.GetAddressOf())));

    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    dsvHeapDesc.NodeMask = 0;
    ThrowIfFailed(m_Device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(m_DsvHeap.GetAddressOf())));

}

void D3DApp::FlushCommandQueue()
{
    m_currentFence++;

    ThrowIfFailed(m_CommandQueue->Signal(m_Fence.Get(), m_currentFence));

    if (m_Fence->GetCompletedValue() < m_currentFence)
    {
        HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);

        ThrowIfFailed(m_Fence->SetEventOnCompletion(m_currentFence, eventHandle));

        if (eventHandle != 0)
        {
            WaitForSingleObject(eventHandle, INFINITE);
            CloseHandle(eventHandle);
        }
    }
}
ID3D12Resource* D3DApp::CurrentBackBuffer()const
{
    return m_SwapChainBuffer[m_CurrBackBuffer].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE D3DApp::CurrentBackBufferView()const
{

    return CD3DX12_CPU_DESCRIPTOR_HANDLE(
        m_RtvHeap->GetCPUDescriptorHandleForHeapStart(),m_CurrBackBuffer,m_RtvDescriptorSize);
}

D3D12_CPU_DESCRIPTOR_HANDLE D3DApp::DepthStencilView()const
{
    return m_DsvHeap->GetCPUDescriptorHandleForHeapStart();
}
MeshBox* D3DApp::GetMeshBox() const
{
    return m_Box;
}
TextureBox* D3DApp::GetTextureBox() const
{
    return m_TextureBox;
}
void D3DApp::CalculateFrameStats()
{

    static int frameCnt = 0;
    static float timeElapsed = 0.0f;

    frameCnt++;

    if ((hlt_Time::GetInstance().GetTotalTime() - timeElapsed) >= 1.0f)
    {

        float fps = (float)frameCnt; // fps 
        float mspf = 1000.0f / fps;


        
        wstring fpsStr = to_wstring(fps);
        wstring mspfStr = to_wstring(mspf);

        wstring windowText = m_MainWndCaption +
            L"    fps: " + fpsStr +
            L"   mspf: " + mspfStr;

        SetWindowText(m_pWindow->GetWnd(), windowText.c_str());

        // Reset for next average.
        frameCnt = 0;
        timeElapsed += 1.0f;
    }
}

ConstantBuffer* D3DApp::CreateConstantBufferObject() const
{
    ConstantBuffer* cb = new ConstantBuffer(m_Device.Get());

    return cb;
}

ColorBuffer* D3DApp::CreateColorBufferObject() const
{
    ColorBuffer* colorB = new ColorBuffer(m_Device.Get());

    return colorB;

}

float D3DApp::GetWindowRatio() const
{
    XMINT2 clientSize = m_pWindow->GetWndSize();
    return ((float)clientSize.x / (float)clientSize.y);
}

//float D3DApp::GetWindowWidth() const
//{
//    XMINT2 clientSize = m_pWindow->GetWndSize();
//
//    return (float)clientSize.x;
//}
//float D3DApp::GetWindowHeight() const
//{
//    XMINT2 clientSize = m_pWindow->GetWndSize();
//    return (float)clientSize.y;
//}


void D3DApp::LogAdapters()
{
    UINT i = 0;
    IDXGIAdapter* adapter = nullptr;
    std::vector<IDXGIAdapter*> adapterList;
    while (m_DxgiFactory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
    {
        DXGI_ADAPTER_DESC desc;
        adapter->GetDesc(&desc);

        std::wstring text = L"***Adapter: ";
        text += desc.Description;
        text += L"\n";

        OutputDebugString(text.c_str());

        adapterList.push_back(adapter);

        ++i;
    }

    for (size_t i = 0; i < adapterList.size(); ++i)
    {
        LogAdapterOutputs(adapterList[i]);
        ReleaseCom(adapterList[i]);
    }
}

void D3DApp::LogAdapterOutputs(IDXGIAdapter* adapter)
{
    UINT i = 0;
    IDXGIOutput* output = nullptr;
    while (adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND)
    {
        DXGI_OUTPUT_DESC desc;
        output->GetDesc(&desc);

        std::wstring text = L"***Output: ";
        text += desc.DeviceName;
        text += L"\n";
        OutputDebugString(text.c_str());

        LogOutputDisplayModes(output, m_BackBufferFormat);

        ReleaseCom(output);

        ++i;
    }
}

void D3DApp::LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format)
{

    UINT count = 0;
    UINT flags = 0;

    // Call with nullptr to get list count.
    output->GetDisplayModeList(format, flags, &count, nullptr);

    std::vector<DXGI_MODE_DESC> modeList(count);
    output->GetDisplayModeList(format, flags, &count, &modeList[0]);

    for (auto& x : modeList)
    {
        UINT n = x.RefreshRate.Numerator;
        UINT d = x.RefreshRate.Denominator;
        std::wstring text =
            L"Width = " + std::to_wstring(x.Width) + L" " +
            L"Height = " + std::to_wstring(x.Height) + L" " +
            L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) +
            L"\n";

        ::OutputDebugString(text.c_str());
    }
}

void D3DApp::CreateMeshBox()
{
    m_Box = new MeshBox;
    m_Box->CreateAllMesh(m_Device.Get(), m_CommandList.Get());
}

void D3DApp::CreateOriginalMesh(std::string name, std::vector<Vertex>& vertexList, std::vector<uint16_t>& indexList)
{
    m_Box->CreateMesh(name, vertexList, indexList);
}

void D3DApp::AddMap(Map_Mesh* map)
{
    m_RenderManager->AddMapToRender(map);

}
