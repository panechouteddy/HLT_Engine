#pragma once

#include <iostream>
#include <WindowsX.h>
#include <filesystem>

#include <hlt_core/framework.h>
#include <hlt_core/hlt_Time.h>
#include <hlt_core/hlt_Window.h>
#include <hlt_core/hlt_Transform3D.h>
#include <hlt_core/hlt_Input.h>
#include <hlt_core/hlt_Color.h>

#include "hlt_Camera.h"

#include "UploadBuffer.h"
#include "ConstantBuffer.h"
#include "ColorBuffer.h"
#include "Mesh.h"
#include "Texture.h"
#include "RenderManager.h"

using Microsoft::WRL::ComPtr;
using namespace std;
using namespace DirectX;

#include "D3DApp.h"
#include "d3dUtil.h"
#include "hlt_D2DResource.h"
#include "hlt_TextureLoader.h" 
#include "GameTimer.h"
#include "MathHelper.h"

#include "hlt_Material.h"
#include "hlt_Camera.h"
#include "hlt_Light.h"
#include "hlt_Shader.h"
#include "hlt_RenderItem.h"
#include "hlt_UI.h"
#include "hlt_PSO.h"
#include "hlt_SplashScreen.h"


#include "d3dUtil.h"
#include "D3DApp.h"