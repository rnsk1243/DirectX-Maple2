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

	// D3D 객체, 장치 초기화
	bool InitD3D(HWND hWnd, LPDIRECT3D9& gpD3D, LPDIRECT3DDEVICE9& gpD3DDevice);
	// 에셋 로드(쉐이더,매핑맵,모델)
	bool LoadAssets(LPDIRECT3DTEXTURE9& gpTextureDM, LPD3DXEFFECT& gpShader, LPD3DXMESH& gpModel,
		const char* texture, const char* shader, const char* model, LPDIRECT3DDEVICE9& gpD3DDevice);
	// UI 폰트 만들기
	bool LoadFont(LPDIRECT3DDEVICE9& gpD3DDevice, ID3DXFont*& gpFont);

	// 쉐이더 로드
	LPD3DXEFFECT LoadShader(const char * filename, LPDIRECT3DDEVICE9& gpD3DDevice);
	// 텍스쳐 로드
	LPDIRECT3DTEXTURE9 LoadTexture(const char * filename, LPDIRECT3DDEVICE9& gpD3DDevice);
	// .x File 로드 (모델)
	LPD3DXMESH LoadModel(const char * filename, LPDIRECT3DDEVICE9& gpD3DDevice);
};

