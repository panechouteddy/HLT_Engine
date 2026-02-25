#pragma once

// LOWER LEVEL FRAMEWORK
#include <hlt_core/framework.h>
#include <hlt_render/framework.h>

// CONTAINERS
#include <vector>
#include <unordered_map>

// LOGIC
#include <algorithm>

// UTILITY
#include <hlt_core/hlt_Time.h>
#include <hlt_core/hlt_Window.h>

// ECS
#include "hlt_ECS.inl"

// ECS COMPONENT
#include "hlt_Transform3DComponent.h"
#include "hlt_ConstantMoveComponent.h"
#include "hlt_InputComponent.h"
#include "hlt_ColliderComponent.h"
#include "hlt_MeshComponent.h"
#include "hlt_HierarchyComponent.h"

// ECS SYSTEMS
#include "hlt_System.h"
#include "hlt_ConstantMoveSystem.h"
#include "hlt_InputSystem.h"
#include "hlt_ColliderSystem.h"
#include "hlt_HierarchySystem.h"

// ENTITY MANAGER
#include "hlt_EntityManager.h"

// APP USING FUNCTION
#include "hlt_ToCall.h"

// GAME MANAGER
#include "hlt_GameManager.h"

// HLT GLOBAL
#include "HLT.inl"


// HLT ENGINE CLASS DEFINE
#define HLT_TIME hlt_Time::GetInstance()
#define HLT_WINDOW hlt_Window::GetInstance()
#define HLT_GAMEMANAGER hlt_GameManager::GetInstance()
#define HLT_CAMERA HLT_GAMEMANAGER.GetCamera()

// SETUP
#define SETUP_APP_ONSTART(method) HLT_GAMEMANAGER.GetAppMainMethods().m_Start.Set(this, &App::method)
#define SETUP_APP_ONUPDATE(method) HLT_GAMEMANAGER.GetAppMainMethods().m_Update.Set(this, &App::method)
#define SETUP_APP_ONEXIT(method) HLT_GAMEMANAGER.GetAppMainMethods().m_Exit.Set(this, &App::method)

// RUN