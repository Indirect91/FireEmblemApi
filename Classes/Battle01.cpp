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
	//��ESC�޴��� ����� UI�� ��Ͻ�Ŵ
	escMenu = new ESCMenu; //ESC UI�� ���Ҵ����
	DATACENTRE.AddObj(ObjType::UI, "escMenu", escMenu); //�� �� �ְ� �����

	//���ϸŴ����� ����� UI�� ��Ͻ�Ŵ
	turnManager = new TurnManager; //�ϰ��� UI�� ���Ҵ����
	DATACENTRE.AddObj(ObjType::UI, "TurnManager", turnManager); //�� �� �ְ� �����

	SOUNDMANAGER->pause("Ÿ��ƲBGM");
	SOUNDMANAGER->play("�ΰ���BGM");


	//��ʿ� �°� ī�޶� ������
	CAMERA.SetCamera({ 0,0,800,800 });

	//��Ŀ���� ����� �̴ֽ�Ŵ
	cursor = new Cursor;
	cursor->Init();
	DATACENTRE.AddObj(ObjType::UI, "Cursor", cursor);

	//���÷��̾ ����� �̴ֽ�Ŵ
	player = new Player;
	player->Init();

	//�忡�ʹ̸� ����� �̴ֽ�Ŵ
	enemy = new Enemy;
	enemy->Init();

	//��Ÿ�ϸŴ����� ����� �̴��� ��Ŵ
	tileManager = new TileManager;
	tileManager->Init();

	bg = IMAGEMANAGER->AddImage("tmp", L"IMAGE/Tiles/temp.png");
	bg2 = IMAGEMANAGER->AddImage("tmp1", L"IMAGE/Tiles/temp2.png");
	bg3 = IMAGEMANAGER->AddImage("tmp2", L"IMAGE/Tiles/temp3.png");
	bg4 = IMAGEMANAGER->AddImage("tmp3", L"IMAGE/Tiles/temp4.png");

	CAMERA.Init();

	player->SetTurnStart(); //TODO:����
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
	
	//������� ���¿� ���� ������Ʈ ����� �޶���
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
	//�巣�� ������ �������� �� ����
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