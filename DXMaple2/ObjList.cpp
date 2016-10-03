#include "ObjList.h"



CObjList::CObjList():m_head(nullptr), m_count(0)
{
}


CObjList::~CObjList()
{
}

void CObjList::insertObject(int pos, LPDIRECT3DTEXTURE9 gpTextureDM, LPD3DXEFFECT Shader, LPD3DXMESH gpModel, nodeAssetName * name)
{
	nodeObj* curNode = m_head;
	if (pos<0 || pos > m_count)return;

	nodeObj* newNode = new nodeObj(gpTextureDM, Shader, gpModel, name);


	if (pos == 0 || m_head == nullptr)
	{
		newNode->next = m_head; // null넣고
		m_head = newNode; //null있던데에다 새로 만든 주소 넣음
	}
	else
	{
		for (int i = 0; i <= pos - 1; i++)
		{
			// 넣어줄 위치까지 이동
			curNode = curNode->next;
		}
		// 가리키는 노드 변경 (끼워 넣기)
		newNode->next = curNode->next;
		curNode->next = newNode;
	}
	m_count++;
}

void CObjList::removeObject(int pos)
{
	//pos가 0이면 헤드값이 바뀌니까 0일때 따로 함.
	nodeObj* curNode = m_head;
	nodeObj* delNode;
	if (isEmpty())return;
	if (pos<0 || pos > m_count - 1)return;
	if (pos == 0)
	{
		delNode = m_head;
		m_head = m_head->next;
	}
	else
	{
		for (int i = 0; i < pos - 1; i++)
			curNode = curNode->next;
		delNode = curNode->next;//가리키게 하고 싶은 것을 넣어라.
		curNode->next = delNode->next;
	}
	delete delNode;
	m_count--;
}
