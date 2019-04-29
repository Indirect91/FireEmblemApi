#include "../stdafx.h"
#include "BattleScenes.h"

BattleScenes::BattleScenes()
{
	currentState = IngameStatus::PlayerTurn;
	previousState = IngameStatus::StartPlacement;
	escMenu = nullptr;
	turnManager = nullptr;
	tileManager = nullptr;
	player = nullptr;
	enemy = nullptr;
	startPlacement = nullptr;
	cursor = nullptr;
	selectionUI = nullptr;
	ingameUI = nullptr;
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
