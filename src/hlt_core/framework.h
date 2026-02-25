#pragma once

#pragma comment(lib, "Winmm.lib")

#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <Ws2tcpip.h>
#include <wrl.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <map>
#include <thread>
#include <functional>
#include <cmath>
#include <emmintrin.h>

// DIRECTX API
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

#include <d3d12.h>
using namespace DirectX;

// DEBUG TOOL
#ifdef NDEBUG
constexpr bool DEBUG = false;
#else
constexpr bool DEBUG = true;
#endif

#include "hlt_DebugTools.h"
using namespace hlt_DebugTools;

#ifdef _DEBUG
#include <crtdbg.h>
#endif

/// HLT CORE CLASS DEFINE
#define HLT_MOUSE hlt_Input::MouseInput::GetInstance()
#define HLT_KEYBOARD hlt_Input::KeyboardInput::GetInstance()

/// INPUTS DEFINE
#define VK_Z 0x5A
#define VK_Q 0x51
#define VK_S 0x53
#define VK_D 0x44

// CONST VALUE
const float PI = 3.1415926535f;