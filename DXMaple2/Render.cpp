#include "Render.h"
#include"GameManager.h"
#include<cstdlib>
#include<ctime>



CRender::CRender()
{
}


CRender::~CRender()
{
}

// 3D ��ü���� �׸���. 
void CRender::RenderScene(LPD3DXEFFECT& gpShader, LPD3DXMESH& gpModel, LPDIRECT3DTEXTURE9& gpTextureDM)
{
	

	
	// .fx���Ͽ��� sampler2D DiffuseSampler �ٷ� ���� texture ������������ ����� ������ ã��
	// ù��° �Ű������� �־��ش�.
	gpShader->SetTexture("DiffuseMap_Tex", gpTextureDM);
	// �н��� ���� ������ ���� ����
	UINT numPasses;
	// �н��� ���� ����
	gpShader->Begin(&numPasses, NULL);
	{
		// �н� ��ŭ ��ü�� �׸�.
		for (UINT i = 0;i < numPasses;++i)
		{
			gpShader->BeginPass(i);
			{
				// �𵨸� �׸���.
				gpModel->DrawSubset(0);
			}
			gpShader->EndPass();
		}
	}
	gpShader->End();
}

void CRender::RenderScene(CObjList*& objList)
{

	for (int i = 0; i < objList->getCount(); i++)
	{
		//D3DXMATRIXA16 matWorld;
		//D3DXMatrixTranslation(&matWorld, i*200.f, 0.f, 0.f);

		nodeObj* temp = objList->getObj(i);

		LPD3DXEFFECT gpShader = temp->Shader;
		LPD3DXMESH gpModel = temp->gpModel;
		LPDIRECT3DTEXTURE9 gpTextureDM = temp->gpTextureDM;

		//gpShader->SetMatrix("gWorldMatrix", &matWorld);

		// .fx���Ͽ��� sampler2D DiffuseSampler �ٷ� ���� texture ������������ ����� ������ ã��
		// ù��° �Ű������� �־��ش�.
		gpShader->SetTexture("DiffuseMap_Tex", gpTextureDM);
		// �н��� ���� ������ ���� ����
		UINT numPasses;
		// �н��� ���� ����
		gpShader->Begin(&numPasses, NULL);
		{
			// �н� ��ŭ ��ü�� �׸�.
			for (UINT i = 0;i < numPasses;++i)
			{
				gpShader->BeginPass(i);
				{
					// �𵨸� �׸���.
					gpModel->DrawSubset(0);
				}
				gpShader->EndPass();
			}
		}
		gpShader->End();
	}
}

void CRender::RenderGround(CObjList*& objList, D3DXMATRIXA16* View)
{
	// ��������� �����.
	// �ο������ ���� ��
	D3DXMATRIXA16 matProjection;
	// ��������� ������ִ� �Լ� ȣ��
	D3DXMatrixPerspectiveFovLH(&matProjection, FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);

	int j = 0;
	int row = 0;
	D3DXMATRIXA16 matWorld;
	for (int i = 0; i < objList->getCount(); i++)
	{
		if (j > 13)
		{
			row++;
			j = 0;
		}
		
		D3DXMatrixTranslation(&matWorld, j * 90.0f, 0.f, row * 90.0f);
		j++;
		nodeObj* temp = objList->getObj(i);

		LPD3DXEFFECT gpShader = temp->Shader;
		LPD3DXMESH gpModel = temp->gpModel;
		LPDIRECT3DTEXTURE9 gpTextureDM = temp->gpTextureDM;

		gpShader->SetMatrix("gWorldMatrix", &matWorld);


		gpShader->SetMatrix("gViewMatrix", View);
		gpShader->SetMatrix("gProjectionMatrix", &matProjection);

		// .fx���Ͽ��� sampler2D DiffuseSampler �ٷ� ���� texture ������������ ����� ������ ã��
		// ù��° �Ű������� �־��ش�.
		gpShader->SetTexture("DiffuseMap_Tex", gpTextureDM);
		// �н��� ���� ������ ���� ����
		UINT numPasses;
		// �н��� ���� ����
		gpShader->Begin(&numPasses, NULL);
		{
			// �н� ��ŭ ��ü�� �׸�.
			for (UINT i = 0;i < numPasses;++i)
			{
				gpShader->BeginPass(i);
				{
					// �𵨸� �׸���.
					gpModel->DrawSubset(0);
				}
				gpShader->EndPass();
			}
		}
		gpShader->End();
	}
}


void CRender::RenderRandomGround(CObjList*& objList, D3DXMATRIXA16* View, int floor, int* XRange, int* YRange)
{
	// ������ ��ġ
	D3DXVECTOR4				gWorldLightPosition = D3DXVECTOR4(500.0f, 500.0f, 500.0f, 1.0f);
	// ǥ���� ����
	D3DXVECTOR4				gSurfaceColor = D3DXVECTOR4(1, 0.49804, 0, 1);

	// ��������� �����.
	// �ο������ ���� ��
	D3DXMATRIXA16 matProjection;
	// ��������� ������ִ� �Լ� ȣ��
	D3DXMatrixPerspectiveFovLH(&matProjection, FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);

	// ���� ��� ��ǥ
	D3DXMATRIXA16 matWorld;
	for (int i = 0; i < objList->getCount(); i++)
	{

		nodeObj* temp = objList->getObj(i);

		LPD3DXEFFECT gpShader = temp->Shader;
		LPD3DXMESH gpModel = temp->gpModel;
		LPDIRECT3DTEXTURE9 gpTextureDM = temp->gpTextureDM;
		D3DXMATRIXA16 matWorldViewProjection;
		if (temp->_name->texture == "house.png" || temp->_name->texture == "blue_house.png")
		{
			// ��������� ������� ���Ѵ�.
			D3DXMATRIXA16 matInvWorld;
			D3DXMatrixTranspose(&matInvWorld, &matWorld);

			// ����/��/��������� �̸� ���Ѵ�.
			D3DXMATRIXA16 matWorldView;
			
			D3DXMatrixMultiply(&matWorldView, &matWorld, View);
			D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProjection);

			// ���̴� ������������ ����
			gpShader->SetMatrix("gWorldViewProjectionMatrix", &matWorldViewProjection);
			gpShader->SetMatrix("gInvWorldMatrix", &matInvWorld);

			gpShader->SetVector("gWorldLightPosition", &gWorldLightPosition);
			gpShader->SetVector("gSurfaceColor", &gSurfaceColor);

			D3DXMatrixTranslation(&matWorld, XRange[i] * 90.0f, floor * 72.f, YRange[i] * 90.0f);
		}
		else if (temp->_name->texture == "tree_blue.png" || temp->_name->texture == "tree_bora.png" || temp->_name->texture == "tree_green.png")
		{
			D3DXMatrixTranslation(&matWorld, XRange[i] * 90.0f, floor * 48.f, YRange[i] * 90.0f);
		}
		else
		{
			D3DXMatrixTranslation(&matWorld, XRange[i] * 90.0f, floor * 90.f, YRange[i] * 90.0f);
		}
		

		

		gpShader->SetMatrix("gWorldMatrix", &matWorld);


		gpShader->SetMatrix("gViewMatrix", View);
		gpShader->SetMatrix("gProjectionMatrix", &matProjection);

		// .fx���Ͽ��� sampler2D DiffuseSampler �ٷ� ���� texture ������������ ����� ������ ã��
		// ù��° �Ű������� �־��ش�.
		gpShader->SetTexture("DiffuseMap_Tex", gpTextureDM);
		// �н��� ���� ������ ���� ����
		UINT numPasses;
		// �н��� ���� ����
		gpShader->Begin(&numPasses, NULL);
		{
			// �н� ��ŭ ��ü�� �׸�.
			for (UINT i = 0;i < numPasses;++i)
			{
				gpShader->BeginPass(i);
				{
					// �𵨸� �׸���.
					gpModel->DrawSubset(0);
				}
				gpShader->EndPass();
			}
		}
		gpShader->End();
	}
}

// ����� ���� ���� ���.
void CRender::RenderInfo(ID3DXFont*& gpFont, char* info, int width, int height)
{
	// �ؽ�Ʈ ����
	D3DCOLOR fontColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	// �ؽ�Ʈ�� ����� ��ġ
	RECT rct;
	rct.left = 5;
	rct.right = width / 3;
	rct.top = 5;
	rct.bottom = height / 3;

	// Ű �Է� ������ ���
	gpFont->DrawText(NULL, info, -1, &rct, 0, fontColor);
}