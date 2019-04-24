#include "../stdafx.h"
#include "Camera.h"
#include "Cursor.h"
#include "Tiles.h"

//▼생성땐 초기화만, 사용때 의미있는 값 넣음
Camera::Camera()
{
	cRc = { 0,0,WINSIZEX,WINSIZEY }; //일단은 윈도 사이즈만큼 만들어 출고됨
	toFollow = { WINSIZEX / 2,WINSIZEY / 2 }; //일단은 화면 중간으로 출고
	followSpeed = 0.5f;
	maxFollowSpd = 10;
	prevRc = cRc;
	cStatus = CameraStatus::Idle;
	cursor = nullptr;
	maxDistance = 100;
	minDistance = 5;
	adjustFourPoints = { -80,-80,TILECOLX*TILESIZE + 80, TILEROWY * TILESIZE +TILESIZE + 180};
}

//▼카메라 이닛. 현재 활성화된 커서를 넣음
void Camera::Init()
{
	cursor = dynamic_cast<Cursor*>(DATACENTRE.GetCertainObject(ObjType::UI, "Cursor"));
}

//▼카메라 업뎃
void Camera::Update()
{
	//▼움직일 대상을 보고 카메라의 상태를 정함
	if (!PointCompare(toFollow, GetRectCentre(cRc)))
	{
		cStatus = CameraStatus::Moving;
	}
	else
	{
		cStatus = CameraStatus::Idle;
	}



	//▼주어진 상태에 따라 행동
	switch (cStatus)
	{
	case Camera::CameraStatus::Idle:
		break;
	case Camera::CameraStatus::Moving:
		SmoothenCamera();	//부드러운 이동은 이동시에만 실행
		AdjustCamera();		//보정코드는 이동시에만 실행
		break;
	}
}

//▼부드러운 이동 업데이트. 대상과의 거리가 가까울수록 느리게 움직이고 멀수록 빠르게 움직임
void Camera::SmoothenCamera()
{
	POINT cCentre = GetRectCentre(cRc); //카메라의 센터점 구함
	FLOAT toCalculate = GetDistance(cCentre, toFollow); //먼저 대상까지의 거리 계산
	FLOAT angle = GetAngle(cCentre, toFollow); //카메라 센터점부터 따라다니려는 대상까지의 각도 구함

	if (toCalculate < minDistance) //만약 상대와의 거리가 너무 작다면, 계산 대신 대입연산 실행
	{
		cRc = RectMakeCentre(toFollow.x, toFollow.y, GetWidth(cRc), GetHeight(cRc)); //렉트 고정
		followSpeed = 0; //이동속도 0처리
	}
	else //너무 작은 거리가 아니라면
	{
		if (toCalculate > maxDistance) //거리가 너무 멀면 카메라 최대 속도로 이동
		{
			followSpeed = maxFollowSpd;
		}
		else //감속이 시작해야 할 땐
		{
			//따라다니는 속도는 남은 거리에 비례해서 달라지게 함
			followSpeed = (maxFollowSpd/(maxDistance - minDistance)) * (toCalculate - minDistance);
		}

		cCentre.x += cosf(angle) * followSpeed;		//계산이 다 끝난 후 이동 시작
		cCentre.y += -sinf(angle) * followSpeed;	//계산이 다 끝난 후 이동 시작
	}
	cRc = RectMakeCentre(cCentre.x, cCentre.y, GetWidth(cRc), GetHeight(cRc)); //이동된 포인트로 렉트 만듦
}

//▼카메라 최대치 보정
void Camera::AdjustCamera()
{
	UINT width = GetWidth(cRc);
	UINT height = GetHeight(cRc);

	if (cRc.left < adjustFourPoints.left)
	{
		cRc.left = adjustFourPoints.left;
		cRc.right = cRc.left + width;
	}
	else if (cRc.right > adjustFourPoints.right)
	{
		cRc.right = adjustFourPoints.right;
		cRc.left = cRc.right - width;
	}
	if (cRc.top < adjustFourPoints.top)
	{
		cRc.top = adjustFourPoints.top;
		cRc.bottom = cRc.top + height;
	}
	else if (cRc.bottom > adjustFourPoints.bottom)
	{
		cRc.bottom = adjustFourPoints.bottom;
		cRc.top = cRc.bottom - height;
	}
}



//▼카메라에 상대적인 렉트
RECT Camera::RelativeCameraRect(RECT _toConvert)
{
	_toConvert.left -= cRc.left;
	_toConvert.top -= cRc.top;
	_toConvert.right -= cRc.left;
	_toConvert.bottom -= cRc.top;

	return _toConvert;
}

//▼카메라에 상대적인 포인트
POINT Camera::RelativeCameraPoint(POINT _toConvert)
{
	return {_toConvert.x - cRc.left, _toConvert.y - cRc.top };
}
