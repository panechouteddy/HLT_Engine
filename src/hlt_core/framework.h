#pragma once

#pragma comment(lib, "Winmm.lib")

#include <iostream>

// DIRECTX API
#include <DirectXMath.h>

using namespace DirectX;

// DEBUG TOOL
#ifdef NDEBUG
constexpr bool DEBUG = false;
#else
#include "hlt_DebugTools.h"
using namespace hlt_DebugTools;
constexpr bool DEBUG = true;
#endif


/// INPUTS DEFINE
#define VK_Z 0x5A
#define VK_Q 0x51
#define VK_S 0x53
#define VK_D 0x44
