#include "pch.h"

#include <WindowsX.h>

using Microsoft::WRL::ComPtr;
using namespace std;
using namespace DirectX;

//LRESULT CALLBACK
//MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//    // Forward hwnd on because we can get messages (e.g., WM_CREATE)
//    // before CreateWindow returns, and thus before mhMainWnd is valid.
//    return D3DApp::GetApp()->MsgProc(hwnd, msg, wParam, lParam);
//}

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

int D3DApp::Run()
{
    MSG msg = { 0 };

    while (msg.message != WM_QUIT)
    {
        // If there are Window messages then process them.
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        // Otherwise, do animation/game stuff.
        else
        {
            if (!m_pWindow->IsPaused())
            {
                CalculateFrameStats();
                Update();
                Draw();
            }
            else
            {
                Sleep(100);
            }
        }
    }

    return (int)msg.wParam;
}
bool D3DApp::Initialize()
{
    //if (!InitMainWindow())
    //    return false;

    if (!InitDirect3D())
        return false;

    // Do the initial resize code.
    OnResize();

    m_Camera = new hlt_Camera;
    InitDirect3DDraw();

    return true;
}
void D3DApp::Update()
{
    m_RenderManager->UpdateConstantBuffer(m_MeshPosition);
    m_RenderManager->UpdateView(m_Camera->m_View);

}

void D3DApp::Draw()
{
    ThrowIfFailed(m_DirectCmdListAlloc->Reset());

    ThrowIfFailed(m_CommandList->Reset(m_DirectCmdListAlloc.Get(), nullptr));

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

    m_RenderManager->Draw();

    auto barrierToPresent = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    m_CommandList->ResourceBarrier(1, &barrierToPresent);

    // Done recording commands.
    ThrowIfFailed(m_CommandList->Close());

    // Add the command list to the queue for execution.
    ID3D12CommandList* cmdsLists[] = { m_CommandList.Get()};
    m_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    ThrowIfFailed(m_SwapChain->Present(0, 0));
    m_CurrBackBuffer = (m_CurrBackBuffer + 1) % SwapChainBufferCount;

    FlushCommandQueue();
}

//LRESULT D3DApp::MsgProc(HWND& hwnd, UINT& msg, WPARAM& wParam, LPARAM& lParam)
//{
//    //switch (msg)
//    //{
//    //    // WM_ACTIVATE is sent when the window is activated or deactivated.  
//    //    // We pause the game when the window is deactivated and unpause it 
//    //    // when it becomes active.  
//    ////case WM_ACTIVATE:
//    ////    if (LOWORD(wParam) == WA_INACTIVE)
//    ////    {
//    ////        m_AppPaused = true;
//    ////        m_Timer.Stop();
//    ////    }
//    ////    else
//    ////    {
//    ////        m_AppPaused = false;
//    ////        m_Timer.Start();
//    ////    }
//    ////    return 0;
//
//    //    // WM_SIZE is sent when the user resizes the window.  
//    ////case WM_SIZE:
//    ////    // Save the new client area dimensions.
//    ////    //m_ClientWidth = LOWORD(lParam);
//    ////    //m_ClientHeight = HIWORD(lParam);
//    ////    if (m_Device)
//    ////    {
//    ////        if (wParam == SIZE_MINIMIZED)
//    ////        {
//    ////            m_AppPaused = true;
//    ////            m_Minimized = true;
//    ////            m_Maximized = false;
//    ////        }
//    ////        else if (wParam == SIZE_MAXIMIZED)
//    ////        {
//    ////            m_AppPaused = false;
//    ////            m_Minimized = false;
//    ////            m_Maximized = true;
//    ////            OnResize();
//    ////        }
//    ////        else if (wParam == SIZE_RESTORED)
//    ////        {
//
//    ////            // Restoring from minimized state?
//    ////            if (m_Minimized)
//    ////            {
//    ////                m_AppPaused = false;
//    ////                m_Minimized = false;
//    ////                OnResize();
//    ////            }
//
//    ////            // Restoring from maximized state?
//    ////            else if (m_Maximized)
//    ////            {
//    ////                m_AppPaused = false;
//    ////                m_Maximized = false;
//    ////                OnResize();
//    ////            }
//    ////            else if (m_Resizing)
//    ////            {
//    ////                // If user is dragging the resize bars, we do not resize 
//    ////                // the buffers here because as the user continuously 
//    ////                // drags the resize bars, a stream of WM_SIZE messages are
//    ////                // sent to the window, and it would be pointless (and slow)
//    ////                // to resize for each WM_SIZE message received from dragging
//    ////                // the resize bars.  So instead, we reset after the user is 
//    ////                // done resizing the window and releases the resize bars, which 
//    ////                // sends a WM_EXITSIZEMOVE message.
//    ////            }
//    ////            else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
//    ////            {
//    ////                OnResize();
//    ////            }
//    ////        }
//    ////    }
//    ////    return 0;
//
//    //    // WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
//    //case WM_ENTERSIZEMOVE:
//    //    m_AppPaused = true;
//    //    m_Resizing = true;
//    //    m_Timer.Stop();
//    //    return 0;
//
//    //    // WM_EXITSIZEMOVE is sent when the user releases the resize bars.
//    //    // Here we reset everything based on the new window dimensions.
//    //case WM_EXITSIZEMOVE:
//    //    m_AppPaused = false;
//    //    m_Resizing = false;
//    //    m_Timer.Start();
//    //    OnResize();
//    //    return 0;
//
//    //    // WM_DESTROY is sent when the window is being destroyed.
//    //case WM_DESTROY:
//    //    PostQuitMessage(0);
//    //    return 0;
//
//    //    // The WM_MENUCHAR message is sent when a menu is active and the user presses 
//    //    // a key that does not correspond to any mnemonic or accelerator key. 
//    //case WM_MENUCHAR:
//    //    // Don't beep when we alt-enter.
//    //    return MAKELRESULT(0, MNC_CLOSE);
//
//    //    // Catch this message so to prevent the window from becoming too small.
//    //case WM_GETMINMAXINFO:
//    //    ((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
//    //    ((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
//    //    return 0;
//
//    //case WM_LBUTTONDOWN:
//    //case WM_MBUTTONDOWN:
//    //case WM_RBUTTONDOWN:
//    //    OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
//    //    return 0;
//    //case WM_LBUTTONUP:
//    //case WM_MBUTTONUP:
//    //case WM_RBUTTONUP:
//    //    OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
//    //    return 0;
//    //case WM_MOUSEMOVE:
//    //    OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
//    //    return 0;
//    //case WM_KEYUP:
//    //    if (wParam == VK_ESCAPE)
//    //    {
//    //        PostQuitMessage(0);
//    //    }
//    //    else if ((int)wParam == VK_F2)
//    //        Set4xMsaaState(!m_4xMsaaState);
//
//    //    return 0;
//    //}
//
//    //return DefWindowProc(hwnd, msg, wParam, lParam);
//}
void D3DApp::OnResize()
{
    assert(m_Device);
    assert(m_SwapChain);
    assert(m_DirectCmdListAlloc);

    FlushCommandQueue();

    ThrowIfFailed(m_CommandList->Reset(m_DirectCmdListAlloc.Get(), nullptr));

    for (int i = 0; i < SwapChainBufferCount; ++i)
        m_SwapChainBuffer[i].Reset();
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

    // Create descriptor to mip level 0 of entire resource using the format of the resource.
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Format = m_DepthStencilFormat;
    dsvDesc.Texture2D.MipSlice = 0;
    m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &dsvDesc, DepthStencilView());

    // Transition the resource from its initial state to be used as a depth buffer.
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_DepthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
   
    m_CommandList->ResourceBarrier(1,&barrier);

    // Execute the resize commands.
    ThrowIfFailed(m_CommandList->Close());
    ID3D12CommandList* cmdsLists[] = { m_CommandList.Get() };
    m_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    // Wait until resize is complete.
    FlushCommandQueue();

    // Update the viewport transform to cover the client area.
    m_ScreenViewport.TopLeftX = 0;
    m_ScreenViewport.TopLeftY = 0;
    m_ScreenViewport.Width = static_cast<float>(clientSize.x);
    m_ScreenViewport.Height = static_cast<float>(clientSize.y);
    m_ScreenViewport.MinDepth = 0.0f;
    m_ScreenViewport.MaxDepth = 1.0f;

    m_ScissorRect = { 0, 0, clientSize.x, clientSize.y };
}
//bool D3DApp::InitMainWindow()
//{
//
//    WNDCLASS wc;
//    wc.style = CS_HREDRAW | CS_VREDRAW;
//    wc.lpfnWndProc = MainWndProc;
//    wc.cbClsExtra = 0;
//    wc.cbWndExtra = 0;
//    wc.hInstance = m_hAppInst;
//    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
//    wc.hCursor = LoadCursor(0, IDC_ARROW);
//    wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
//    wc.lpszMenuName = 0;
//    wc.lpszClassName = L"MainWnd";
//
//    if (!RegisterClass(&wc))
//    {
//        MessageBox(0, L"RegisterClass Failed.", 0, 0);
//        return false;
//    }
//
//    // Compute window rectangle dimensions based on requested client area dimensions.
//    RECT R = { 0, 0, m_ClientWidth, m_ClientHeight };
//    AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
//    int width = R.right - R.left;
//    int height = R.bottom - R.top;
//
//    m_hMainWnd = CreateWindow(L"MainWnd", m_MainWndCaption.c_str(),
//        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, m_hAppInst, 0);
//    if (!m_hMainWnd)
//    {
//        MessageBox(0, L"CreateWindow Failed.", 0, 0);
//        return false;
//    }
//
//    ShowWindow(m_hMainWnd, SW_SHOW);
//    UpdateWindow(m_hMainWnd);
//
//    return true;
//}
bool D3DApp::InitDirect3D()
{
   // #if defined(DEBUG) || defined(_DEBUG)
    // Enable the D3D12 debug layer.
   // {
       // ComPtr<ID3D12Debug> debugController;
       // ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
       // debugController->EnableDebugLayer();
   // }
   // #endif
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

void D3DApp::InitDirect3DDraw()
{
    m_RenderManager = new RenderManager(m_CommandList.Get(),m_DirectCmdListAlloc.Get());
    m_RenderManager->BuildDescriptorHeaps(m_Device.Get());
    m_RenderManager->BuildRootSignature(m_Device.Get());
    m_RenderManager->BuildShadersAndInputLayout();
    m_RenderManager->BuildPSO(m_Device.Get(),m_4xMsaaState,m_4xMsaaQuality);

    ThrowIfFailed(m_CommandList->Reset(m_DirectCmdListAlloc.Get(), nullptr));
    m_DirectCmdListAlloc->Reset();

    CreateMeshBox();

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
    // Advance the fence value to mark commands up to this fence point.
    m_currentFence++;

    // Add an instruction to the command queue to set a new fence point.  Because we 
    // are on the GPU timeline, the new fence point won't be set until the GPU finishes
    // processing all the commands prior to this Signal().
    ThrowIfFailed(m_CommandQueue->Signal(m_Fence.Get(), m_currentFence));

    // Wait until the GPU has completed commands up to this fence point.
    if (m_Fence->GetCompletedValue() < m_currentFence)
    {
        HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);

        // Fire event when GPU hits current fence.  
        ThrowIfFailed(m_Fence->SetEventOnCompletion(m_currentFence, eventHandle));

        // Wait until the GPU hits current fence event is fired.
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

float D3DApp::GetWindowRatio() const
{
    XMINT2 clientSize = m_pWindow->GetWndSize();
    return ((float)clientSize.x / (float)clientSize.y);
}



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

void D3DApp::AddMeshPosition(XMFLOAT4X4* pos)
{
    m_MeshPosition.push_back(pos);
}

void D3DApp::AddMesh(Mesh* mesh)
{
    mesh->SetMeshVisibility(true);
    m_RenderManager->AddMeshToDraw(mesh);
}
