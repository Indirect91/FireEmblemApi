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
#include "UserInteface.h"
#include "SelectionUI.h"
#include "ExecuteBattle.h"

Battle01::Battle01()
{
	bg = nullptr;
}

void Battle01::Init()
{
	//▼윈도우 및 커버 초기화
	SCENEMANAGER->RefCoverSpeed() = 0.035f;
	SetCoverStatue::FadeIn;

	//▼음악 초기화
	SOUNDMANAGER->pause("타이틀BGM");
	SOUNDMANAGER->play("인게임BGM",0.6);
	//SOUNDMANAGER->pause("SelectionTitleComplete");
	//SOUNDMANAGER->setFadeIn();


	DATACENTRE.LoadFromFile();
	//▼ESC메뉴를 만들고 UI로 등록시킴
	escMenu = new ESCMenu; //ESC UI를 뉴할당받음
	DATACENTRE.AddObj(ObjType::UI, "escMenu", escMenu); //쓸 수 있게 등록함

	//▼커서를 만들고 이닛시킴
	cursor = new Cursor;
	cursor->Init();
	DATACENTRE.AddObj(ObjType::UI, "Cursor", cursor);

	//▼턴매니져를 만들고 UI로 등록시킴
	turnManager = new TurnManager; //턴관련 UI를 뉴할당받을
	turnManager->Init();
	DATACENTRE.AddObj(ObjType::UI, "TurnManager", turnManager); //쓸 수 있게 등록함


	//▼맵에 맞게 카메라를 세팅함
	CAMERA.SetCamera({ 0,0,800,800 });



	//▼전투 매니져를 만들고 등록시킴
	battleManager = new ExecuteBattle;	//전투매니져
	battleManager->Init();
	DATACENTRE.AddObj(ObjType::Battle, "BattleManager", battleManager);

	//▼플레이어를 만들고 이닛시킴
	player = new Player;
	player->Init();

	//▼에너미를 만들고 이닛시킴
	enemy = new Enemy;
	enemy->Init();

	//▼타일매니져를 만들고 이닛을 시킴
	tileManager = new TileManager;
	tileManager->Init();

	//▼통상 UI 제작
	ingameUI = new UserInteface;
	ingameUI->Init();

	//▼선택 UI 제작
	selectionUI = new SelectionUI;
	selectionUI->Init();
	DATACENTRE.AddObj(ObjType::UI, "SelectionUI", selectionUI);

	bg = IMAGEMANAGER->AddImage("tmp", L"IMAGE/Tiles/temp.png");

	CAMERA.Init();

	player->SetTurnStart(); //TODO:수정
	cursor->SetCursorTurn(IngameStatus::TurnChanging);
	gameoverInit = false;
	gameEndReady = false;
	gameOverPhase = 0;
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
	ingameUI->Update();
	
	checkDeadUnit();
	checkTurnEnd();

	if (DATACENTRE.RefObjects(ObjType::PlayerArmy).size() == 0 || (DATACENTRE.RefObjects(ObjType::PlayerArmy).size() == 1 && dynamic_cast<Character*>((*(DATACENTRE.RefObjects(ObjType::PlayerArmy).begin())).second)->GetStatus()==Character::CharStatus::IsDying))
	{
		cursor->SetCursorTurn(IngameStatus::GameOver);

	}
	else if (DATACENTRE.RefObjects(ObjType::EnemyArmy).size() == 0 || (DATACENTRE.RefObjects(ObjType::PlayerArmy).size() == 1 && dynamic_cast<Character*>((*(DATACENTRE.RefObjects(ObjType::EnemyArmy).begin())).second)->GetStatus() == Character::CharStatus::IsDying))
	{
		cursor->SetCursorTurn(IngameStatus::PlayerWon);
	}

	

	//▼커서에게 누구턴인지 물어봄
	currentState = cursor->GetCursorTurn();


	//▼게임의 상태에 따라 업데이트 대상이 달라짐
	switch (currentState)
	{
	case IngameStatus::StartPlacement:
		startPlacement->Update();
		break;
	case IngameStatus::TurnChanging:
		turnManager->Update();
		break;
	case IngameStatus::ExecutingBattle:
		battleManager->Update();
		break;
	case IngameStatus::ESCMenu:
		escMenu->Update();
		break;
	case IngameStatus::PlayerTurn:
		player->Update();
		break;
	case IngameStatus::EnemyTurn:
		enemy->Update();
		break;
	case IngameStatus::SelectionUI:
		selectionUI->Update();
		break;

	case IngameStatus::GameOver:
	case IngameStatus::PlayerWon:

		switch (gameOverPhase)
		{
		case 0:
			SetCoverStatue::FadeOut;
			SOUNDMANAGER->FadeOut();
			gameOverPhase++;
			break;
		case 1:
			if (SCENEMANAGER->RefAlphaStatue() == SceneManager::tagCoverStatue::Idle)
			{
				gameEndReady = true;
				SetCoverStatue::FadeIn;
				gameOverPhase++;
			}
			break;
		case 2:
			if (SCENEMANAGER->RefAlphaStatue() == SceneManager::tagCoverStatue::Idle)
			{
				if (KEYMANAGER->IsOnceKeyDown('A') || KEYMANAGER->IsOnceKeyDown('S'))
				{
					SOUNDMANAGER->pause("인게임BGM");
					SOUNDMANAGER->FadeIn();

					SCENEMANAGER->LoadScene("TitleScene");
				}
			}
			break;
		}

		break;
	default:
		assert(!"인게임status 미작성된거 있음");
		break;

	}
	CAMERA.Update();
}

void Battle01::Render()
{
	if (currentState == IngameStatus::GameOver && gameEndReady)
	{
		IMAGEMANAGER->FindImage("GameOver")->Render(WINSIZEX / 2, WINSIZEY / 2, Pivot::Centre);
	}
	else if (currentState == IngameStatus::PlayerWon && gameEndReady)
	{
		IMAGEMANAGER->FindImage("PlayerWon")->Render(WINSIZEX / 2, WINSIZEY / 2, Pivot::Centre);
	}
	else
	{
		IMAGEMANAGER->FindImage("BattleBg")->Render(0, 0);
		//▼랜더 순서를 결정지을 수 있음
		bg->RelativeRender(0, 0);
		bg->RelativeRender(384, 0);
		bg->RelativeRender(384 + 384, 0);
		bg->RelativeRender(384 + 384 + 384, 0);
		bg->RelativeRender(384 + 384 + 384 + 384, 0);
		bg->RelativeRender(384 + 384 + 384 + 384 + 384, 0);
		bg->RelativeRender(0, 480);
		bg->RelativeRender(384, 480);
		bg->RelativeRender(384 + 384, 480);
		bg->RelativeRender(384 + 384 + 384, 480);
		bg->RelativeRender(384 + 384 + 384 + 384, 480);
		bg->RelativeRender(384 + 384 + 384 + 384 + 384, 480);


		
		tileManager->Render();

		player->Render();
		enemy->Render();
		cursor->Render();
		if ((currentState != IngameStatus::GameOver) || (currentState != IngameStatus::PlayerWon))
		{
			turnManager->Render();
		}
		if (currentState == IngameStatus::SelectionUI)
		{
			selectionUI->Render();
		}
		battleManager->Render();
		ingameUI->Render();

	}
}