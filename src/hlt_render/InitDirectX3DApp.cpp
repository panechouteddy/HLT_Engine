#include "pch.h"
#include "InitDirectX3DApp.h"

InitDirectX3DApp::InitDirectX3DApp(hlt_Window* window)
	: D3DApp(window)
{


}

InitDirectX3DApp::~InitDirectX3DApp()
{
}

bool InitDirectX3DApp::Initialize()
{
	if (!D3DApp::Initialize())
		return false;

	Mesh* meshTest = new Mesh;
	meshTest->InitPyramidMesh();
	//Ajouter le mesh 
	AddMesh(meshTest);

	return true;
}

void InitDirectX3DApp::OnResize()
{
	D3DApp::OnResize();

}
void InitDirectX3DApp::Update()
{
	D3DApp::Update();
}

void InitDirectX3DApp::Draw()
{
	D3DApp::Draw();
}