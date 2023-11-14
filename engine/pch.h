#pragma once

#define WIN32_LEAN_AND_MEAN
#pragma comment(lib, "d3d11.lib")

#include <Windows.h>
#include <string>
#include <memory>
#include <cassert>

// DirectX
#include <wrl.h>
#include <d3d11.h>

using namespace std;
using namespace Microsoft::WRL;