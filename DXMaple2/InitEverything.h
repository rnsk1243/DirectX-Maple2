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

	// D3D ��ü, ��ġ �ʱ�ȭ
	bool InitD3D(HWND hWnd, LPDIRECT3D9& gpD3D, LPDIRECT3DDEVICE9& gpD3DDevice);
	// ���� �ε�(���̴�,���θ�,��)
	bool LoadAssets(LPDIRECT3DTEXTURE9& gpTextureDM, LPD3DXEFFECT& gpShader, LPD3DXMESH& gpModel,
		const char* texture, const char* shader, const char* model, LPDIRECT3DDEVICE9& gpD3DDevice);
	// UI ��Ʈ �����
	bool LoadFont(LPDIRECT3DDEVICE9& gpD3DDevice, ID3DXFont*& gpFont);

	// ���̴� �ε�
	LPD3DXEFFECT LoadShader(const char * filename, LPDIRECT3DDEVICE9& gpD3DDevice);
	// �ؽ��� �ε�
	LPDIRECT3DTEXTURE9 LoadTexture(const char * filename, LPDIRECT3DDEVICE9& gpD3DDevice);
	// .x File �ε� (��)
	LPD3DXMESH LoadModel(const char * filename, LPDIRECT3DDEVICE9& gpD3DDevice);
};

