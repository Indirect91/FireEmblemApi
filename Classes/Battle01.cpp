#include "../stdafx.h"
#include "Battle01.h"
#include "Tiles.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "TurnManager.h"
#include "TileManager.h"
#include "StartPlacement.h"
#include "ESCMenu.h"
#include "Cursor.h"

Battle01::Battle01()
{
	bg = nullptr;
	bg2 = nullptr;
	bg3 = nullptr;
	bg4 = nullptr;
}

void Battle01::Init()
{
	DATACENTRE.LoadFromFile();
	//▼ESC메뉴를 만들고 UI로 등록시킴
	escMenu = new ESCMenu; //ESC UI를 뉴할당받음
	DATACENTRE.AddObj(ObjType::UI, "escMenu", escMenu); //쓸 수 있게 등록함

	//▼턴매니져를 만들고 UI로 등록시킴
	turnManager = new TurnManager; //턴관련 UI를 뉴할당받을
	DATACENTRE.AddObj(ObjType::UI, "TurnManager", turnManager); //쓸 수 있게 등록함

	SOUNDMANAGER->pause("타이틀BGM");
	SOUNDMANAGER->play("인게임BGM");


	//▼맵에 맞게 카메라를 세팅함
	CAMERA.SetCamera({ 0,0,800,800 });

	//▼커서를 만들고 이닛시킴
	cursor = new Cursor;
	cursor->Init();
	DATACENTRE.AddObj(ObjType::UI, "Cursor", cursor);

	//▼플레이어를 만들고 이닛시킴
	player = new Player;
	player->Init();

	//▼에너미를 만들고 이닛시킴
	enemy = new Enemy;
	enemy->Init();

	//▼타일매니져를 만들고 이닛을 시킴
	tileManager = new TileManager;
	tileManager->Init();

	bg = IMAGEMANAGER->AddImage("tmp", L"IMAGE/Tiles/temp.png");
	bg2 = IMAGEMANAGER->AddImage("tmp1", L"IMAGE/Tiles/temp2.png");
	bg3 = IMAGEMANAGER->AddImage("tmp2", L"IMAGE/Tiles/temp3.png");
	bg4 = IMAGEMANAGER->AddImage("tmp3", L"IMAGE/Tiles/temp4.png");

	CAMERA.Init();

	player->SetTurnStart(); //TODO:수정
}

	//▼현재 씬에서 뉴할당 받은 애들 역순으로 제거
void Battle01::Release()
{
	//▼현재 씬을 닫으려 하니, 현재 씬에서 등록했던 컨테이너 내용들 비움
	DATACENTRE.RefObjects(ObjType::UI).clear();


	//▼타일매니져 릴리즈 후 삭제
	tileManager->Release();
	SAFE_DELETE(tileManager);

	//▼커서 릴리즈 후 삭제
	cursor->Release();
	SAFE_DELETE(cursor);

	//▼에너미 릴리즈 후 삭제
	enemy->Release();
	SAFE_DELETE(enemy);
	
	//▼플레이어 릴리즈 후 삭제
	player->Release();
	SAFE_DELETE(player);

	//▼턴매니져 릴리즈 후 삭제
	turnManager->Release();
	SAFE_DELETE(turnManager);

	//▼ESC메뉴 릴리즈 후 삭제
	escMenu->Release();
	SAFE_DELETE(escMenu);
}

void Battle01::Update()
{
	ESCManage(); //ESC 메뉴를 눌렀을시 메뉴 나오는부분
	cursor->Update();
	tileManager->Update();
	
	//▼게임의 상태에 따라 업데이트 대상이 달라짐
	switch (currentState)
	{
	case BattleScenes::ingameStatus::StartPlacement:
		cursor->SetCursorTurn(Cursor::CursorTurn::UI);
		startPlacement->Update();
		break;
	case BattleScenes::ingameStatus::TurnChanging:
		cursor->SetCursorTurn(Cursor::CursorTurn::UI);
		turnManager->Update();
		break;
	case BattleScenes::ingameStatus::ESCMenu:
		cursor->SetCursorTurn(Cursor::CursorTurn::UI);
		escMenu->Update();
		break;
	case BattleScenes::ingameStatus::PlayerTurn:
		cursor->SetCursorTurn(Cursor::CursorTurn::PlayerTurn);
		player->Update();
		break;
	case BattleScenes::ingameStatus::EnemyTurn:
		cursor->SetCursorTurn(Cursor::CursorTurn::EnemyTurn);
		enemy->Update();
		break;
	}
	CAMERA.Update();
}

void Battle01::Render()
{
	IMAGEMANAGER->FindImage("BattleBg")->Render(0, 0);
	//▼랜더 순서를 결정지을 수 있음
	bg->RelativeRender(0,0);
	bg->RelativeRender(288, 0);
	//bg->RelativeRender(-288, 0);
	bg->RelativeRender(288+288, 0);
	bg->RelativeRender(288+288+288, 0);
	bg->RelativeRender(288 + 288 + 288 +288, 0);

	bg2->RelativeRender(0, bg->GetHeight());
	bg2->RelativeRender(bg2->GetWidth(), bg->GetHeight());
	bg2->RelativeRender(bg2->GetWidth() + bg2->GetWidth() + bg2->GetWidth() + bg2->GetWidth(), bg->GetHeight());
	bg2->RelativeRender(bg2->GetWidth() + bg2->GetWidth() + bg2->GetWidth(), bg->GetHeight()); 
	bg2->RelativeRender(bg2->GetWidth() + bg2->GetWidth(), bg->GetHeight()); 
	
	tileManager->Render();
	IMAGEMANAGER->FindImage("BattleUIBg")->Render(0, 0);

	player->Render();
	enemy->Render();
	cursor->Render();
}