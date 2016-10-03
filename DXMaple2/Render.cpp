#include "Render.h"



CRender::CRender()
{
}


CRender::~CRender()
{
}

// 3D ��ü���� �׸���. 
void CRender::RenderScene(LPD3DXEFFECT& gpShader, LPD3DXMESH& gpModel, LPDIRECT3DTEXTURE9& gpTextureDM)
{
	/*
	//������� �����
	// ������� ���� ��
	//D3DXMATRIXA16 matView;
	// ī�޶��� ���� ��ġ ����
	//D3DXVECTOR3 vEyePt(0.0f, 0.0f, -200.0f);
	// ī�޶� �ٶ󺸴� ��ġ ����
	//D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	// ī�޶��� ������ ����Ű�� ����
	//D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	// ������� ������ִ� �Լ� ȣ��
	//D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);

	// ��������� �����.
	// �ο������ ���� ��
	D3DXMATRIXA16 matProjection;
	// ��������� ������ִ� �Լ� ȣ��
	D3DXMatrixPerspectiveFovLH(&matProjection, FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);

	// �����Ӹ��� 0.2���� ȸ���� ��Ų��.
	gRotationY += 0.2f * PI / 180.0f; //(0.4f * 2PI / 360.0f;)
	if (gRotationY > 2 * PI)
	{
		gRotationY -= 2 * PI;
	}

	// ��������� �����.
	// �����,������İ� �޸� �� ��ü���� ��������� ����������.(�� ��ü���� ��ġ,ũ�Ⱑ �ٸ��Ƿ�)
	// ���⼭�� ������ ����(0,0,0)�� ��ü�� ���Ƶдٰ� �����ϹǷ� ������ķ� ����.
	D3DXMATRIXA16 matWorld;
	// �������
	//D3DXMatrixIdentity(&matWorld);
	// gRotationY ȸ���� ��ŭ ���� ������ǥ�� matWorld�� ������.
	D3DXMatrixRotationY(&matWorld, gRotationY);

	// ���̴� ������������ ����
	// SetMatrix() �Լ��� �̿��Ͽ� �����,�������,��������� ���̴��� �������ش�.
	// SetMatrix(������Ű���� ���������� �����ߴ� �����̸��� ������ ��ġ�ؾ���, �������)
	gpTextureMappingShader->SetMatrix("gWorldMatrix", &matWorld);
	gpTextureMappingShader->SetMatrix("gViewMatrix", &matView);
	gpTextureMappingShader->SetMatrix("gProjectionMatrix", &matProjection);
	// .fx���Ͽ��� sampler2D DiffuseSampler �ٷ� ���� texture ������������ ����� ������ ã��
	// ù��° �Ű������� �־��ش�.
	gpTextureMappingShader->SetTexture("DiffuseMap_Tex", gpEarthDM);
	// ���̴��� �����Ѵ�.
	// pass�� �پ��� ���̴��� �̿��Ͽ� ������ ��ü�� ������ �׸��� ����
	// Begin�Լ��� ȣ���Ҷ� numPasses������ 
	// �ּҸ� �����Ͽ� ���̴� �ȿ� ��� �ִ� �н��� ��(��κ� 1)�� ���� �´�.

	*/
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