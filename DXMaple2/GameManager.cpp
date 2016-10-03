#include "GameManager.h"
#include<Windows.h>
#include <stdio.h>
#include<time.h>
#include <limits>


CGameManager::~CGameManager()
{
}

bool CGameManager::LoadMap()
{
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
	/*
	// 프레임마다 0.2도씩 회전을 시킨다.
	gRotationY += 0.2f * PI / 180.0f; //(0.4f * 2PI / 360.0f;)
	if (gRotationY > 2 * PI)
	{
		gRotationY -= 2 * PI;
	}
	*/

	// 월드행렬을 만든다.
	// 뷰행령,투영행렬과 달리 각 물체마다 월드행렬을 만들어줘야함.(각 물체마다 위치,크기가 다르므로)
	// 여기서는 월드의 원점(0,0,0)에 물체를 놓아둔다고 가정하므로 단위행렬로 놔둠.
	D3DXMATRIXA16 matWorld;
	// 단위행렬
	D3DXMatrixIdentity(&matWorld);
	// gRotationY 회전각 만큼 돌린 월드좌표를 matWorld에 저장함.
	//D3DXMatrixRotationY(&matWorld, gRotationY);

	// 셰이더 전역변수들을 설정
	// SetMatrix() 함수를 이용하여 뷰행렬,투영행렬,월드행렬을 셰이더에 전달해준다.
	// SetMatrix(렌더몽키에서 전역변수로 선언했던 변수이름과 완전히 일치해야함, 각종행렬)
	
	m_Shader->SetMatrix("gWorldMatrix", &matWorld);
	m_Shader->SetMatrix("gViewMatrix", &m_matView);
	m_Shader->SetMatrix("gProjectionMatrix", &matProjection);
	// .fx파일에서 sampler2D DiffuseSampler 바로 위에 texture 데이터형으로 선언된 변수를 찾아
	// 첫번째 매개변수로 넣어준다.
	m_Shader->SetTexture("DiffuseMap_Tex", m_gpTextureDM);

	m_render.RenderScene(m_Shader, m_gpModel, m_gpTextureDM);

	return false;
}

bool CGameManager::LoadUI()
{
	m_render.RenderInfo(m_gpFont, "MapleStory2", WIN_HEIGHT, WIN_HEIGHT);
	return false;
}

void CGameManager::CameraUpdate(float deltaTime)
{


	if (m_gpD3DDevice)
	{
		// 앞으로
		if (::GetAsyncKeyState('W') & 0x8000f)
			m_camera.walk(CAMERA_SPEED * deltaTime);
		// 뒤로
		if (::GetAsyncKeyState('S') & 0x8000f)
			m_camera.walk(-CAMERA_SPEED * deltaTime);
		// 왼쪽
		if (::GetAsyncKeyState('A') & 0x8000f)
			m_camera.strafe(-CAMERA_SPEED * deltaTime);
		// 오른쪽
		if (::GetAsyncKeyState('D') & 0x8000f)
			m_camera.strafe(CAMERA_SPEED * deltaTime);
		// 수직상승
		if (::GetAsyncKeyState('R') & 0x8000f)
			m_camera.fly(CAMERA_SPEED * deltaTime);
		// 하강
		if (::GetAsyncKeyState('F') & 0x8000f)
			m_camera.fly(-CAMERA_SPEED * deltaTime);
		// 위로 올려다 보기
		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			m_camera.pitch(-CAMERA_R * deltaTime);
		// 아래로 내려 보기
		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			m_camera.pitch(CAMERA_R * deltaTime);
		// 왼쪽 보기
		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			m_camera.yaw(-CAMERA_R * deltaTime);
		// 오른쪽 보기
		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			m_camera.yaw(CAMERA_R * deltaTime);
		// 왼쪽 회전
		if (::GetAsyncKeyState('N') & 0x8000f)
			m_camera.roll(CAMERA_R * deltaTime);
		// 오른쪽 회전
		if (::GetAsyncKeyState('M') & 0x8000f)
			m_camera.roll(-CAMERA_R * deltaTime);

		// Update the view matrix representing the cameras 
		// new position/orientation.
		//D3DXMATRIX V;
		// 뷰행렬 저장
		m_camera.getViewMatrix(&m_matView);
		// 뷰행렬 적용
		m_gpD3DDevice->SetTransform(D3DTS_VIEW, &m_matView);
	}
}

void CGameManager::Update()
{
	static float lastTime = (float)clock();

	RenderFrame();

	float currTime = (float)clock();
	float timeDelta = (currTime - lastTime)*0.001f;
	
	CameraUpdate(timeDelta);
	
	lastTime = currTime;
}

void CGameManager::Cleanup()
{
	// 폰트를 release 한다.
	if (m_gpFont)
	{
		m_gpFont->Release();
		m_gpFont = NULL;
	}

	// 모델을 release 한다.
	if (m_gpModel)
	{
		m_gpModel->Release();
		m_gpModel = NULL;
	}

	// 쉐이더를 release 한다.
	if (m_Shader)
	{
		m_Shader->Release();
		m_Shader = NULL;
	}

	// 텍스처를 release 한다.
	if (m_gpTextureDM)
	{
		m_gpTextureDM->Release();
		m_gpTextureDM = NULL;
	}

	// D3D를 release 한다.
	if (m_gpD3DDevice)
	{
		m_gpD3DDevice->Release();
		m_gpD3DDevice = NULL;
	}

	if (m_gpD3D)
	{
		m_gpD3D->Release();
		m_gpD3D = NULL;
	}
}

void CGameManager::RenderFrame()
{
	D3DCOLOR bgColour = 0xFF0000FF;	// 배경색상 - 파랑

	m_gpD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), bgColour, 1.0f, 0);

	m_gpD3DDevice->BeginScene();
	{
		LoadMap();
		LoadUI();
	}
	m_gpD3DDevice->EndScene();

	m_gpD3DDevice->Present(NULL, NULL, NULL, NULL);
}
