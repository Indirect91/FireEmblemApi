#include "../stdafx.h"
#include "LoadingScene.h"
#include "TitleScene.h"
#include "MapToolScene.h"
#include "testScene.h"
#include "WorldScene.h"
#include "Battle01.h"
#include "SaveloadScene.h"

void LoadingScene::Init()
{
	//▼로딩클래스 뉴할당 해주고 초기화
	_loading = new Loading;
	_loading->Init();

	//▼이미지 사운드 씬 로딩
	this->LoadingImage();
	this->LoadingSound();
	this->LoadScenes();

}

void LoadingScene::Release()
{
	//로딩클래스 뉴할당 해준적 있으니 해제
	_loading->Release();
	SAFE_DELETE(_loading);
}

//▼업데이트 한번에 하나의 아이템 로딩함. 로딩서 움직이는 이미지 추가해줘도 되고, 로딩씬서 해줘도 되고.
void LoadingScene::Update()
{
	_loading->Update(); //로딩클래스 애들 업데이트

	if (_loading->LoadingDone()) //이프 조건문을 확인하는 동시에 그 아래 있는 함수들까지 실행됨
	{
		//▼로딩이 끝나고 이동할 씬
		SCENEMANAGER->RefcoverAlpha() = 1.f;
		//SCENEMANAGER->LoadScene("test");
		//SCENEMANAGER->LoadScene("Battle01");
		SCENEMANAGER->LoadScene("TitleScene");
	}
}

//▼퍼센테이지 화면에 그려줌. 
void LoadingScene::Render()
{
	float num = _loading->GetLoadItem().size();
	float cur = _loading->GetCurrentGauge();
	per = cur / num * 100;

	std::wstring str = std::to_wstring(per);
	str += L" %";
	D2DRENDERER->RenderText(WINSIZEX/2, WINSIZEY/2, (std::wstring)str, str.size(), D2DRenderer::DefaultBrush::White);

}

//▼원래 사용하던 이미지 로딩방법
void LoadingScene::LoadingImage()
{
	//▼커서
	_loading->LoadImg("Cursor", L"IMAGE/ETC/cursor.png");
	_loading->LoadImg("CursorRed", L"IMAGE/ETC/cursorRed.png");
	_loading->LoadImg("CursorBlue", L"IMAGE/ETC/cursorBlue.png");
	

	//▼색
	_loading->LoadImg("Blue", L"IMAGE/ETC/BLUE.png");
	_loading->LoadImg("Yellow", L"IMAGE/ETC/YELLOW.png");
	_loading->LoadImg("Green", L"IMAGE/ETC/GREEN.png");
	_loading->LoadImg("Red", L"IMAGE/ETC/RED.png");

	//▼맵툴용 
	_loading->LoadImg("MapToolBg1",		L"IMAGE/MapToolScene/MapToolBg1.png");
	_loading->LoadImg("MapToolBg2",		L"IMAGE/MapToolScene/MapToolBg2.png");
	_loading->LoadImg("MapToolTitle",	L"IMAGE/MapToolScene/MaptoolTitle.png");
	_loading->LoadImg("MapToolMapbox",	L"IMAGE/MapToolScene/MaptoolMapbox2.png");
	_loading->LoadImg("MapToolMat2",	L"IMAGE/MapToolScene/MaptoolMat2.png");
	_loading->LoadImg("oldpaper",		L"IMAGE/MapToolScene/oldpaper.png");
	
	//▼타이틀용 
	_loading->LoadImg("타이틀백그라운드",		L"IMAGE/TitleScene/TitleBackground.png");
	_loading->LoadImg("타이틀선택박스",		L"IMAGE/TitleScene/TitlePick.png");
	_loading->LoadImg("타이틀텍스트",			L"IMAGE/TitleScene/TitleLogo.png");
	_loading->LoadFrameImage("타이틀팔시온",	L"IMAGE/TitleScene/TitleFalchion.png", 34, 1);

	//▼캐릭터 프레임이미지
	_loading->LoadFrameImage("캐릭터Chrome", L"IMAGE/Characters/Chrome.png", 12, 128);
	_loading->LoadFrameImage("캐릭터Virion", L"IMAGE/Characters/Virion.png", 12, 144);
	_loading->LoadFrameImage("캐릭터Anna", L"IMAGE/Characters/Anna.png", 12, 144);
	_loading->LoadFrameImage("캐릭터Tharja", L"IMAGE/Characters/Tharja.png", 12, 128);
	_loading->LoadFrameImage("캐릭터Olivia", L"IMAGE/Characters/Olivia.png", 12, 128);
	_loading->LoadFrameImage("캐릭터Lissa", L"IMAGE/Characters/Lissa.png", 12, 128);
	
	//▼캐릭터 초상화
	_loading->LoadImg("초상화Chrome", L"IMAGE/Portraits/Chrome.png");
	_loading->LoadImg("초상화Virion", L"IMAGE/Portraits/Virion.png");
	_loading->LoadImg("초상화Anna", L"IMAGE/Portraits/Anna.png");
	_loading->LoadImg("초상화Tharja", L"IMAGE/Portraits/Tharja.png");
	_loading->LoadImg("초상화Olivia", L"IMAGE/Portraits/Olivia.png");
	_loading->LoadImg("초상화Lissa", L"IMAGE/Portraits/Lissa.png");

	//▼타일용
	_loading->LoadFrameImage("타일커서", L"IMAGE/Tiles/SelectedTile.png", 4, 1);
	_loading->LoadFrameImage("MoveArrow", L"IMAGE/Tiles/MoveArrow.png", 4, 4);

	//UI용
	_loading->LoadImg("ActionTaken", L"IMAGE/ETC/ActionTaken.png");

}

//▼원래 사용하던 로딩방법
void LoadingScene::LoadingSound()
{
	//▼맵툴용
	_loading->LoadSound("맵툴BGM", "SOUND/FE/MapTool_Road Taken.mp3", true, true);
	//▼타이틀용
	_loading->LoadSound("타이틀BGM", "SOUND/FE/TitleBgm.mp3", true, true);
}

//▼로딩방법
void LoadingScene::LoadScenes()
{
	_loading->LoadScene("TitleScene",new TitleScene);
	_loading->LoadScene("MapToolScene", new MapToolScene);
	_loading->LoadScene("test", new testScene);
	_loading->LoadScene("WorldScene", new WorldScene);
	_loading->LoadScene("Battle01", new Battle01);
	_loading->LoadScene("SaveloadScene", new SaveloadScene);
}
