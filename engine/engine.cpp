#include "pch.h"
#include "engine.h"

Engine::Engine(AppDesc desc)
    : mAppDesc { desc }
{
}

WPARAM Engine::Run()
{
    MyRegisterClass();

    if (!InitInstance(SW_SHOWNORMAL))
        return FALSE;

    assert(mAppDesc.app);

    CreateDeviceAndSwapChain();
    CreateRenderTargetView();
    CreateDepthStencilView();
    SetViewport();

    mAppDesc.app->Init();

    MSG msg = { 0 };
    while (msg.message != WM_QUIT) {
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        } else {
            Update();
        }
    }

    return msg.wParam;
}

void Engine::Update()
{
    {
        float clear[4] = { 0.3f, 0.3f, 0.3f, 1.0f };

        mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
        mContext->RSSetViewports(1, &mViewport);

        mContext->ClearRenderTargetView(mRenderTargetView.Get(), clear);
        mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }

    mAppDesc.app->Update();
    mAppDesc.app->Render();

    {
        mSwapChain->Present(1, 0);
    }
}

ATOM Engine::MyRegisterClass()
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = mAppDesc.hInstance;
    wcex.hIcon = ::LoadIcon(NULL, IDI_WINLOGO);
    wcex.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = mAppDesc.name.c_str();
    wcex.hIconSm = wcex.hIcon;

    return RegisterClassExW(&wcex);
}

BOOL Engine::InitInstance(int cmdShow)
{
    RECT windowRect = { 0, 0, mAppDesc.width, mAppDesc.height };
    ::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

    mAppDesc.hWnd = ::CreateWindowW(mAppDesc.name.c_str(), mAppDesc.name.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, mAppDesc.hInstance, nullptr);

    if (!mAppDesc.hWnd) {
        return FALSE;
    }

    ::ShowWindow(mAppDesc.hWnd, cmdShow);
    ::UpdateWindow(mAppDesc.hWnd);

    return TRUE;
}

LRESULT Engine::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_SIZE:
        break;
    case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return ::DefWindowProc(handle, message, wParam, lParam);
    }

    return 0;
}

void Engine::CreateDeviceAndSwapChain()
{
    DXGI_SWAP_CHAIN_DESC desc;
    ZeroMemory(&desc, sizeof(desc));

    desc.BufferDesc.Width = mAppDesc.width;
    desc.BufferDesc.Height = mAppDesc.height;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferCount = 1;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.Windowed = TRUE;
    desc.OutputWindow = mAppDesc.hWnd;

    HRESULT hr = ::D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &desc, mSwapChain.GetAddressOf(), mDevice.GetAddressOf(), nullptr, mContext.GetAddressOf());

    assert(SUCCEEDED(hr));
}

void Engine::CreateRenderTargetView()
{
    HRESULT hr;
    ComPtr<ID3D11Texture2D> backBuffer;

    hr = mSwapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
    assert(SUCCEEDED(hr));

    hr = mDevice->CreateRenderTargetView(backBuffer.Get(), NULL, mRenderTargetView.GetAddressOf());
    assert(SUCCEEDED(hr));
}

void Engine::CreateDepthStencilView()
{
    HRESULT hr;
    {
        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof(desc));

        desc.Width = mAppDesc.width;
        desc.Height = mAppDesc.height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

        hr = mDevice->CreateTexture2D(&desc, nullptr, mDetphStencilTexture.GetAddressOf());
        assert(SUCCEEDED(hr));
    }

    {
        D3D11_DEPTH_STENCIL_VIEW_DESC desc;
        ZeroMemory(&desc, sizeof(desc));

        desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        desc.Flags = 0;
        desc.Texture2D.MipSlice = 0;

        hr = mDevice->CreateDepthStencilView(mDetphStencilTexture.Get(), &desc, mDepthStencilView.GetAddressOf());
        assert(SUCCEEDED(hr));
    }
}

void Engine::SetViewport()
{
    mViewport.TopLeftX = 0.0f;
    mViewport.TopLeftY = 0.0f;
    mViewport.Width = static_cast<float>(mAppDesc.width);
    mViewport.Height = static_cast<float>(mAppDesc.height);
    mViewport.MinDepth = 0.0f;
    mViewport.MaxDepth = 1.0f;
}
