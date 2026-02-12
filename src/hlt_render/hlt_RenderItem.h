#pragma once
class hlt_RenderItem
{
public:
	hlt_RenderItem();
	~hlt_RenderItem() = default;

	XMFLOAT4X4 m_World;
	
	int m_NumFrame;
	
	UINT m_ObjCBIndex;

	hlt_Material* m_Material;
};

