#pragma once
#include <d3d9.h>
#include <d3dx9.h>

class CRender
{
public:
	CRender();
	~CRender();

	void RenderScene(LPD3DXEFFECT& gpTextureMappingShader, LPD3DXMESH& gpSphere, LPDIRECT3DTEXTURE9& gpEarthDM);
	void RenderInfo(ID3DXFont*& gpFont, char* info, int width, int height);
};

