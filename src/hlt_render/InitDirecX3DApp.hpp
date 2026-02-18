#include "pch.h"
#include "D3DApp.h"

#include <array>

#include "ConstantBuffer.h"

using namespace DirectX;

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;



class InitDirect3DApp : public D3DApp
{
public:
	InitDirect3DApp(HINSTANCE hInstance);
	~InitDirect3DApp();

	virtual bool Initialize()override;
private:
	ConstantBuffer* m_CBobject;

	virtual void OnResize()override;
	virtual void Update(const GameTimer& gt)override;
	virtual void Draw(const GameTimer& gt)override;


	float mTheta = 1.5f * XM_PI;
	float mPhi = XM_PIDIV4;
	float mRadius = 5.0f;

	XMFLOAT4X4 m_pos = MathHelper::Identity4x4();

};



InitDirect3DApp::InitDirect3DApp(HINSTANCE hInstance)
	: D3DApp(hInstance)
{


}

InitDirect3DApp::~InitDirect3DApp()
{
}

bool InitDirect3DApp::Initialize()
{
	if (!D3DApp::Initialize())
		return false;

	Mesh* meshTest = new Mesh;
	meshTest->InitPyramidMesh();
	//Ajouter le mesh 
	AddMesh(meshTest);

	//Ajouter la position (transform)
	m_pos._43 = 1;
	AddMeshPosition(&m_pos);
	
}

void InitDirect3DApp::OnResize()
{
	D3DApp::OnResize();

}
 void InitDirect3DApp::Update(const GameTimer& gt)
{
	D3DApp::Update(gt);
}

void InitDirect3DApp::Draw(const GameTimer& gt)
{
	D3DApp::Draw(gt);

}

