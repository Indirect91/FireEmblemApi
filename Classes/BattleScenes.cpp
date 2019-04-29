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
		if (currentState != IngameStatus::ESCMenu) //현재 ESC 메뉴를 열고 있는 상태가 아니었다면
		{
			previousState = currentState;
			currentState = IngameStatus::ESCMenu;
		}
		else //ESC가 아니었을 모든 상태는 위 조건에 걸리니, else는 ESC메뉴를 보여주고 있는 상황
		{
			currentState = previousState; //올드 브러쉬를 잡듯 이전 스테이트 다시 넣음
		}
	}
}
