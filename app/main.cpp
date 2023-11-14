#pragma comment(lib, "engine.lib")

#include "framework.h"
#include "engine.h"
#include "triangle.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    AppDesc desc;
    desc.hInstance = hInstance;
    desc.hWnd = NULL;
    desc.width = 720;
    desc.height = 720;
    desc.app = std::make_shared<Triangle>();
    desc.name = L"app";

    std::unique_ptr<Engine> engine = std::make_unique<Engine>(desc);
    engine->Run();

    return 0;
}
