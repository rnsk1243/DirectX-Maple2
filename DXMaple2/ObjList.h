#pragma once
#include <d3d9.h>
#include <d3dx9.h>

struct nodeAssetName
{
	nodeAssetName(char* textureName, char* shaderName, char* xFileName):
		texture(textureName),shader(shaderName),xFile(xFileName)
	{
	
	}
	const char* texture;
	const char* shader;
	const char* xFile;
};

struct nodeObj
{
	nodeObj(LPDIRECT3DTEXTURE9 TextureDM, LPD3DXEFFECT shader, LPD3DXMESH Model, nodeAssetName* name)
	{
		gpTextureDM = TextureDM;
		Shader = shader;
		gpModel = Model;
		_name = name;
		next = nullptr;
	}
	nodeObj(nodeAssetName* name)
	{
		gpTextureDM = NULL;
		Shader = NULL;
		gpModel = NULL;
		_name = name;
		next = nullptr;
	}
	// 텍스쳐
	LPDIRECT3DTEXTURE9 gpTextureDM;
	// 쉐이더
	LPD3DXEFFECT Shader;
	// 모델
	LPD3DXMESH gpModel;
	// 에셋 이름
	nodeAssetName* _name;
	// 다음 노드를 가리키는 포인터
	nodeObj* next;
};

class CObjList
{
	nodeObj* m_head;
	int m_count;
public:
	CObjList();
	~CObjList();
	void pushFront(LPDIRECT3DTEXTURE9 gpTextureDM, LPD3DXEFFECT Shader, LPD3DXMESH gpModel, nodeAssetName* name)
	{ 
		insertObject(0, gpTextureDM, Shader, gpModel, name); 
	}
	void pushFront(nodeObj* obj)
	{
		insertObject(0, obj->gpTextureDM, obj->Shader, obj->gpModel, obj->_name);
	}
	nodeObj* getObj(int pos);
	void insertObject(int pos, LPDIRECT3DTEXTURE9 gpTextureDM, LPD3DXEFFECT Shader, LPD3DXMESH gpModel, nodeAssetName* name);
	void removeObject(int pos);
	bool isEmpty() { return m_head == nullptr; }
	int getCount() { return m_count; }
};

