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
		newNode->next = m_head; // null�ְ�
		m_head = newNode; //null�ִ������� ���� ���� �ּ� ����
	}
	else
	{
		for (int i = 0; i <= pos - 1; i++)
		{
			// �־��� ��ġ���� �̵�
			curNode = curNode->next;
		}
		// ����Ű�� ��� ���� (���� �ֱ�)
		newNode->next = curNode->next;
		curNode->next = newNode;
	}
	m_count++;
}

void CObjList::removeObject(int pos)
{
	//pos�� 0�̸� ��尪�� �ٲ�ϱ� 0�϶� ���� ��.
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
		delNode = curNode->next;//����Ű�� �ϰ� ���� ���� �־��.
		curNode->next = delNode->next;
	}
	delete delNode;
	m_count--;
}
