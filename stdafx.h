#pragma once

#include "Classes/targetver.h"

//▼PI
#define PI 3.141592f
#define PI2 (PI * 2)
#define PI_2 (PI / 2) 
#define PI_4 (PI / 4) 
#define PI_8 (PI / 8) 

#define WIN32_LEAN_AND_MEAN
//▼Windows, C헤더
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <assert.h>
#include <math.h>

//▼C++, STL
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <functional>
#include <stack>

//▼다이렉트2D 라이브러리
#pragma comment(lib, "d2d1.lib")
#include <D2D1.h>
#include <d2d1helper.h>

//▼다이렉트 Write 라이브러리 (텍스트)
#pragma comment(lib, "dwrite.lib")
#include <dwrite.h>

//▼WIC 라이브러리
#pragma comment(lib, "WindowsCodecs.lib")
#include <wincodec.h>

//▼내가 생성한 공용 자료형
enum class Pivot : UINT
{
	LeftTop, Centre, Bottom
};

enum class ObjType : UINT
{
	Tile, 
	PlayerArmy, 
	CurrentPlayerArmy, 
	PlayerItem, 
	EnemyArmy, 
	CurrentEnemyArmy, 
	UI, 
	Gold, 
	ObjSize
};

//▼내가 생성한 헤더
#include "Classes/GameNode.h"
#include "Classes/CommonMacroFunction.h"
#include "Classes/StringHelper.h"
#include "Classes/D2DRenderer.h"
#include "Classes/ImageManager.h"
#include "Classes/KeyManager.h"
#include "Classes/SceneManager.h"
#include "Classes/TimeManager.h"
#include "Classes/DataCentre.h"
#include "Classes/SoundManager.h"
#include "Classes/Camera.h"



//▼싱글톤 겟인스턴스
#define D2DRENDERER D2DRenderer::Getinstance()
#define IMAGEMANAGER ImageManager::Getinstance()
#define KEYMANAGER KeyManager::Getinstance()
#define SCENEMANAGER SceneManager::Getinstance()
#define TIMEMANAGER TimeManager::Getinstance()
#define SOUNDMANAGER SoundManager::Getinstance()
#define CAMERA Camera::Getinstance()
#define DATACENTRE DataCentre::Getinstance()

//▼윈도우
#define WINNAME			L"신한영 WinApi+D2D"
#define WINSTARTX		310
#define WINSTARTY		220
#define WINSIZEX		1300
#define WINSIZEY		640
#define WINSTYLE		WS_CAPTION | WS_SYSMENU

//▼전처리매크로
#define SAFE_DELETE(p)		{if(p) {delete (p); (p) = NULL;}}
#define SAFE_DELETE_ARRAY(p)	{if(p) {delete[] (p); (p) = NULL;}}
#define SAFE_RELEASE(p){if(p){(p)->Release();(p) = NULL;}}
#define SetCoverStatue SCENEMANAGER->RefAlphaStatue()=SceneManager::tagCoverStatue

//▼전역 변수
extern HINSTANCE	_hInstance;
extern HWND			_hWnd;
extern POINT		_ptMouse;

