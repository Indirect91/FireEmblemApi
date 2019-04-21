#include "../stdafx.h"
#include "BattleScenes.h"

void BattleScenes::CursorFrameManage()
{
	cursorCounter++;
	if (cursorCounter % 10 == 0)
	{
		cursorFrame++;
		if (cursorFrame > 3)cursorFrame = 0;
	}
}

BattleScenes::BattleScenes()
{
	currentState = ingameStatus::UI;
	escMenu = nullptr;
	turnManager = nullptr;
	player = nullptr;
	enemy = nullptr;
	cursorFrame = 0;
	cursorCounter = 0;
}
