#pragma once

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