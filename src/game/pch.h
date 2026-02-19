#ifndef PCH_H
#define PCH_H

#include <iostream>
#include <Windows.h>
#include <memory>

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
#include <hlt_core/hlt_DebugTools.h>
constexpr bool DEBUG = true;
#endif

// ECS
#include <hlt_core/hlt_ECS.h>

// ECS SYSTEMS
#include <hlt_core/hlt_System.h>
#include <hlt_core/hlt_CMoveSystem.h>

using namespace hlt_System;

// ECS COMPONENT
#include <hlt_core/hlt_Transform3DComponent.h>
#include <hlt_core/hlt_CMoveComponent.h>

using namespace hlt_Component;


#endif