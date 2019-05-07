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
	//�������� �� Ŀ�� �ʱ�ȭ
	SCENEMANAGER->RefCoverSpeed() = 0.035f;
	SetCoverStatue::FadeIn;

	//������ �ʱ�ȭ
	SOUNDMANAGER->pause("Ÿ��ƲBGM");
	SOUNDMANAGER->play("�ΰ���BGM",0.6);
	//SOUNDMANAGER->pause("SelectionTitleComplete");
	//SOUNDMANAGER->setFadeIn();


	DATACENTRE.LoadFromFile();
	//��ESC�޴��� ����� UI�� ��Ͻ�Ŵ
	escMenu = new ESCMenu; //ESC UI�� ���Ҵ����
	DATACENTRE.AddObj(ObjType::UI, "escMenu", escMenu); //�� �� �ְ� �����

	//��Ŀ���� ����� �̴ֽ�Ŵ
	cursor = new Cursor;
	cursor->Init();
	DATACENTRE.AddObj(ObjType::UI, "Cursor", cursor);

	//���ϸŴ����� ����� UI�� ��Ͻ�Ŵ
	turnManager = new TurnManager; //�ϰ��� UI�� ���Ҵ����
	turnManager->Init();
	DATACENTRE.AddObj(ObjType::UI, "TurnManager", turnManager); //�� �� �ְ� �����


	//��ʿ� �°� ī�޶� ������
	CAMERA.SetCamera({ 0,0,800,800 });



	//������ �Ŵ����� ����� ��Ͻ�Ŵ
	battleManager = new ExecuteBattle;	//�����Ŵ���
	battleManager->Init();
	DATACENTRE.AddObj(ObjType::Battle, "BattleManager", battleManager);

	//���÷��̾ ����� �̴ֽ�Ŵ
	player = new Player;
	player->Init();

	//�忡�ʹ̸� ����� �̴ֽ�Ŵ
	enemy = new Enemy;
	enemy->Init();

	//��Ÿ�ϸŴ����� ����� �̴��� ��Ŵ
	tileManager = new TileManager;
	tileManager->Init();

	//����� UI ����
	ingameUI = new UserInteface;
	ingameUI->Init();

	//�弱�� UI ����
	selectionUI = new SelectionUI;
	selectionUI->Init();
	DATACENTRE.AddObj(ObjType::UI, "SelectionUI", selectionUI);

	bg = IMAGEMANAGER->AddImage("tmp", L"IMAGE/Tiles/temp.png");

	CAMERA.Init();

	player->SetTurnStart(); //TODO:����
	cursor->SetCursorTurn(IngameStatus::TurnChanging);
	gameoverInit = false;
	gameEndReady = false;
	gameOverPhase = 0;
}

	//������ ������ ���Ҵ� ���� �ֵ� �������� ����
void Battle01::Release()
{
	//������ ���� ������ �ϴ�, ���� ������ ����ߴ� �����̳� ����� ���
	DATACENTRE.RefObjects(ObjType::UI).clear();


	//��Ÿ�ϸŴ��� ������ �� ����
	tileManager->Release();
	SAFE_DELETE(tileManager);

	//��Ŀ�� ������ �� ����
	cursor->Release();
	SAFE_DELETE(cursor);

	//�忡�ʹ� ������ �� ����
	enemy->Release();
	SAFE_DELETE(enemy);
	
	//���÷��̾� ������ �� ����
	player->Release();
	SAFE_DELETE(player);

	//���ϸŴ��� ������ �� ����
	turnManager->Release();
	SAFE_DELETE(turnManager);

	//��ESC�޴� ������ �� ����
	escMenu->Release();
	SAFE_DELETE(escMenu);
}

void Battle01::Update()
{
	ESCManage(); //ESC �޴��� �������� �޴� �����ºκ�
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

	

	//��Ŀ������ ���������� ���
	currentState = cursor->GetCursorTurn();


	//������� ���¿� ���� ������Ʈ ����� �޶���
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
					SOUNDMANAGER->pause("�ΰ���BGM");
					SOUNDMANAGER->FadeIn();

					SCENEMANAGER->LoadScene("TitleScene");
				}
			}
			break;
		}

		break;
	default:
		assert(!"�ΰ���status ���ۼ��Ȱ� ����");
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
		//�巣�� ������ �������� �� ����
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