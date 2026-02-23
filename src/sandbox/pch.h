#ifndef PCH_H
#define PCH_H

#pragma comment (lib,"../hlt_core/x64/Debug/hlt_core.lib")
#pragma comment (lib,"../hlt_render/x64/Debug/hlt_render.lib")
#pragma comment (lib,"../hlt_engine/x64/Debug/hlt_engine.lib")

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

#endif
