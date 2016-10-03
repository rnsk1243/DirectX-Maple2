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
	/*
	// �����Ӹ��� 0.2���� ȸ���� ��Ų��.
	gRotationY += 0.2f * PI / 180.0f; //(0.4f * 2PI / 360.0f;)
	if (gRotationY > 2 * PI)
	{
		gRotationY -= 2 * PI;
	}
	*/

	// ��������� �����.
	// �����,������İ� �޸� �� ��ü���� ��������� ����������.(�� ��ü���� ��ġ,ũ�Ⱑ �ٸ��Ƿ�)
	// ���⼭�� ������ ����(0,0,0)�� ��ü�� ���Ƶдٰ� �����ϹǷ� ������ķ� ����.
	D3DXMATRIXA16 matWorld;
	// �������
	D3DXMatrixIdentity(&matWorld);
	// gRotationY ȸ���� ��ŭ ���� ������ǥ�� matWorld�� ������.
	//D3DXMatrixRotationY(&matWorld, gRotationY);

	// ���̴� ������������ ����
	// SetMatrix() �Լ��� �̿��Ͽ� �����,�������,��������� ���̴��� �������ش�.
	// SetMatrix(������Ű���� ���������� �����ߴ� �����̸��� ������ ��ġ�ؾ���, �������)
	
	m_Shader->SetMatrix("gWorldMatrix", &matWorld);
	m_Shader->SetMatrix("gViewMatrix", &m_matView);
	m_Shader->SetMatrix("gProjectionMatrix", &matProjection);
	// .fx���Ͽ��� sampler2D DiffuseSampler �ٷ� ���� texture ������������ ����� ������ ã��
	// ù��° �Ű������� �־��ش�.
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
		// ������
		if (::GetAsyncKeyState('W') & 0x8000f)
			m_camera.walk(CAMERA_SPEED * deltaTime);
		// �ڷ�
		if (::GetAsyncKeyState('S') & 0x8000f)
			m_camera.walk(-CAMERA_SPEED * deltaTime);
		// ����
		if (::GetAsyncKeyState('A') & 0x8000f)
			m_camera.strafe(-CAMERA_SPEED * deltaTime);
		// ������
		if (::GetAsyncKeyState('D') & 0x8000f)
			m_camera.strafe(CAMERA_SPEED * deltaTime);
		// �������
		if (::GetAsyncKeyState('R') & 0x8000f)
			m_camera.fly(CAMERA_SPEED * deltaTime);
		// �ϰ�
		if (::GetAsyncKeyState('F') & 0x8000f)
			m_camera.fly(-CAMERA_SPEED * deltaTime);
		// ���� �÷��� ����
		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			m_camera.pitch(-CAMERA_R * deltaTime);
		// �Ʒ��� ���� ����
		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			m_camera.pitch(CAMERA_R * deltaTime);
		// ���� ����
		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			m_camera.yaw(-CAMERA_R * deltaTime);
		// ������ ����
		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			m_camera.yaw(CAMERA_R * deltaTime);
		// ���� ȸ��
		if (::GetAsyncKeyState('N') & 0x8000f)
			m_camera.roll(CAMERA_R * deltaTime);
		// ������ ȸ��
		if (::GetAsyncKeyState('M') & 0x8000f)
			m_camera.roll(-CAMERA_R * deltaTime);

		// Update the view matrix representing the cameras 
		// new position/orientation.
		//D3DXMATRIX V;
		// ����� ����
		m_camera.getViewMatrix(&m_matView);
		// ����� ����
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
	// ��Ʈ�� release �Ѵ�.
	if (m_gpFont)
	{
		m_gpFont->Release();
		m_gpFont = NULL;
	}

	// ���� release �Ѵ�.
	if (m_gpModel)
	{
		m_gpModel->Release();
		m_gpModel = NULL;
	}

	// ���̴��� release �Ѵ�.
	if (m_Shader)
	{
		m_Shader->Release();
		m_Shader = NULL;
	}

	// �ؽ�ó�� release �Ѵ�.
	if (m_gpTextureDM)
	{
		m_gpTextureDM->Release();
		m_gpTextureDM = NULL;
	}

	// D3D�� release �Ѵ�.
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
	D3DCOLOR bgColour = 0xFF0000FF;	// ������ - �Ķ�

	m_gpD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), bgColour, 1.0f, 0);

	m_gpD3DDevice->BeginScene();
	{
		LoadMap();
		LoadUI();
	}
	m_gpD3DDevice->EndScene();

	m_gpD3DDevice->Present(NULL, NULL, NULL, NULL);
}
