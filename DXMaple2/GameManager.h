#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include"InitEverything.h"
#include"Render.h"
#include"camera.h"

// 투영행렬을 만들때 필요한 상수들을 #define문으로 정의합니다.
#define PI 3.14159265f
// 시야각
#define FOV (PI/4.0f)
// 화면의 종횡비
#define ASPECT_RATIO (WIN_WIDTH/(float)WIN_HEIGHT)
// 근접 평면
#define NEAR_PLANE 1
// 원거리 평면
#define FAR_PLANE 10000

#define WIN_WIDTH		800
#define WIN_HEIGHT		600

class CGameManager
{
	CInitEverything m_Init;
	CRender m_render;
	Camera m_camera;

	D3DXMATRIXA16 m_matView;

	HWND m_hWnd;
	LPDIRECT3DDEVICE9 m_gpD3DDevice;
	ID3DXFont* m_gpFont;
	LPDIRECT3D9 m_gpD3D;
	LPDIRECT3DTEXTURE9 m_gpTextureDM;
	LPD3DXEFFECT m_Shader;
	LPD3DXMESH m_gpModel;

	bool m_Read;

	const char* m_texture = "house.png";
	const char* m_shader = "TextureMapping.fx";
	const char* m_xFile = "house_non_left.x";
public:
	CGameManager(HWND hWnd, LPDIRECT3DDEVICE9 gpD3DDevice, ID3DXFont* gpFont, LPDIRECT3D9 gpD3D,
		LPDIRECT3DTEXTURE9 gpTextureDM, LPD3DXEFFECT gpShader, LPD3DXMESH gpModel)
	{
		m_hWnd = hWnd; m_gpD3DDevice = gpD3DDevice; m_gpFont = gpFont;
		m_gpD3D = gpD3D; m_gpTextureDM = gpTextureDM; m_Shader = gpShader;
		m_gpModel = gpModel;
		D3DXMatrixIdentity(&m_matView);

		
		
		
		m_Read = m_Init.InitD3D(m_hWnd, m_gpD3D, m_gpD3DDevice);
		m_Read = m_Init.LoadAssets(m_gpTextureDM, m_Shader, m_gpModel, m_texture, m_shader, m_xFile, m_gpD3DDevice);
		m_Read = m_Init.LoadFont(m_gpD3DDevice, m_gpFont);
	}
	~CGameManager();

	bool LoadMap();
	bool LoadUI();
	void CameraUpdate(float deltaTime);
	void Update();
	void Cleanup();
	void RenderFrame();

	bool getRead() { return m_Read; }
};

