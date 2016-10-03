#include "InitEverything.h"
#include"GameManager.h"
/*
Direct3D �ڿ����� ���� ���� ������ �޸� Ǯ�� ������ �� �ִ�.
D3DPOOL_DEFAULT
- ���� ������ �ڿ�(�����޸�, AGP�޸�, �ý��� �޸� ��)���� �޸𸮿� ����
- ����Ʈ Ǯ�� ������ �ڿ����� �ݵ�� IDirect3DDevice9::Reset ȣ�� ������ �ı��Ǿ�� �Ѵ�.
*/



CInitEverything::CInitEverything(){}


CInitEverything::~CInitEverything(){}
/*
bool CInitEverything::InitEverything(HWND hWnd, LPDIRECT3DDEVICE9 gpD3DDevice, ID3DXFont* gpFont, LPDIRECT3D9 gpD3D, 
	LPDIRECT3DTEXTURE9 gpEarthDM, LPD3DXEFFECT gpTextureMappingShader, LPD3DXMESH gpSphere,
	char* texture, char* shader, char* model)
{
	// D3D�� �ʱ�ȭ
	if (!InitD3D(hWnd, gpD3D, gpD3DDevice))
	{
		return false;
	}

	// ��, ���̴�, �ؽ�ó���� �ε�
	if (!LoadAssets(gpEarthDM, gpTextureMappingShader, gpSphere, texture, shader, model, gpD3DDevice))
	{
		return false;
	}

	// ��Ʈ�� �ε�
	if (FAILED(D3DXCreateFont(gpD3DDevice, 20, 10, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, (DEFAULT_PITCH | FF_DONTCARE),
		"Arial", &gpFont)))
	{
		return false;
	}

	return true;
}
*/


// D3D ��ü �� ��ġ �ʱ�ȭ
bool CInitEverything::InitD3D(HWND hWnd, LPDIRECT3D9& gpD3D, LPDIRECT3DDEVICE9& gpD3DDevice)
{
	// Step 1: Create the IDirect3D9 object.
	// Direct3D �ʱ�ȭ �ϱ� 1�ܰ�

	//IDirect3D9 �������̽����� �����͸� ��´�.
	// ���� gpD3D ������ ���ؼ� ������ �ϵ���� ��ġ�� ��Ÿ���� C++��ü�� IDirect3DDevice9 �������̽��� ����� �� �̿�ȴ�. ��) gpD3D->CreateDevice();
	// ���� gpD3D ������ ��ġ �����뵵(���,���� �� �׷�����ġ�� �����ϴ� Ư������ ��� ����)�ε� �̿�ȴ�.
	gpD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!gpD3D)
	{
		return false;
	}

	// Direct3D �ʱ�ȭ �ϱ� 2�ܰ� (�ϵ���� ���ؽ� ���μ����� �����ϴ��� �˾ƺ�)
	// Step 2: Check for hardware vp.
	// Direct3D�� �����ϴ� ��� ��ɵ��� D3DCAPS9 ����ü ���� ��Ʈ�� ������ ����� �����ȴ�.
	D3DCAPS9 caps;
	// 1�ܰ迡�� ��ġ �����뵵�ε� ���ȴٰ� �ߴµ� �̷��� d3d9������ caps�ʱ�ȭ ���ش�.
	// GetDeviceCaps(Ư���� ����� �ϴ� ���� ���÷��� �����, �̿��� ��ġ Ÿ���� ����, ����� ���Ͻ�ų����);
	gpD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	// ���ؽ����μ��� �������θ� vp������ ��µ� 4�ܰ迡�� IDirect3DDevice9 ��ü�� ���鶧 ����ϱ� ���ؼ� �̴�.
	int vp = 0;
	// �� ��ǻ���� �׷�����ġ�� ��ȯ�� ���� ����� �ϵ���������� ó���� �� �ִ��� ���� Ȯ��
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		// ������
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		// �Ұ���
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// Direct3D �ʱ�ȭ �ϱ� 3�ܰ�(D3DPRESENT_PARAMETERS ����ü �ν��Ͻ��� �ʱ�ȭ �Ѵ�)
	//D3DPRESENT_PARAMETERS ����ü�� IDirect3DDevice9 �ν��Ͻ��� Ư���� ������.
	// Step 3: Fill out the D3DPRESENT_PARAMETERS structure.

	// D3D��ġ�� �����ϴµ� �ʿ��� ����ü�� ä���ִ´�.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	// �ȼ� ������ �ĸ� ������ �ʺ�� ���� ����
	d3dpp.BackBufferWidth = WIN_WIDTH;
	d3dpp.BackBufferHeight = WIN_HEIGHT;
	// �ĸ������ �ȼ� ������ �����Ѵ�.
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8; // 32��Ʈ �ȼ� ����
	// �̿��� �ĸ� ������ ��
	d3dpp.BackBufferCount = 1;
	// ��Ƽ ���ø��� �������� �ʴ´�. 
	// ��Ƽ ���ø��� �����ϸ� ��ģ�̹����� �ε巯�� �̹����� ���� �� �ִ�. 
	// ���� ����� D3DMULTISAMPLE_1_SAMPLE ~ D3DMULTISAMPLE_16_SAMPLE ���� ������ ���� �� �� �ִ�
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	// �ø��� ü���� ���۰� ��ȯ�Ǵ� ����� ����
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// ���񽺿� ����� ������ �ڵ� ( ȭ���� �׷��� �����츦 �����Ѵ� )
	d3dpp.hDeviceWindow = hWnd;
	// â����϶��� true�� ��ü ȭ�� �϶��� false�� �����Ѵ�.
	d3dpp.Windowed = TRUE;
	// Direct3D�� �ڵ����� ����/���ٽ� ���۸� ����� �����ϱ� ���Ѵٸ� true�� �����Ѵ�.
	d3dpp.EnableAutoDepthStencil = TRUE;
	// 24��Ʈ ���� ���۸� �����ϰ� ���ٽ� ���� �����Ѵ�.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;
	// �ΰ����� Ư���� ����( �ĸ���۸� ��� �� �ְ� �ϰų� �ĸ� ���۸� �ÿ��� �ڿ� ����/���ٽ� ���۸� ���� ������ )
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	// �����Ӽ��� �����Ѵ�. ����Ʈ ������� ������.
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	// ���� �ÿ��ϰ� ����.
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	///////////////////////////////////// Ư�� ���� �� ////////////////////////////////////////////

	// Step 4: Create the device. �����
	// IDirect3D9 ��ü �����ͺ��� gpD3D�� ���ؼ� IDirect3DDevice9��ü �����Ϳ� ����̽��� ����� ����

	// D3D��ġ�� �����Ѵ�.
	// �ñ������� ����(�ʱ�ȭ��) IDirect3DDevice9 ��ý ������
	// ������ ��ġ�� ��Ÿ��
	if (FAILED(gpD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		vp,
		&d3dpp, &gpD3DDevice)))
	{
		return false;
	}

	return true;
}

// ���� �ε�
// ������ �ؽ���,���̴�,�� �����ּҿ� �ؽ���,���̴�,�� �̸�, �׸��� ��ġ�� ����.
bool CInitEverything::LoadAssets(LPDIRECT3DTEXTURE9& gpTextureDM, LPD3DXEFFECT& gpShader, LPD3DXMESH& gpModel, 
	const char* texture, const char* shader, const char* model, LPDIRECT3DDEVICE9& gpD3DDevice)
{
	// �ؽ�ó �ε�
	// �����ּҿ� �ؽ��ĸ� �ִ´�.
	gpTextureDM = LoadTexture(texture, gpD3DDevice);
	if (!gpTextureDM)
		return false;

	// ���̴� �ε�
	gpShader = LoadShader(shader, gpD3DDevice);
	if (!gpShader)
		return false;

	// �� �ε�
	gpModel = LoadModel(model, gpD3DDevice);
	if (!gpModel)
		return false;

	return true;
}

bool CInitEverything::LoadAssets(nodeObj*& obj, LPDIRECT3DDEVICE9& gpD3DDevice)
{
	// �ؽ�ó �ε�
	// �����ּҿ� �ؽ��ĸ� �ִ´�.
	obj->gpTextureDM = LoadTexture(obj->_name->texture, gpD3DDevice);
	if (!obj->gpTextureDM)
		return false;

	// ���̴� �ε�
	obj->Shader = LoadShader(obj->_name->shader, gpD3DDevice);
	if (!obj->Shader)
		return false;

	// �� �ε�
	obj->gpModel = LoadModel(obj->_name->xFile, gpD3DDevice);
	if (!obj->gpModel)
		return false;

	return true;
}

// UI�� ����� ���ڸ� �����.
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

// ���̴� �ε�
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

	// ���̴� �ε��� ������ ��� outputâ�� ���̴�
	// ������ ������ ����Ѵ�.
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

// �� �ε�
LPD3DXMESH CInitEverything::LoadModel(const char * filename, LPDIRECT3DDEVICE9& gpD3DDevice)
{
	LPD3DXMESH ret = NULL;
	if (FAILED(D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM, gpD3DDevice, NULL, NULL, NULL, NULL, &ret)))
	{
		OutputDebugString("�� �ε� ����: ");
		OutputDebugString(filename);
		OutputDebugString("\n");
	};

	return ret;
}

// �ؽ�ó �ε�
LPDIRECT3DTEXTURE9 CInitEverything::LoadTexture(const char * filename, LPDIRECT3DDEVICE9& gpD3DDevice)
{
	LPDIRECT3DTEXTURE9 ret = NULL;
	if (FAILED(D3DXCreateTextureFromFile(gpD3DDevice, filename, &ret)))
	{
		OutputDebugString("�ؽ�ó �ε� ����: ");
		OutputDebugString(filename);
		OutputDebugString("\n");
	}

	return ret;
}
