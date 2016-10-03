#include "InitEverything.h"
#include"GameManager.h"
/*
Direct3D 자원들은 여러 가지 종류의 메모리 풀에 보관할 수 있다.
D3DPOOL_DEFAULT
- 가장 적합한 자원(비디오메모리, AGP메모리, 시스템 메모리 등)들을 메모리에 보관
- 디폴트 풀에 보관된 자원들은 반드시 IDirect3DDevice9::Reset 호출 이전에 파괴되어야 한다.
*/



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
	// Step 1: Create the IDirect3D9 object.
	// Direct3D 초기화 하기 1단계

	//IDirect3D9 인터페이스로의 포인터를 얻는다.
	// 이제 gpD3D 변수를 통해서 물리적 하드웨어 장치를 나타내는 C++객체인 IDirect3DDevice9 인터페이스를 만드는 데 이용된다. 예) gpD3D->CreateDevice();
	// 또한 gpD3D 변수는 장치 검증용도(기능,포맷 등 그래픽장치가 제공하는 특성정보 얻는 과정)로도 이용된다.
	gpD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!gpD3D)
	{
		return false;
	}

	// Direct3D 초기화 하기 2단계 (하드웨어 버텍스 프로세싱을 지원하는지 알아봄)
	// Step 2: Check for hardware vp.
	// Direct3D가 제공하는 모든 기능들은 D3DCAPS9 구조체 내의 비트와 데이터 멤버에 대응된다.
	D3DCAPS9 caps;
	// 1단계에서 장치 검증용도로도 사용된다고 했는데 이렇게 d3d9변수로 caps초기화 해준다.
	// GetDeviceCaps(특성을 얻고자 하는 물리 디스플레이 어댑터, 이용할 장치 타입을 지정, 결과를 리턴시킬변수);
	gpD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	// 버텍스프로세싱 지원여부를 vp변수에 담는데 4단계에서 IDirect3DDevice9 객체를 만들때 사용하기 위해서 이다.
	int vp = 0;
	// 내 컴퓨터의 그래픽장치가 변환과 조명 계산을 하드웨어적으로 처리할 수 있는지 여부 확인
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		// 가능함
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		// 불가능
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// Direct3D 초기화 하기 3단계(D3DPRESENT_PARAMETERS 구조체 인스턴스를 초기화 한다)
	//D3DPRESENT_PARAMETERS 구조체는 IDirect3DDevice9 인스턴스의 특성을 지정함.
	// Step 3: Fill out the D3DPRESENT_PARAMETERS structure.

	// D3D장치를 생성하는데 필요한 구조체를 채워넣는다.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	// 픽셀 단위의 후면 버퍼의 너비와 높이 지정
	d3dpp.BackBufferWidth = WIN_WIDTH;
	d3dpp.BackBufferHeight = WIN_HEIGHT;
	// 후면버퍼의 픽셀 포맷을 지정한다.
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8; // 32비트 픽셀 포맷
	// 이용할 후면 버퍼의 수
	d3dpp.BackBufferCount = 1;
	// 멀티 샘플링을 지정하지 않는다. 
	// 멀티 샘플링을 지정하면 거친이미지를 부드러운 이미지로 만들 수 있다. 
	// 지정 방법은 D3DMULTISAMPLE_1_SAMPLE ~ D3DMULTISAMPLE_16_SAMPLE 까지 레벨을 지정 할 수 있다
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	// 플리핑 체인의 버퍼가 교환되는 방법을 지정
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// 서비스와 연결된 윈도우 핸들 ( 화면이 그려질 윈도우를 지정한다 )
	d3dpp.hDeviceWindow = hWnd;
	// 창모드일때는 true를 전체 화면 일때는 false를 지정한다.
	d3dpp.Windowed = TRUE;
	// Direct3D가 자동으로 깊이/스텐실 버퍼를 만들고 관리하길 원한다면 true를 지정한다.
	d3dpp.EnableAutoDepthStencil = TRUE;
	// 24비트 깊이 버퍼를 지정하고 스텐실 버퍼 예약한다.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;
	// 부가적인 특성을 지정( 후면버퍼를 잠글 수 있게 하거나 후면 버퍼를 시연한 뒤에 깊이/스텐실 버퍼를 버릴 것인지 )
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	// 프레임수를 지정한다. 디폴트 재생율로 지정함.
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	// 언제 시연하게 될지.
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	///////////////////////////////////// 특성 지정 끝 ////////////////////////////////////////////

	// Step 4: Create the device. 만들기
	// IDirect3D9 객체 포인터변수 gpD3D을 통해서 IDirect3DDevice9객체 포인터에 디바이스를 만들어 담음

	// D3D장치를 생성한다.
	// 궁극적으로 만들(초기화할) IDirect3DDevice9 객첵 포인터
	// 물리적 장치를 나타냄
	if (FAILED(gpD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		vp,
		&d3dpp, &gpD3DDevice)))
	{
		return false;
	}

	return true;
}

// 에셋 로드
// 변수로 텍스쳐,쉐이더,모델 원본주소와 텍스쳐,쉐이더,모델 이름, 그리고 장치가 들어간다.
bool CInitEverything::LoadAssets(LPDIRECT3DTEXTURE9& gpTextureDM, LPD3DXEFFECT& gpShader, LPD3DXMESH& gpModel, 
	const char* texture, const char* shader, const char* model, LPDIRECT3DDEVICE9& gpD3DDevice)
{
	// 텍스처 로딩
	// 원본주소에 텍스쳐를 넣는다.
	gpTextureDM = LoadTexture(texture, gpD3DDevice);
	if (!gpTextureDM)
		return false;

	// 쉐이더 로딩
	gpShader = LoadShader(shader, gpD3DDevice);
	if (!gpShader)
		return false;

	// 모델 로딩
	gpModel = LoadModel(model, gpD3DDevice);
	if (!gpModel)
		return false;

	return true;
}

bool CInitEverything::LoadAssets(nodeObj*& obj, LPDIRECT3DDEVICE9& gpD3DDevice)
{
	// 텍스처 로딩
	// 원본주소에 텍스쳐를 넣는다.
	obj->gpTextureDM = LoadTexture(obj->_name->texture, gpD3DDevice);
	if (!obj->gpTextureDM)
		return false;

	// 쉐이더 로딩
	obj->Shader = LoadShader(obj->_name->shader, gpD3DDevice);
	if (!obj->Shader)
		return false;

	// 모델 로딩
	obj->gpModel = LoadModel(obj->_name->xFile, gpD3DDevice);
	if (!obj->gpModel)
		return false;

	return true;
}

// UI에 사용할 글자를 만든다.
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
