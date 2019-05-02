#include "../stdafx.h"
#include "BattleScenes.h"

BattleScenes::BattleScenes()
{
	previousState = IngameStatus::StartPlacement;
	currentState = IngameStatus::PlayerTurn;

	startPlacement = nullptr;
	battleManager = nullptr;
	tileManager = nullptr;
	selectionUI = nullptr;
	turnManager = nullptr;
	ingameUI = nullptr;
	escMenu = nullptr;
	cursor = nullptr;
	player = nullptr;
	enemy = nullptr;
}

void BattleScenes::ESCManage()
{
	if (KEYMANAGER->IsOnceKeyDown(VK_ESCAPE))
	{
		if (currentState != IngameStatus::ESCMenu) //���� ESC �޴��� ���� �ִ� ���°� �ƴϾ��ٸ�
		{
			previousState = currentState;
			currentState = IngameStatus::ESCMenu;
		}
		else //ESC�� �ƴϾ��� ��� ���´� �� ���ǿ� �ɸ���, else�� ESC�޴��� �����ְ� �ִ� ��Ȳ
		{
			currentState = previousState; //�õ� �귯���� ��� ���� ������Ʈ �ٽ� ����
		}
	}
}
