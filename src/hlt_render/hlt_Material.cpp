#include "pch.h"

hlt_Material::hlt_Material()
{
	XMStoreFloat3(&color, Colors::PapayaWhip.v);
	values = nullptr;
}
