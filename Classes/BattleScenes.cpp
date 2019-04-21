#include "../stdafx.h"
#include "BattleScenes.h"

BattleScenes::BattleScenes()
{
	currentState = ingameStatus::UI;
	escMenu = nullptr;
	turnManager = nullptr;
	player = nullptr;
	enemy = nullptr;
}
