#pragma once
#include "GameNode.h"

class BattleScenes : public GameNode
{

protected:

	IngameStatus currentState;						//모든 전투씬은 셋중 하나의 상태를 가진다.
	IngameStatus previousState;						//UI불릴때를 대비해 이전상태를 기억한다
	
	class StartPlacement* startPlacement = nullptr;	//모든 전투씬은 시작 UI를 가진다
	class ExecuteBattle* battleManager = nullptr;	//모든 전투씬은 전투를 가진다
	class TileManager* tileManager = nullptr;		//모든 전투씬은 타일 매니져를 가진다
	class TurnManager* turnManager = nullptr;		//모든 전투씬은 턴 매니져를 가진다
	class SelectionUI* selectionUI = nullptr;		//모든 전투씬은 선택UI를 가진다
	class UserInteface* ingameUI = nullptr;			//모든 전투씬은 인게임용 UI를 가진다
	class ESCMenu* escMenu = nullptr;				//모든 전투씬은 ESC메뉴를 가진다
	class Cursor* cursor = nullptr;					//모든 씬은 커서를 가진다
	class Player* player = nullptr;					//모든 전투씬은 플레이어를 가진다
	class Enemy* enemy = nullptr;					//모든 전투씬은 적을 가진다
	
	//▼ESC 메뉴 관련
	void ESCManage();

public:
	BattleScenes();
	~BattleScenes() {};
};

