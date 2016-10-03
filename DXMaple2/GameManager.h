#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include"InitEverything.h"
#include"Render.h"
#include"camera.h"

// ��������� ���鶧 �ʿ��� ������� #define������ �����մϴ�.
#define PI 3.14159265f
// �þ߰�
#define FOV (PI/4.0f)
// ȭ���� ��Ⱦ��
#define ASPECT_RATIO (WIN_WIDTH/(float)WIN_HEIGHT)
// ���� ���
#define NEAR_PLANE 1
// ���Ÿ� ���
#define FAR_PLANE 10000

#define WIN_WIDTH		800
#define WIN_HEIGHT		600

// ī�޶� �̵� �ӵ�
#define CAMERA_SPEED 20
// ī�޶� ȸ�� �ӵ�
#define CAMERA_R 1



class CGameManager
{
	// ��� �ʱ�ȭ�� ����ϴ� ��ü
	CInitEverything m_Init;
	// ��� ������ ����ϴ� ��ü
	CRender m_render;
	// ī�޶� ��ü
	Camera m_camera;
	// �亯ȯ ���
	D3DXMATRIXA16 m_matView;
	// ������ �ڵ�
	HWND m_hWnd;
	// D3D ��ġ
	LPDIRECT3DDEVICE9 m_gpD3DDevice;
	// ��Ʈ
	ID3DXFont* m_gpFont;
	// D3D ��ġ ������ �ʿ�
	LPDIRECT3D9 m_gpD3D;
	// �ؽ���
	LPDIRECT3DTEXTURE9 m_gpTextureDM;
	// ���̴�
	LPD3DXEFFECT m_Shader;
	// ��
	LPD3DXMESH m_gpModel;



	// ��� �ʱ�ȭ�� ����� �̷�� ����?
	bool m_IsRead;

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
		// m_matView ��� �ʱ�ȭ ������ķ�
		D3DXMatrixIdentity(&m_matView);

		
		
		
		m_IsRead = m_Init.InitD3D(m_hWnd, m_gpD3D, m_gpD3DDevice);
		m_IsRead = m_Init.LoadAssets(m_gpTextureDM, m_Shader, m_gpModel, m_texture, m_shader, m_xFile, m_gpD3DDevice);
		m_IsRead = m_Init.LoadFont(m_gpD3DDevice, m_gpFont);
	}
	~CGameManager();

	bool LoadMap();
	bool LoadUI();
	void CameraUpdate(float deltaTime);
	void Update();
	void Cleanup();
	void RenderFrame();

	bool getRead() { return m_IsRead; }
};

