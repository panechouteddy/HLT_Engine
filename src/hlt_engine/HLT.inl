#pragma once
#include "pch.h"
#include "HLT.h"

template <typename T>
inline void HLT::Run()
{
	hlt_GameManager& gm = hlt_GameManager::GetInstance();

	T* app = new T();

	gm.Run();
}