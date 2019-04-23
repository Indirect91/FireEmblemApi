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
}

void Battle01::Init()
{
	//��ESC�޴��� ����� UI�� ��Ͻ�Ŵ
	escMenu = new ESCMenu; //ESC UI�� ���Ҵ����
	DATACENTRE.AddObj(ObjType::UI, "escMenu", escMenu); //�� �� �ְ� �����

	//���ϸŴ����� ����� UI�� ��Ͻ�Ŵ
	turnManager = new TurnManager; //�ϰ��� UI�� ���Ҵ����
	DATACENTRE.AddObj(ObjType::UI, "TurnManager", turnManager); //�� �� �ְ� �����



	//�忡�ʹ̸� ����� �̴ֽ�Ŵ
	enemy = new Enemy;
	//enemy->Init();

	//��ʿ� �°� ī�޶� ������
	CAMERA.SetCamera({ 0,0,800,800 });

	//��Ŀ���� ����� �̴ֽ�Ŵ
	cursor = new Cursor;
	cursor->Init(); // ��ġ ���� �ʿ�
	DATACENTRE.AddObj(ObjType::UI, "Cursor", cursor);

	//���÷��̾ ����� �̴ֽ�Ŵ
	player = new Player;
	//player->Init();

	//��Ÿ�ϸŴ����� ����� �̴��� ��Ŵ
	tileManager = new TileManager;
	tileManager->Init();

	bg = IMAGEMANAGER->AddImage("tmp", L"IMAGE/Tiles/temp.png");
	bg2 = IMAGEMANAGER->AddImage("tmp", L"IMAGE/Tiles/temp2.png");



	player->SetTurnStart(); //�����ʿ�
}

void Battle01::Release()
{
	//������ ���� ������ �ϴ�, ���� ������ ����ߴ� �����̳� ����� ���
	DATACENTRE.RefObjects(ObjType::UI).clear();

	//������ ������ ���Ҵ� ���� �ֵ� �������� ����



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
		startPlacement->Update();
		break;
	case BattleScenes::ingameStatus::TurnChanging:
		turnManager->Update();
		break;
	case BattleScenes::ingameStatus::ESCMenu:
		escMenu->Update();
		break;
	case BattleScenes::ingameStatus::PlayerTurn:
		player->Update();
		break;
	case BattleScenes::ingameStatus::EnemyTurn:
		enemy->Update();
		break;
	}
}

void Battle01::Render()
{
	//�巣�� ������ �������� �� ����
	bg->Render(0 - CAMERA.GetCameraRc().left, 0 - CAMERA.GetCameraRc().top);
	bg2->Render(288 - CAMERA.GetCameraRc().left, 0 - CAMERA.GetCameraRc().top);

	tileManager->Render();
	player->Render();
	enemy->Render();
	cursor->Render();
}