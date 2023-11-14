#pragma once
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <string>
#include <memory>
#include "app.h"

struct AppDesc {
    std::shared_ptr<class IApp> app;
    std::wstring name;
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
};