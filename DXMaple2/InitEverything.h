#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include<Windows.h>
#include <stdio.h>

class CInitEverything
{
public:
	CInitEverything();
	~CInitEverything();
	//bool InitEverything(HWND hWnd, LPDIRECT3DDEVICE9 gpD3DDevice, ID3DXFont* gpFont, LPDIRECT3D9 gpD3D, 
	//	LPDIRECT3DTEXTURE9 gpEarthDM, LPD3DXEFFECT gpTextureMappingShader, LPD3DXMESH gpSphere,
	//	char* texture, char* shader, char* model);
	bool InitD3D(HWND hWnd, LPDIRECT3D9& gpD3D, LPDIRECT3DDEVICE9& gpD3DDevice);
	bool LoadAssets(LPDIRECT3DTEXTURE9& gpEarthDM, LPD3DXEFFECT& gpTextureMappingShader, LPD3DXMESH& gpSphere,
		const char* texture, const char* shader, const char* model, LPDIRECT3DDEVICE9& gpD3DDevice);
	bool LoadFont(LPDIRECT3DDEVICE9& gpD3DDevice, ID3DXFont*& gpFont);

	LPD3DXEFFECT LoadShader(const char * filename, LPDIRECT3DDEVICE9& gpD3DDevice);
	LPDIRECT3DTEXTURE9 LoadTexture(const char * filename, LPDIRECT3DDEVICE9& gpD3DDevice);
	LPD3DXMESH LoadModel(const char * filename, LPDIRECT3DDEVICE9& gpD3DDevice);
};

