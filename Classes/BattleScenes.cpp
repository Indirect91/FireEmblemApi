#include "../stdafx.h"
#include "BattleScenes.h"

BattleScenes::BattleScenes()
{
	currentState = ingameStatus::PlayerTurn;
	previousState = ingameStatus::StartPlacement;
	escMenu = nullptr;
	turnManager = nullptr;
	player = nullptr;
	enemy = nullptr;
	startPlacement = nullptr;
	cursor = nullptr;

}

void BattleScenes::ESCManage()
{
	if (KEYMANAGER->IsOnceKeyDown(VK_ESCAPE))
	{
		if (currentState != ingameStatus::ESCMenu) //���� ESC �޴��� ���� �ִ� ���°� �ƴϾ��ٸ�
		{
			previousState = currentState;
			currentState = ingameStatus::ESCMenu;
		}
		else //ESC�� �ƴϾ��� ��� ���´� �� ���ǿ� �ɸ���, else�� ESC�޴��� �����ְ� �ִ� ��Ȳ
		{
			currentState = previousState; //�õ� �귯���� ��� ���� ������Ʈ �ٽ� ����
		}
	}
}
