#include "Render.h"



CRender::CRender()
{
}


CRender::~CRender()
{
}

// 3D 물체등을 그린다. 
void CRender::RenderScene(LPD3DXEFFECT& gpShader, LPD3DXMESH& gpModel, LPDIRECT3DTEXTURE9& gpTextureDM)
{
	/*
	//뷰행렬을 만든다
	// 뷰행렬을 만들 곳
	//D3DXMATRIXA16 matView;
	// 카메라의 현재 위치 정함
	//D3DXVECTOR3 vEyePt(0.0f, 0.0f, -200.0f);
	// 카메라가 바라보는 위치 정함
	//D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	// 카메라의 위쪽을 가리키는 벡터
	//D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	// 뷰행렬을 만들어주는 함수 호출
	//D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);

	// 투영행렬을 만든다.
	// 부영행렬을 만들 곧
	D3DXMATRIXA16 matProjection;
	// 투영행렬을 만들어주는 함수 호출
	D3DXMatrixPerspectiveFovLH(&matProjection, FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);

	// 프레임마다 0.2도씩 회전을 시킨다.
	gRotationY += 0.2f * PI / 180.0f; //(0.4f * 2PI / 360.0f;)
	if (gRotationY > 2 * PI)
	{
		gRotationY -= 2 * PI;
	}

	// 월드행렬을 만든다.
	// 뷰행령,투영행렬과 달리 각 물체마다 월드행렬을 만들어줘야함.(각 물체마다 위치,크기가 다르므로)
	// 여기서는 월드의 원점(0,0,0)에 물체를 놓아둔다고 가정하므로 단위행렬로 놔둠.
	D3DXMATRIXA16 matWorld;
	// 단위행렬
	//D3DXMatrixIdentity(&matWorld);
	// gRotationY 회전각 만큼 돌린 월드좌표를 matWorld에 저장함.
	D3DXMatrixRotationY(&matWorld, gRotationY);

	// 셰이더 전역변수들을 설정
	// SetMatrix() 함수를 이용하여 뷰행렬,투영행렬,월드행렬을 셰이더에 전달해준다.
	// SetMatrix(렌더몽키에서 전역변수로 선언했던 변수이름과 완전히 일치해야함, 각종행렬)
	gpTextureMappingShader->SetMatrix("gWorldMatrix", &matWorld);
	gpTextureMappingShader->SetMatrix("gViewMatrix", &matView);
	gpTextureMappingShader->SetMatrix("gProjectionMatrix", &matProjection);
	// .fx파일에서 sampler2D DiffuseSampler 바로 위에 texture 데이터형으로 선언된 변수를 찾아
	// 첫번째 매개변수로 넣어준다.
	gpTextureMappingShader->SetTexture("DiffuseMap_Tex", gpEarthDM);
	// 셰이더를 시작한다.
	// pass는 다양한 셰이더를 이용하여 동일한 물체를 여러번 그릴때 유용
	// Begin함수를 호출할때 numPasses변수의 
	// 주소를 전달하여 셰이더 안에 들어 있는 패스의 수(대부분 1)를 구해 온다.

	*/
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