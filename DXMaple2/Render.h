#pragma once
#include <d3d9.h>
#include <d3dx9.h>

class CRender
{
public:
	CRender();
	~CRender();
	// 3D ��ü���� �׸���. (���̴�,��,�ؽ���)
	void RenderScene(LPD3DXEFFECT& gpShader, LPD3DXMESH& gpModel, LPDIRECT3DTEXTURE9& gpTextureDM);
	// ���� ���(��Ʈ,����,��ġ)
	void RenderInfo(ID3DXFont*& gpFont, char* info, int width, int height);
};

