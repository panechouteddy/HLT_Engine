#include "pch.h"

hlt_Material::hlt_Material()
{
	XMStoreFloat4(&color, Colors::PapayaWhip);
	values = nullptr;
}
