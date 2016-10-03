#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include"InitEverything.h"
#include"Render.h"
#include"camera.h"
#include"ObjList.h"
#include<cstdlib>
#include<ctime>

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

#define WIN_WIDTH		1920
#define WIN_HEIGHT		1200

// ī�޶� �̵� �ӵ�
#define CAMERA_SPEED 40
// ī�޶� ȸ�� �ӵ�
#define CAMERA_R 1.5f



class CGameManager
{
	int* xRange;
	int* yRange;
	// ��� �ʱ�ȭ�� ����ϴ� ��ü
	CInitEverything m_Init;
	// ��� ������ ����ϴ� ��ü
	CRender m_render;
	// List
	CObjList* m_RedHouseList;
	CObjList* m_BlueHouseList;
	// �ٴںκ�
	CObjList* m_BoxRound;
	// 1��
	CObjList* m_floor1;
	//=========== list �� //
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

	// ���� �̸� ����ü
	nodeAssetName* m_redHouse;
	nodeAssetName* m_blueHouse;
	
	nodeAssetName* m_box0;
	nodeAssetName* m_box1;
	nodeAssetName* m_box2;
	nodeAssetName* m_box3;
	nodeAssetName* m_box4;

	nodeAssetName* m_treeBlue;
	nodeAssetName* m_treeBora;
	nodeAssetName* m_treeGreen;

	// ��ü�� �׸��µ� �ʿ��� ���� ����ü
	nodeObj* m_ObjRedHouse;
	nodeObj* m_ObjBlueHouse;

	nodeObj* m_Objm_box0;
	nodeObj* m_Objm_box1;
	nodeObj* m_Objm_box2;
	nodeObj* m_Objm_box3;
	nodeObj* m_Objm_box4;

	nodeObj* m_ObjmTreeBlue;
	nodeObj* m_ObjmTreeBora;
	nodeObj* m_ObjmTreeGreen;

	// ��� �ʱ�ȭ�� ����� �̷�� ����?
	bool m_IsRead;

public:
	CGameManager(HWND hWnd)
	{
		m_hWnd = hWnd;
		// m_matView ��� �ʱ�ȭ ������ķ�
		D3DXMatrixIdentity(&m_matView);

		// ���� �̸� �ʱ�ȭ
		m_redHouse = new nodeAssetName("house.png", "TextureMapping.fx", "house_non_left.x");
		m_blueHouse = new nodeAssetName("blue_house.png", "TextureMapping.fx", "house_non_left.x");
		
		m_box0 = new nodeAssetName("box0.png", "TextureMapping.fx", "box_non_left.x");
		m_box1 = new nodeAssetName("box1.png", "TextureMapping.fx", "box_non_left.x");
		m_box2 = new nodeAssetName("box2.png", "TextureMapping.fx", "box_non_left.x");
		m_box3 = new nodeAssetName("box3.png", "TextureMapping.fx", "box_non_left.x");
		m_box4 = new nodeAssetName("box4.png", "TextureMapping.fx", "box_non_left.x");

		m_treeBlue = new nodeAssetName("tree_blue.png", "TextureMapping.fx", "treeNon_left.x");
		m_treeBora = new nodeAssetName("tree_bora.png", "TextureMapping.fx", "treeNon_left.x");
		m_treeGreen = new nodeAssetName("tree_green.png", "TextureMapping.fx", "treeNon_left.x");

		// ���� ����ü �ʱ�ȭ
		m_ObjRedHouse = new nodeObj(m_redHouse);
		m_ObjBlueHouse = new nodeObj(m_blueHouse);

		m_Objm_box0 = new nodeObj(m_box0);
		m_Objm_box1 = new nodeObj(m_box1);
		m_Objm_box2 = new nodeObj(m_box2);
		m_Objm_box3 = new nodeObj(m_box3);
		m_Objm_box4 = new nodeObj(m_box4);

		m_ObjmTreeBlue = new nodeObj(m_treeBlue);
		m_ObjmTreeBora = new nodeObj(m_treeBora);
		m_ObjmTreeGreen = new nodeObj(m_treeGreen);

		// ������Ʈ�� ���� ����Ʈ
		m_RedHouseList = new CObjList();
		m_BlueHouseList = new CObjList();
		m_BoxRound = new CObjList();
		m_floor1 = new CObjList();

		// D3D ��ġ �ʱ�ȭ
		if (!m_Init.InitD3D(m_hWnd, m_gpD3D, m_gpD3DDevice))
		{
			return;
		}

		// ���� ����ü���� ���� �������� �ʱ�ȭ(����ü ���� �ּҸ� �ʿ�� ��)
		if (!m_Init.LoadAssets(m_ObjRedHouse, m_gpD3DDevice))
		{
			return;
		}
		if (!m_Init.LoadAssets(m_ObjBlueHouse, m_gpD3DDevice))
		{
			return;
		}
		if (!m_Init.LoadAssets(m_Objm_box0, m_gpD3DDevice))
		{
			return;
		}
		if (!m_Init.LoadAssets(m_Objm_box1, m_gpD3DDevice))
		{
			return;
		}
		if (!m_Init.LoadAssets(m_Objm_box2, m_gpD3DDevice))
		{
			return;
		}
		if (!m_Init.LoadAssets(m_Objm_box3, m_gpD3DDevice))
		{
			return;
		}
		if (!m_Init.LoadAssets(m_Objm_box4, m_gpD3DDevice))
		{
			return;
		}
		if (!m_Init.LoadAssets(m_ObjmTreeBlue, m_gpD3DDevice))
		{
			return;
		}
		if (!m_Init.LoadAssets(m_ObjmTreeBora, m_gpD3DDevice))
		{
			return;
		}
		if (!m_Init.LoadAssets(m_ObjmTreeGreen, m_gpD3DDevice))
		{
			return;
		}

		// ��Ʈ
		m_IsRead = m_Init.LoadFont(m_gpD3DDevice, m_gpFont);
		
		// 
		if (m_IsRead)
		{
			//pushFront�Լ��� ���� ������Ʈ ����ü�� �����Ͽ� ����Ʈ�� �߰��� 
			//m_RedHouseList->pushFront(m_ObjRedHouse);
			//m_BlueHouseList->pushFront(m_ObjBlueHouse);
			
			xRange = new int[30];
			yRange = new int[30];
			srand((unsigned int)time(NULL));
			for (int i = 0; i < 30; i++)
			{
				xRange[i] = rand() % 11 + 1;
				yRange[i] = rand() % 11 + 1;
			}

			// 1�� 
			srand((unsigned int)time(NULL));
			for (int i = 0; i < 30; i++)
			{
				int randomVal = rand() % 10;

				if (randomVal == 0)
				{
					m_floor1->pushFront(m_Objm_box0);
				}
				else if (randomVal == 1)
				{
					m_floor1->pushFront(m_Objm_box1);
				}
				else if (randomVal == 2)
				{
					m_floor1->pushFront(m_Objm_box2);
				}
				else if (randomVal == 3)
				{
					m_floor1->pushFront(m_Objm_box3);
				}
				else if (randomVal == 4)
				{
					m_floor1->pushFront(m_Objm_box4);
				}
				else if (randomVal == 5)
				{
					m_floor1->pushFront(m_ObjBlueHouse);
				}
				else if (randomVal == 6)
				{
					m_floor1->pushFront(m_ObjRedHouse);
				}
				else if (randomVal == 7)
				{
					m_floor1->pushFront(m_ObjmTreeBlue);
				}
				else if (randomVal == 8)
				{
					m_floor1->pushFront(m_ObjmTreeBora);
				}
				else if (randomVal == 9)
				{
					m_floor1->pushFront(m_ObjmTreeGreen);
				}

			}
			
			// �ٴ�
			srand((unsigned int)time(NULL));
			for (int i = 0; i < 144; i++)
			{
				int randomVal = rand() % 5;

				if (randomVal == 0)
				{
					m_BoxRound->pushFront(m_Objm_box0);
				}
				else if (randomVal == 1)
				{
					m_BoxRound->pushFront(m_Objm_box1);
				}
				else if (randomVal == 2)
				{
					m_BoxRound->pushFront(m_Objm_box2);
				}
				else if (randomVal == 3)
				{
					m_BoxRound->pushFront(m_Objm_box3);
				}
				else if (randomVal == 4)
				{
					m_BoxRound->pushFront(m_Objm_box4);
				}
			}
		}
		
	}
	~CGameManager();

	bool LoadMap();
	bool LoadUI();
	void CameraUpdate(float deltaTime);
	void Update();
	void Cleanup(CObjList* obj);
	void RenderFrame();

	bool getRead() { return m_IsRead; }

	CObjList*& getRedHouseObjList()
	{
		return m_RedHouseList;
	}
	CObjList*& getBlueHouseObjList()
	{
		return m_BlueHouseList;
	}
};

