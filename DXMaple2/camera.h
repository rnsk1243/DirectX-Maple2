//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: camera.h
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Defines a camera's position and orientation.
//         
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __cameraH__
#define __cameraH__

#include <d3dx9.h>



class Camera
{
public:
	enum CameraType { LANDOBJECT/*�ϴ��� �� �� ���� ���*/, AIRCRAFT/*��������*/ };

	Camera();
	// ī�޶� Ÿ������ ��ü�� ����
	Camera(CameraType cameraType);
	~Camera();
	D3DXVECTOR3 WORLD_UP = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	void strafe(float units); // ��,�� �̵�
	void fly(float units);    // ���� , �Ʒ� �̵�
	void walk(float units);   // ���� , ���� ����
	
	void pitch(float angle); // ��,�Ʒ� ȸ��
	void yaw(float angle);   // ��,�� ȸ��
	void roll(float angle);  // ��ȸ�� ȸ��??

	// �� ��� ���
	void getViewMatrix(D3DXMATRIX* V); 
	// ī�޶� Ÿ�� ����
	void setCameraType(CameraType cameraType); 
	// ������ġ ���
	void getPosition(D3DXVECTOR3* pos); 
	// ��ġ ����
	void setPosition(D3DXVECTOR3* pos); 

	// ������
	void getRight(D3DXVECTOR3* right);
	// ��
	void getUp(D3DXVECTOR3* up);
	// ����
	void getLook(D3DXVECTOR3* look);
private:
	// ī�޶� Ÿ��
	CameraType  _cameraType;
	// ���⺤�� ����ȭ�� x�� ����
	D3DXVECTOR3 _right;
	// up���� ����ȭ�� y�� ����
	D3DXVECTOR3 _up;
	// ���溤�� ����ȭ�� z�� ����
	D3DXVECTOR3 _look;
	// ī�޶��� ��ġ
	D3DXVECTOR3 _pos;
};
#endif // __cameraH__