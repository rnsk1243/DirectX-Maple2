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

// 앞으로 걷기
void Camera::walk(float units)
{
	if (units == 0)
		return;
	
	// 카메라가 LANDOBJECT상태이면 공중에 뜨면 안되므로 y성분은 0.0으로 두고  dir(바라보느방향)의 x,z성분만 현재 포지션에 더해준다.
	if (_cameraType == LANDOBJECT)
	{
		//외적을 이용하여 바라보는 방향벡터를 구한다.
		D3DXVECTOR3 dir;
		// 월드 y축과 카메라의 x방위벡터 모두 수직인 방향 z벡터
		D3DXVec3Cross(&dir, &_right, &WORLD_UP);
		_pos += D3DXVECTOR3(dir.x, 0.0f, dir.z) * units;
	}
		
	if( _cameraType == AIRCRAFT )
		_pos += _look * units;
}

// 옆으로 걷기
void Camera::strafe(float units)
{
	// move only on xz plane for land object
	if( _cameraType == LANDOBJECT )
		_pos += D3DXVECTOR3(_right.x, 0.0f, _right.z) * units;

	if( _cameraType == AIRCRAFT )
		_pos += _right * units;
}

// 날기
void Camera::fly(float units)
{
	// move only on y-axis for land object
	if( _cameraType == LANDOBJECT )
		_pos.y += units;

	if( _cameraType == AIRCRAFT )
		_pos += _up * units;
}

// x축을 기준으로 회전 (위, 아래)보기
void Camera::pitch(float angle)
{
	// 회전 행렬을 받을 행렬을 선언한다.
	D3DXMATRIX T;
	// _right축을 기준으로 angle(라디안)만큼 회전하는 행렬을 T에 저장
	D3DXMatrixRotationAxis(&T, &_right,	angle);

	// 위에서 구한 회전 행렬을 이용하여 물체의 _up,_look을 회전시킨다.
	D3DXVec3TransformCoord(&_up,&_up, &T);
	D3DXVec3TransformCoord(&_look,&_look, &T);
}

// y축을 기준으로 회전 (좌, 우)보기
void Camera::yaw(float angle)
{
	D3DXMATRIX T;

	// LANDOBJECT카메라의 경우에는 월드 Y축을 기준으로 회전하게 만든다
	// rotate around world y (0, 1, 0) always for land object
	if( _cameraType == LANDOBJECT )
		D3DXMatrixRotationY(&T, angle);

	// AIRCRAFT카메라는 카메라의 자체 _up벡터를 기준으로 회전 행렬을 만든다.
	// rotate around own up vector for aircraft
	if( _cameraType == AIRCRAFT )
		D3DXMatrixRotationAxis(&T, &_up, angle);

	// 나머지 축을 회전 시킴
	D3DXVec3TransformCoord(&_right,&_right, &T);
	D3DXVec3TransformCoord(&_look,&_look, &T);
}

// z축을 기준으로 회전 기울여 보기
void Camera::roll(float angle)
{
	// z축회전은 카메라가 AIRCRAFT일때만 작동한다.
	if( _cameraType == AIRCRAFT )
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &_look,	angle);

		// rotate _up and _right around _look vector
		D3DXVec3TransformCoord(&_right,&_right, &T);
		D3DXVec3TransformCoord(&_up,&_up, &T);
	}
}
// 뷰행렬 구하기
void Camera::getViewMatrix(D3DXMATRIX* V)
{
	// 카메라의 축이 서로 직각이 되도록 만들기
	// ==============================================
	// 먼저 _look 벡터에 _look을 정규화시켜 담는다. (전방벡터 z축을 _look벡터를 정규화함) 
	D3DXVec3Normalize(&_look, &_look);

	// _look 벡터와 _right 벡터를 외적하여 두 벡터에 모두 직교하는 _up벡터를 구한다.
	D3DXVec3Cross(&_up, &_look, &_right);
	// 구한 _up벡터를 정규화 시킨다.
	D3DXVec3Normalize(&_up, &_up);

	// 위에서 구한 _up벡터와 _look벡터를 내적하여 두 벡터 모두에 직교하는 _right벡터를 구한다.
	D3DXVec3Cross(&_right, &_up, &_look);
	// 구한 _right벡터를 정규화 시킨다. 이로서 카메라의 모든 좌표축을 정규화 시켰다.
	D3DXVec3Normalize(&_right, &_right);

	// _right, _up, _look 모두 정규화 시켰으므로 내적값 자체가 코사인 값이 된다.

	// _right 벡터와 _pos 벡터를 내적하여 두 벡터의 내각을 구하고 카메라를 원점으로 이동시킬 것이므로 -를 붙인다(pos - pos = 0이므로 -pos)
	// x축방향 원점이동&회전각량(원점에서 x축방향으로 얼마나 떨어져있고 얼마나 회전해야 하는지)
	float x = -D3DXVec3Dot(&_right, &_pos);
	// y축방향 원점이동&회전각량
	float y = -D3DXVec3Dot(&_up, &_pos);
	float z = -D3DXVec3Dot(&_look, &_pos);

	// BA = I 이므로 (우리가 구하고자 하는 뷰행렬이 A행렬이다) (B는 월드좌표에 있는 카메라의 x,y,z축(정규화된) 행렬)
	// A = B의 역행렬이 된다. 이때 B행렬은 직교 행렬이므로 B행렬의 역행렬은 B행렬의 전치행렬과 같다 
	// 따라서 B행렬의 전치행렬를 구하면 A행렬 즉 뷰행렬을 구할 수 있다.
	// 위에서 구한 행렬을 전치행렬로 바꾸며 4x4행렬로 만들기
	// 비어있는 V행렬을 (x,y) x행,y행에 값을 채운다.
	(*V)(0,0) = _right.x; (*V)(0, 1) = _up.x; (*V)(0, 2) = _look.x; (*V)(0, 3) = 0.0f;
	(*V)(1,0) = _right.y; (*V)(1, 1) = _up.y; (*V)(1, 2) = _look.y; (*V)(1, 3) = 0.0f;
	(*V)(2,0) = _right.z; (*V)(2, 1) = _up.z; (*V)(2, 2) = _look.z; (*V)(2, 3) = 0.0f;
	// 4행에서는 위에서 구한 카메라의 현재위치와 정규화된 각각의 축과 내적한 값으로 채운다.
	(*V)(3,0) = x;        (*V)(3, 1) = y;     (*V)(3, 2) = z;       (*V)(3, 3) = 1.0f;
}

void Camera::setCameraType(CameraType cameraType)
{
	_cameraType = cameraType;
}
