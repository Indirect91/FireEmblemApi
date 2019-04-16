#include "../stdafx.h"
#include "Camera.h"


Camera::Camera()
{
	cRc = { 0,0,WINSIZEX,WINSIZEY};
	//GetClientRect(_hWnd, &clientRc); 
}

RECT Camera::RelativeCameraRect(RECT _toConvert)
{
	_toConvert.left -= cRc.left; 
	_toConvert.top -= cRc.top;
	_toConvert.right -= cRc.left;
	_toConvert.bottom -= cRc.top;

	return _toConvert;
}
