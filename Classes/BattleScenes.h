#pragma once
#include "GameNode.h"

class BattleScenes : public GameNode
{
protected:
	enum class ingameStatus : UINT //인게임 턴 상태는 배틀씬들만 가지고있는다
	{
		UI, PlayerTurn, EnemyTurn
	};

	ingameStatus currentState;				//모든 전투씬은 셋중 하나의 상태를 가진다.
	class TurnManager* turnManager;			//모든 전투씬은 턴 매니져를 가진다
	class ESCMenu* escMenu;					//모든 전투씬은 ESC메뉴를 가진다
	class Tiles* arrField = nullptr;		//모든 전투씬은 타일을 가진다
	class Player* player = nullptr;			//모든 전투씬은 플레이어를 가진다
	class Enemy* enemy = nullptr;			//모든 전투씬은 적을 가진다
	
	//▼커서 프레임 이미지 관련
	UINT cursorFrame;						//커서 프레임
	UINT cursorCounter;						//커서 프레임올릴 카운터
	void CursorFrameManage();				//커서 프레임 돌릴 함수

public:
	BattleScenes();
	~BattleScenes() {};
};

