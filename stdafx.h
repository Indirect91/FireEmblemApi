#pragma once

#include "Classes/targetver.h"

//��PI
#define PI 3.141592f
#define PI2 (PI * 2)
#define PI_2 (PI / 2) 
#define PI_4 (PI / 4) 
#define PI_8 (PI / 8) 

#define WIN32_LEAN_AND_MEAN
//��Windows, C���
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <assert.h>
#include <math.h>

//��C++, STL
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <functional>
#include <stack>

//����̷�Ʈ2D ���̺귯��
#pragma comment(lib, "d2d1.lib")
#include <D2D1.h>
#include <d2d1helper.h>

//����̷�Ʈ Write ���̺귯�� (�ؽ�Ʈ)
#pragma comment(lib, "dwrite.lib")
#include <dwrite.h>

//��WIC ���̺귯��
#pragma comment(lib, "WindowsCodecs.lib")
#include <wincodec.h>

//����� �ڷ���
enum class Pivot : int
{
	LeftTop, Centre, Bottom, 
};

//�峻�� ������ ���
#include "Classes/GameNode.h"
#include "Classes/CommonMacroFunction.h"
#include "Classes/StringHelper.h"
#include "Classes/D2DRenderer.h"
#include "Classes/ImageManager.h"
#include "Classes/KeyManager.h"
#include "Classes/SceneManager.h"
#include "Classes/TimeManager.h"
#include "Classes/SoundManager.h"
#include "Classes/Camera.h"
#include "Classes/Occupation.h"

//��̱��� ���ν��Ͻ�
#define D2DRENDERER D2DRenderer::Get()
#define IMAGEMANAGER ImageManager::Get()
#define KEYMANAGER KeyManager::Get()
#define SCENEMANAGER SceneManager::Get()
#define TIMEMANAGER TimeManager::Get()
#define SOUNDMANAGER SoundManager::Get()
#define CAMERA Camera::Get()

//��������
#define WINNAME			L"���ѿ� WinApi+D2D"
#define WINSTARTX		310
#define WINSTARTY		220
#define WINSIZEX		1300
#define WINSIZEY		640
#define WINSTYLE		WS_CAPTION | WS_SYSMENU

//����ó����ũ��
#define SAFE_DELETE(p)		{if(p) {delete (p); (p) = NULL;}}
#define SAFE_DELETE_ARRAY(p)	{if(p) {delete[] (p); (p) = NULL;}}
#define SAFE_RELEASE(p){if(p){(p)->Release();(p) = NULL;}}
#define SetCoverStatue SCENEMANAGER->RefAlphaStatue()=SceneManager::tagCoverStatue

//������ ����
extern HINSTANCE	_hInstance;
extern HWND			_hWnd;
extern POINT		_ptMouse;


