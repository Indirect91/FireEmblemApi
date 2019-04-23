#include "../stdafx.h"
#include "Camera.h"


Camera::Camera()
{
	cRc = { 0,0,WINSIZEX,WINSIZEY}; //일단은 윈도 사이즈만큼 만들어 출고됨
	toFollow = { WINSIZEX/2,WINSIZEY/2 }; //일단은 화면 중간으로 출고
	followSpeed = 0.5f;
}

void Camera::Smoothen()
{
	FLOAT toCalculate = GetDistance(GetRectCentre(cRc), toFollow);
	if (toCalculate < 5) 
	{ 
		cRc = RectMakeCentre(toFollow.x, toFollow.y, GetWidth(cRc), GetHeight(cRc)); 
	}
	else if (toCalculate > 10) 
	{ 
		followSpeed = 5; 

	}
	

}

RECT Camera::RelativeCameraRect(RECT _toConvert)
{
	_toConvert.left -= cRc.left; 
	_toConvert.top -= cRc.top;
	_toConvert.right -= cRc.left;
	_toConvert.bottom -= cRc.top;

	return _toConvert;
}
