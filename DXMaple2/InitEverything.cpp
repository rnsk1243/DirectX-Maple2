#include "InitEverything.h"



CInitEverything::CInitEverything(){}


CInitEverything::~CInitEverything(){}
/*
bool CInitEverything::InitEverything(HWND hWnd, LPDIRECT3DDEVICE9 gpD3DDevice, ID3DXFont* gpFont, LPDIRECT3D9 gpD3D, 
	LPDIRECT3DTEXTURE9 gpEarthDM, LPD3DXEFFECT gpTextureMappingShader, LPD3DXMESH gpSphere,
	char* texture, char* shader, char* model)
{
	// D3D를 초기화
	if (!InitD3D(hWnd, gpD3D, gpD3DDevice))
	{
		return false;
	}

	// 모델, 쉐이더, 텍스처등을 로딩
	if (!LoadAssets(gpEarthDM, gpTextureMappingShader, gpSphere, texture, shader, model, gpD3DDevice))
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
*/


// D3D 객체 및 장치 초기화
bool CInitEverything::InitD3D(HWND hWnd, LPDIRECT3D9& gpD3D, LPDIRECT3DDEVICE9& gpD3DDevice)
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

	d3dpp.BackBufferWidth = 800;
	d3dpp.BackBufferHeight = 600;
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

bool CInitEverything::LoadAssets(LPDIRECT3DTEXTURE9& gpEarthDM, LPD3DXEFFECT& gpTextureMappingShader, LPD3DXMESH& gpSphere, 
	const char* texture, const char* shader, const char* model, LPDIRECT3DDEVICE9& gpD3DDevice)
{
	// 텍스처 로딩
	gpEarthDM = LoadTexture(texture, gpD3DDevice);
	if (!gpEarthDM)
		return false;

	// 쉐이더 로딩
	gpTextureMappingShader = LoadShader(shader, gpD3DDevice);
	if (!gpTextureMappingShader)
		return false;

	// 모델 로딩
	gpSphere = LoadModel(model, gpD3DDevice);
	if (!gpSphere)
		return false;

	return true;
}

bool CInitEverything::LoadFont(LPDIRECT3DDEVICE9& gpD3DDevice, ID3DXFont*& gpFont)
{
	if (FAILED(D3DXCreateFont(gpD3DDevice, 20, 10, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, (DEFAULT_PITCH | FF_DONTCARE),
		"Arial", &gpFont)))
	{
		return false;
	}
	return true;
}

// 쉐이더 로딩
LPD3DXEFFECT CInitEverything::LoadShader(const char * filename, LPDIRECT3DDEVICE9& gpD3DDevice)
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
LPD3DXMESH CInitEverything::LoadModel(const char * filename, LPDIRECT3DDEVICE9& gpD3DDevice)
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
LPDIRECT3DTEXTURE9 CInitEverything::LoadTexture(const char * filename, LPDIRECT3DDEVICE9& gpD3DDevice)
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
