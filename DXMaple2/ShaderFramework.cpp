//**********************************************************************
//
// ShaderFramework.cpp
// 
// 쉐이더 데모를 위한 C스타일의 초간단 프레임워크입니다.
// (실제 게임을 코딩하실 때는 절대 이렇게 프레임워크를
// 작성하시면 안됩니다. -_-)
//
// Author: Pope Kim
//
//**********************************************************************

#include "ShaderFramework.h"
#include <stdio.h>
#include<time.h>
#include"camera.h"
#include<Windows.h>

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

//----------------------------------------------------------------------
// 전역변수
//----------------------------------------------------------------------

// 카메라 객체 생성
Camera TheCamera(Camera::LANDOBJECT);

// D3D 관련
LPDIRECT3D9             gpD3D = NULL;				// D3D
LPDIRECT3DDEVICE9       gpD3DDevice = NULL;			// D3D 장치

// 폰트
ID3DXFont*              gpFont = NULL;


// 모델
LPD3DXMESH gpSphere = NULL;

// 쉐이더
LPD3DXEFFECT gpTextureMappingShader = NULL;

// 텍스처
LPDIRECT3DTEXTURE9 gpEarthDM = NULL;
// 프로그램 이름
const char*				gAppName = "초간단 쉐이더 데모 프레임워크";

// 회전 값
float gRotationY = 0.0f;

// 시간변화량
int static timeDelta = 0;

//-----------------------------------------------------------------------
// 프로그램 진입점/메시지 루프
//-----------------------------------------------------------------------

// 진입점
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	// 윈도우 클래스를 등록한다.
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		gAppName, NULL };
	RegisterClassEx(&wc);

	// 프로그램 창을 생성한다.
	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	HWND hWnd = CreateWindow(gAppName, gAppName,
		style, CW_USEDEFAULT, 0, WIN_WIDTH, WIN_HEIGHT,
		GetDesktopWindow(), NULL, wc.hInstance, NULL);

	// Client Rect 크기가 WIN_WIDTH, WIN_HEIGHT와 같도록 크기를 조정한다.
	POINT ptDiff;
	RECT rcClient, rcWindow;

	GetClientRect(hWnd, &rcClient);
	GetWindowRect(hWnd, &rcWindow);
	ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
	ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
	MoveWindow(hWnd, rcWindow.left, rcWindow.top, WIN_WIDTH + ptDiff.x, WIN_HEIGHT + ptDiff.y, TRUE);

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	// D3D를 비롯한 모든 것을 초기화한다.
	if (!InitEverything(hWnd))
		PostQuitMessage(1);

	// 메시지 루프
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else // 메시지가 없으면 게임을 업데이트하고 장면을 그린다
		{
			PlayDemo();
		}
	}

	UnregisterClass(gAppName, wc.hInstance);
	return 0;
}

// 메시지 처리기
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		ProcessInput(hWnd, wParam);
		break;

	case WM_DESTROY:
		Cleanup();
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// 키보드 입력처리
void ProcessInput(HWND hWnd, WPARAM keyPress)
{
	switch (keyPress)
	{
		// ESC 키가 눌리면 프로그램을 종료한다.
	case VK_ESCAPE:
		PostMessage(hWnd, WM_DESTROY, 0L, 0L);
		break;
	}
}

//------------------------------------------------------------
// 게임루프
//------------------------------------------------------------
void PlayDemo()
{
	Update();
	RenderFrame();
}

// 게임로직 업데이트
void Update()
{
	
	int curTime = clock();

	if (gpD3DDevice)
	{
		//
		// Update: Update the camera.
		//
		OutputDebugString("메시지\n");

		if (::GetAsyncKeyState('W') & 0x8000f)
			TheCamera.walk(4.0f * timeDelta);

		if (::GetAsyncKeyState('S') & 0x8000f)
			TheCamera.walk(-4.0f * timeDelta);

		if (::GetAsyncKeyState('A') & 0x8000f)
			TheCamera.strafe(-4.0f * timeDelta);

		if (::GetAsyncKeyState('D') & 0x8000f)
			TheCamera.strafe(4.0f * timeDelta);

		if (::GetAsyncKeyState('R') & 0x8000f)
			TheCamera.fly(4.0f * timeDelta);

		if (::GetAsyncKeyState('F') & 0x8000f)
			TheCamera.fly(-4.0f * timeDelta);

		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			TheCamera.pitch(1.0f * timeDelta);

		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			TheCamera.pitch(-1.0f * timeDelta);

		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			TheCamera.yaw(-1.0f * timeDelta);

		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			TheCamera.yaw(1.0f * timeDelta);

		if (::GetAsyncKeyState('N') & 0x8000f)
			TheCamera.roll(1.0f * timeDelta);

		if (::GetAsyncKeyState('M') & 0x8000f)
			TheCamera.roll(-1.0f * timeDelta);

		// Update the view matrix representing the cameras 
		// new position/orientation.
		D3DXMATRIX V;
		TheCamera.getViewMatrix(&V);
		gpD3DDevice->SetTransform(D3DTS_VIEW, &V);

		timeDelta = clock() - curTime;



	}
}

//------------------------------------------------------------
// 렌더링
//------------------------------------------------------------

void RenderFrame()
{
	D3DCOLOR bgColour = 0xFF0000FF;	// 배경색상 - 파랑

	gpD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), bgColour, 1.0f, 0);

	gpD3DDevice->BeginScene();
	{
		//OutputDebugString("메시지\n");
		RenderScene();				// 3D 물체등을 그린다.
		RenderInfo();				// 디버그 정보 등을 출력한다.
	}
	gpD3DDevice->EndScene();

	gpD3DDevice->Present(NULL, NULL, NULL, NULL);
}


// 3D 물체등을 그린다.
void RenderScene()
{
	//뷰행렬을 만든다
	// 뷰행렬을 만들 곳
	D3DXMATRIXA16 matView;
	// 카메라의 현재 위치 정함
	D3DXVECTOR3 vEyePt(0.0f, 0.0f, -200.0f);
	// 카메라가 바라보는 위치 정함
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	// 카메라의 위쪽을 가리키는 벡터
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	// 뷰행렬을 만들어주는 함수 호출
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);

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

	// 패스의 수를 저장할 변수 선언
	UINT numPasses;
	// 패스의 수를 구함
	gpTextureMappingShader->Begin(&numPasses, NULL);
	{
		// 패스 만큼 물체를 그림.
		for (UINT i = 0;i < numPasses;++i)
		{
			gpTextureMappingShader->BeginPass(i);
			{
				// 구체를 그린다.
				gpSphere->DrawSubset(0);
			}
			gpTextureMappingShader->EndPass();
		}
	}
	gpTextureMappingShader->End();
}

// 디버그 정보 등을 출력.
void RenderInfo()
{
	// 텍스트 색상
	D3DCOLOR fontColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	// 텍스트를 출력할 위치
	RECT rct;
	rct.left = 5;
	rct.right = WIN_WIDTH / 3;
	rct.top = 5;
	rct.bottom = WIN_HEIGHT / 3;

	// 키 입력 정보를 출력
	gpFont->DrawText(NULL, "데모 프레임워크\n\nESC: 데모종료", -1, &rct, 0, fontColor);
}



//------------------------------------------------------------
// 초기화 코드
//------------------------------------------------------------
bool InitEverything(HWND hWnd)
{
	// D3D를 초기화
	if (!InitD3D(hWnd))
	{
		return false;
	}

	// 모델, 쉐이더, 텍스처등을 로딩
	if (!LoadAssets())
	{
		return false;
	}

	// 폰트를 로딩
	if (FAILED(D3DXCreateFont(gpD3DDevice, 20, 10, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, (DEFAULT_PITCH | FF_DONTCARE),
		"Arial", &gpFont)))
	{
		return false;
	}

	return true;
}

// D3D 객체 및 장치 초기화
bool InitD3D(HWND hWnd)
{
	// D3D 객체
	gpD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!gpD3D)
	{
		return false;
	}

	// D3D장치를 생성하는데 필요한 구조체를 채워넣는다.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = WIN_WIDTH;
	d3dpp.BackBufferHeight = WIN_HEIGHT;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	// D3D장치를 생성한다.
	if (FAILED(gpD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &gpD3DDevice)))
	{
		return false;
	}

	return true;
}

bool LoadAssets()
{
	// 텍스처 로딩
	gpEarthDM = LoadTexture("house.png");
	if (!gpEarthDM)
		return false;

	// 쉐이더 로딩
	gpTextureMappingShader = LoadShader("TextureMapping.fx");
	if (!gpTextureMappingShader)
		return false;

	// 모델 로딩
	gpSphere = LoadModel("house_non_left.x");
	if (!gpSphere)
		return false;

	return true;
}

// 쉐이더 로딩
LPD3DXEFFECT LoadShader(const char * filename)
{
	LPD3DXEFFECT ret = NULL;

	LPD3DXBUFFER pError = NULL;
	DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

	D3DXCreateEffectFromFile(gpD3DDevice, filename,
		NULL, NULL, dwShaderFlags, NULL, &ret, &pError);

	// 쉐이더 로딩에 실패한 경우 output창에 쉐이더
	// 컴파일 에러를 출력한다.
	if (!ret && pError)
	{
		int size = pError->GetBufferSize();
		void *ack = pError->GetBufferPointer();

		if (ack)
		{
			char* str = new char[size];
			sprintf(str, (const char*)ack, size);
			OutputDebugString(str);
			delete[] str;
		}
	}

	return ret;
}

// 모델 로딩
LPD3DXMESH LoadModel(const char * filename)
{
	LPD3DXMESH ret = NULL;
	if (FAILED(D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM, gpD3DDevice, NULL, NULL, NULL, NULL, &ret)))
	{
		OutputDebugString("모델 로딩 실패: ");
		OutputDebugString(filename);
		OutputDebugString("\n");
	};

	return ret;
}

// 텍스처 로딩
LPDIRECT3DTEXTURE9 LoadTexture(const char * filename)
{
	LPDIRECT3DTEXTURE9 ret = NULL;
	if (FAILED(D3DXCreateTextureFromFile(gpD3DDevice, filename, &ret)))
	{
		OutputDebugString("텍스처 로딩 실패: ");
		OutputDebugString(filename);
		OutputDebugString("\n");
	}

	return ret;
}
//------------------------------------------------------------
// 뒷정리 코드.
//------------------------------------------------------------

void Cleanup()
{
	// 폰트를 release 한다.
	if (gpFont)
	{
		gpFont->Release();
		gpFont = NULL;
	}

	// 모델을 release 한다.
	if (gpSphere)
	{
		gpSphere->Release();
		gpSphere = NULL;
	}

	// 쉐이더를 release 한다.
	if (gpTextureMappingShader)
	{
		gpTextureMappingShader->Release();
		gpTextureMappingShader = NULL;
	}

	// 텍스처를 release 한다.
	if (gpEarthDM)
	{
		gpEarthDM->Release();
		gpEarthDM = NULL;
	}
	
	// D3D를 release 한다.
	if (gpD3DDevice)
	{
		gpD3DDevice->Release();
		gpD3DDevice = NULL;
	}

	if (gpD3D)
	{
		gpD3D->Release();
		gpD3D = NULL;
	}
}

