#include "engine.h"
#include <cassert>

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
    mAppDesc.app->Update();
    mAppDesc.app->Render();
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

    mAppDesc.hWnd = CreateWindowW(mAppDesc.name.c_str(), mAppDesc.name.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, mAppDesc.hInstance, nullptr);

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
