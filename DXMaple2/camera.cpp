//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: camera.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Defines a camera's position and orientation.
//         
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "camera.h"

Camera::Camera()
{
	_cameraType = AIRCRAFT;

	_pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

Camera::Camera(CameraType cameraType)
{
	_cameraType = cameraType;

	_pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

Camera::~Camera()
{

}

void Camera::getPosition(D3DXVECTOR3* pos)
{
	*pos = _pos;
}

void Camera::setPosition(D3DXVECTOR3* pos)
{
	_pos = *pos;
}

void Camera::getRight(D3DXVECTOR3* right)
{
	*right = _right;
}

void Camera::getUp(D3DXVECTOR3* up)
{
	*up = _up;
}

void Camera::getLook(D3DXVECTOR3* look)
{
	*look = _look;
}

// ������ �ȱ�
void Camera::walk(float units)
{
	if (units == 0)
		return;
	
	// ī�޶� LANDOBJECT�����̸� ���߿� �߸� �ȵǹǷ� y������ 0.0���� �ΰ�  dir(�ٶ󺸴�����)�� x,z���и� ���� �����ǿ� �����ش�.
	if (_cameraType == LANDOBJECT)
	{
		//������ �̿��Ͽ� �ٶ󺸴� ���⺤�͸� ���Ѵ�.
		D3DXVECTOR3 dir;
		// ���� y��� ī�޶��� x�������� ��� ������ ���� z����
		D3DXVec3Cross(&dir, &_right, &WORLD_UP);
		_pos += D3DXVECTOR3(dir.x, 0.0f, dir.z) * units;
	}
		
	if( _cameraType == AIRCRAFT )
		_pos += _look * units;
}

// ������ �ȱ�
void Camera::strafe(float units)
{
	// move only on xz plane for land object
	if( _cameraType == LANDOBJECT )
		_pos += D3DXVECTOR3(_right.x, 0.0f, _right.z) * units;

	if( _cameraType == AIRCRAFT )
		_pos += _right * units;
}

// ����
void Camera::fly(float units)
{
	// move only on y-axis for land object
	if( _cameraType == LANDOBJECT )
		_pos.y += units;

	if( _cameraType == AIRCRAFT )
		_pos += _up * units;
}

// x���� �������� ȸ�� (��, �Ʒ�)����
void Camera::pitch(float angle)
{
	// ȸ�� ����� ���� ����� �����Ѵ�.
	D3DXMATRIX T;
	// _right���� �������� angle(����)��ŭ ȸ���ϴ� ����� T�� ����
	D3DXMatrixRotationAxis(&T, &_right,	angle);

	// ������ ���� ȸ�� ����� �̿��Ͽ� ��ü�� _up,_look�� ȸ����Ų��.
	D3DXVec3TransformCoord(&_up,&_up, &T);
	D3DXVec3TransformCoord(&_look,&_look, &T);
}

// y���� �������� ȸ�� (��, ��)����
void Camera::yaw(float angle)
{
	D3DXMATRIX T;

	// LANDOBJECTī�޶��� ��쿡�� ���� Y���� �������� ȸ���ϰ� �����
	// rotate around world y (0, 1, 0) always for land object
	if( _cameraType == LANDOBJECT )
		D3DXMatrixRotationY(&T, angle);

	// AIRCRAFTī�޶�� ī�޶��� ��ü _up���͸� �������� ȸ�� ����� �����.
	// rotate around own up vector for aircraft
	if( _cameraType == AIRCRAFT )
		D3DXMatrixRotationAxis(&T, &_up, angle);

	// ������ ���� ȸ�� ��Ŵ
	D3DXVec3TransformCoord(&_right,&_right, &T);
	D3DXVec3TransformCoord(&_look,&_look, &T);
}

// z���� �������� ȸ�� ��￩ ����
void Camera::roll(float angle)
{
	// z��ȸ���� ī�޶� AIRCRAFT�϶��� �۵��Ѵ�.
	if( _cameraType == AIRCRAFT )
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &_look,	angle);

		// rotate _up and _right around _look vector
		D3DXVec3TransformCoord(&_right,&_right, &T);
		D3DXVec3TransformCoord(&_up,&_up, &T);
	}
}
// ����� ���ϱ�
void Camera::getViewMatrix(D3DXMATRIX* V)
{
	// ī�޶��� ���� ���� ������ �ǵ��� �����
	// ==============================================
	// ���� _look ���Ϳ� _look�� ����ȭ���� ��´�. (���溤�� z���� _look���͸� ����ȭ��) 
	D3DXVec3Normalize(&_look, &_look);

	// _look ���Ϳ� _right ���͸� �����Ͽ� �� ���Ϳ� ��� �����ϴ� _up���͸� ���Ѵ�.
	D3DXVec3Cross(&_up, &_look, &_right);
	// ���� _up���͸� ����ȭ ��Ų��.
	D3DXVec3Normalize(&_up, &_up);

	// ������ ���� _up���Ϳ� _look���͸� �����Ͽ� �� ���� ��ο� �����ϴ� _right���͸� ���Ѵ�.
	D3DXVec3Cross(&_right, &_up, &_look);
	// ���� _right���͸� ����ȭ ��Ų��. �̷μ� ī�޶��� ��� ��ǥ���� ����ȭ ���״�.
	D3DXVec3Normalize(&_right, &_right);

	// _right, _up, _look ��� ����ȭ �������Ƿ� ������ ��ü�� �ڻ��� ���� �ȴ�.

	// _right ���Ϳ� _pos ���͸� �����Ͽ� �� ������ ������ ���ϰ� ī�޶� �������� �̵���ų ���̹Ƿ� -�� ���δ�(pos - pos = 0�̹Ƿ� -pos)
	// x����� �����̵�&ȸ������(�������� x��������� �󸶳� �������ְ� �󸶳� ȸ���ؾ� �ϴ���)
	float x = -D3DXVec3Dot(&_right, &_pos);
	// y����� �����̵�&ȸ������
	float y = -D3DXVec3Dot(&_up, &_pos);
	float z = -D3DXVec3Dot(&_look, &_pos);

	// BA = I �̹Ƿ� (�츮�� ���ϰ��� �ϴ� ������� A����̴�) (B�� ������ǥ�� �ִ� ī�޶��� x,y,z��(����ȭ��) ���)
	// A = B�� ������� �ȴ�. �̶� B����� ���� ����̹Ƿ� B����� ������� B����� ��ġ��İ� ���� 
	// ���� B����� ��ġ��ĸ� ���ϸ� A��� �� ������� ���� �� �ִ�.
	// ������ ���� ����� ��ġ��ķ� �ٲٸ� 4x4��ķ� �����
	// ����ִ� V����� (x,y) x��,y�࿡ ���� ä���.
	(*V)(0,0) = _right.x; (*V)(0, 1) = _up.x; (*V)(0, 2) = _look.x; (*V)(0, 3) = 0.0f;
	(*V)(1,0) = _right.y; (*V)(1, 1) = _up.y; (*V)(1, 2) = _look.y; (*V)(1, 3) = 0.0f;
	(*V)(2,0) = _right.z; (*V)(2, 1) = _up.z; (*V)(2, 2) = _look.z; (*V)(2, 3) = 0.0f;
	// 4�࿡���� ������ ���� ī�޶��� ������ġ�� ����ȭ�� ������ ��� ������ ������ ä���.
	(*V)(3,0) = x;        (*V)(3, 1) = y;     (*V)(3, 2) = z;       (*V)(3, 3) = 1.0f;
}

void Camera::setCameraType(CameraType cameraType)
{
	_cameraType = cameraType;
}
