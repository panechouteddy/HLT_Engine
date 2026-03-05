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

#include "d3dUtil.h"
//#include "D3DApp.h"
//#include "InitDirecX3DApp.h"
#include "D3DApp.h"

using Microsoft::WRL::ComPtr;
using namespace std;
using namespace DirectX;