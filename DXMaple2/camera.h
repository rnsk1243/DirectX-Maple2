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
	enum CameraType { LANDOBJECT/*하늘을 날 수 없는 모드*/, AIRCRAFT/*자유시점*/ };

	Camera();
	// 카메라 타입으로 객체를 생성
	Camera(CameraType cameraType);
	~Camera();
	D3DXVECTOR3 WORLD_UP = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	void strafe(float units); // 좌,우 이동
	void fly(float units);    // 위쪽 , 아래 이동
	void walk(float units);   // 앞쪽 , 뒤쪽 이족
	
	void pitch(float angle); // 위,아래 회전
	void yaw(float angle);   // 좌,우 회전
	void roll(float angle);  // 고개회전 회전??

	// 뷰 행렬 얻기
	void getViewMatrix(D3DXMATRIX* V); 
	// 카메라 타입 변경
	void setCameraType(CameraType cameraType); 
	// 현재위치 얻기
	void getPosition(D3DXVECTOR3* pos); 
	// 위치 변경
	void setPosition(D3DXVECTOR3* pos); 

	// 오른쪽
	void getRight(D3DXVECTOR3* right);
	// 위
	void getUp(D3DXVECTOR3* up);
	// 보기
	void getLook(D3DXVECTOR3* look);
private:
	// 카메라 타입
	CameraType  _cameraType;
	// 우향벡터 정규화된 x축 벡터
	D3DXVECTOR3 _right;
	// up벡터 정규화된 y축 벡터
	D3DXVECTOR3 _up;
	// 전방벡터 정규화된 z축 벡터
	D3DXVECTOR3 _look;
	// 카메라의 위치
	D3DXVECTOR3 _pos;
};
#endif // __cameraH__