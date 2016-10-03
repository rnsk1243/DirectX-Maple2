#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include"ObjList.h"

class CRender
{
public:
	CRender();
	~CRender();
	// 3D 물체등을 그린다. (쉐이더,모델,텍스쳐)
	void RenderScene(LPD3DXEFFECT& gpShader, LPD3DXMESH& gpModel, LPDIRECT3DTEXTURE9& gpTextureDM);
	// 리스트에 담긴 3D 물체를 모두 그린다.
	void RenderScene(CObjList*& objList);
	// 글자 출력(폰트,내용,위치)
	void RenderInfo(ID3DXFont*& gpFont, char* info, int width, int height);

	void RenderGround(CObjList*& objList, D3DXMATRIXA16* View);

	void RenderRandomGround(CObjList*& objList, D3DXMATRIXA16* View, int floor, int* XRange, int* YRange);
};

