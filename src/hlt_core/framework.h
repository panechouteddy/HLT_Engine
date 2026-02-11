#pragma once

#include <iostream>
#include <Windows.h>
#include <memory>
#include <algorithm>

// CONTAINERS
#include <vector>
#include <unordered_map>

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

// ECS
#include "hlt_ECS.inl"

// ECS SYSTEMS
#include "hlt_System.h"
#include "hlt_CMoveSystem.h"
#include "hlt_KeyboardInputSystem.h"

using namespace hlt_System;

// ECS COMPONENT
#include "hlt_Transform3DComponent.h"
#include "hlt_CMoveComponent.h"
#include "hlt_KeyboardInputComponent.h"

using namespace hlt_Component;