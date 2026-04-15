#ifndef PCH_H
#define PCH_H

#ifdef NDEBUG
#pragma comment (lib,"../hlt_core/x64/Release/hlt_core.lib")
#pragma comment (lib,"../hlt_render/x64/Release/hlt_render.lib")
#pragma comment (lib,"../hlt_engine/x64/Release/hlt_engine.lib")
#else
#pragma comment (lib,"../hlt_core/x64/Debug/hlt_core.lib")
#pragma comment (lib,"../hlt_render/x64/Debug/hlt_render.lib")
#pragma comment (lib,"../hlt_engine/x64/Debug/hlt_engine.lib")
#endif

#include <hlt_engine/framework.h>

// UTILITY
#include <hlt_engine/hlt_Prefab.h>

// APP 
#include "App.h"

// TEST FILES

#include "Test_ECS.hpp"
#include "Test_Input.hpp"
#include "Test_GameManager.hpp"
#include "Test_GameManagerXRender.hpp"


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#endif
