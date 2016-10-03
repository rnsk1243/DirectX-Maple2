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

// 3D 물체등을 그린다. 
void CRender::RenderScene(LPD3DXEFFECT& gpShader, LPD3DXMESH& gpModel, LPDIRECT3DTEXTURE9& gpTextureDM)
{
	

	
	// .fx파일에서 sampler2D DiffuseSampler 바로 위에 texture 데이터형으로 선언된 변수를 찾아
	// 첫번째 매개변수로 넣어준다.
	gpShader->SetTexture("DiffuseMap_Tex", gpTextureDM);
	// 패스의 수를 저장할 변수 선언
	UINT numPasses;
	// 패스의 수를 구함
	gpShader->Begin(&numPasses, NULL);
	{
		// 패스 만큼 물체를 그림.
		for (UINT i = 0;i < numPasses;++i)
		{
			gpShader->BeginPass(i);
			{
				// 모델를 그린다.
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

		// .fx파일에서 sampler2D DiffuseSampler 바로 위에 texture 데이터형으로 선언된 변수를 찾아
		// 첫번째 매개변수로 넣어준다.
		gpShader->SetTexture("DiffuseMap_Tex", gpTextureDM);
		// 패스의 수를 저장할 변수 선언
		UINT numPasses;
		// 패스의 수를 구함
		gpShader->Begin(&numPasses, NULL);
		{
			// 패스 만큼 물체를 그림.
			for (UINT i = 0;i < numPasses;++i)
			{
				gpShader->BeginPass(i);
				{
					// 모델를 그린다.
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
	// 투영행렬을 만든다.
	// 부영행렬을 만들 곧
	D3DXMATRIXA16 matProjection;
	// 투영행렬을 만들어주는 함수 호출
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

		// .fx파일에서 sampler2D DiffuseSampler 바로 위에 texture 데이터형으로 선언된 변수를 찾아
		// 첫번째 매개변수로 넣어준다.
		gpShader->SetTexture("DiffuseMap_Tex", gpTextureDM);
		// 패스의 수를 저장할 변수 선언
		UINT numPasses;
		// 패스의 수를 구함
		gpShader->Begin(&numPasses, NULL);
		{
			// 패스 만큼 물체를 그림.
			for (UINT i = 0;i < numPasses;++i)
			{
				gpShader->BeginPass(i);
				{
					// 모델를 그린다.
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
	// 광원의 위치
	D3DXVECTOR4				gWorldLightPosition = D3DXVECTOR4(500.0f, 500.0f, 500.0f, 1.0f);
	// 표면의 색상
	D3DXVECTOR4				gSurfaceColor = D3DXVECTOR4(1, 0.49804, 0, 1);

	// 투영행렬을 만든다.
	// 부영행렬을 만들 곧
	D3DXMATRIXA16 matProjection;
	// 투영행렬을 만들어주는 함수 호출
	D3DXMatrixPerspectiveFovLH(&matProjection, FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);

	// 월드 행렬 좌표
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
			// 월드행렬의 역행렬을 구한다.
			D3DXMATRIXA16 matInvWorld;
			D3DXMatrixTranspose(&matInvWorld, &matWorld);

			// 월드/뷰/투영행렬을 미리 곱한다.
			D3DXMATRIXA16 matWorldView;
			
			D3DXMatrixMultiply(&matWorldView, &matWorld, View);
			D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProjection);

			// 쉐이더 전역변수들을 설정
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

		// .fx파일에서 sampler2D DiffuseSampler 바로 위에 texture 데이터형으로 선언된 변수를 찾아
		// 첫번째 매개변수로 넣어준다.
		gpShader->SetTexture("DiffuseMap_Tex", gpTextureDM);
		// 패스의 수를 저장할 변수 선언
		UINT numPasses;
		// 패스의 수를 구함
		gpShader->Begin(&numPasses, NULL);
		{
			// 패스 만큼 물체를 그림.
			for (UINT i = 0;i < numPasses;++i)
			{
				gpShader->BeginPass(i);
				{
					// 모델를 그린다.
					gpModel->DrawSubset(0);
				}
				gpShader->EndPass();
			}
		}
		gpShader->End();
	}
}

// 디버그 정보 등을 출력.
void CRender::RenderInfo(ID3DXFont*& gpFont, char* info, int width, int height)
{
	// 텍스트 색상
	D3DCOLOR fontColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	// 텍스트를 출력할 위치
	RECT rct;
	rct.left = 5;
	rct.right = width / 3;
	rct.top = 5;
	rct.bottom = height / 3;

	// 키 입력 정보를 출력
	gpFont->DrawText(NULL, info, -1, &rct, 0, fontColor);
}