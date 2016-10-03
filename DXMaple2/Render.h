#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include"ObjList.h"

class CRender
{
public:
	CRender();
	~CRender();
	// 3D ��ü���� �׸���. (���̴�,��,�ؽ���)
	void RenderScene(LPD3DXEFFECT& gpShader, LPD3DXMESH& gpModel, LPDIRECT3DTEXTURE9& gpTextureDM);
	// ����Ʈ�� ��� 3D ��ü�� ��� �׸���.
	void RenderScene(CObjList*& objList);
	// ���� ���(��Ʈ,����,��ġ)
	void RenderInfo(ID3DXFont*& gpFont, char* info, int width, int height);

	void RenderGround(CObjList*& objList, D3DXMATRIXA16* View);

	void RenderRandomGround(CObjList*& objList, D3DXMATRIXA16* View, int floor, int* XRange, int* YRange);
};

