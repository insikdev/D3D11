#pragma once

#include "pch.h"
#include "app.h"

struct AppDesc {
    shared_ptr<class IApp> app;
    wstring name;
    HINSTANCE hInstance;
    HWND hWnd;
    LONG width;
    LONG height;
};

class Engine {
public:
    Engine(AppDesc);
    WPARAM Run();

private:
    void Update();

private: // Windows
    ATOM MyRegisterClass();
    BOOL InitInstance(int cmdShow);
    static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

private:
    AppDesc mAppDesc;

private:
    void CreateDeviceAndSwapChain();
    void CreateRenderTargetView();
    void CreateDepthStencilView();
    void SetViewport();

private:
    ComPtr<ID3D11Device> mDevice;
    ComPtr<ID3D11DeviceContext> mContext;
    ComPtr<IDXGISwapChain> mSwapChain;
    ComPtr<ID3D11RenderTargetView> mRenderTargetView;
    ComPtr<ID3D11Texture2D> mDetphStencilTexture;
    ComPtr<ID3D11DepthStencilView> mDepthStencilView;
    D3D11_VIEWPORT mViewport;
};