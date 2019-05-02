#pragma once
#include "GameObject.h"
class ExecuteBattle : public GameObject
{
public:
	enum class BattleMode
	{
		Simplified,
		Full,
	};

	enum class BattleState
	{
		AttackerMoving,
		AttackerAttacking,
		VictimAttacking,
	};

	struct Opponents
	{
		INT phase = 0;						//공격 모션을 상징
		INT moveCounter = 0;				//공격 이동수치
		BOOL isActionDone = false;			//공격 다 했는지
		INT attackMotionSpeed = 3;
		class Character* charPtr = nullptr;	//캐릭터 포인터
		AttackingDirection attackingDirection = AttackingDirection::LEFT;
	};


private:
	Opponents attacker;						//공격자
	Opponents victim;						//피격대상

	class Cursor* cursor = nullptr;			//커서는 너무 커서
	BattleMode battleMode;					//배틀 방식
	BattleState battleState;				//배틀 스테이트

	//std::map<std::string, GameObject*>& playerTroop = DATACENTRE.RefObjects(ObjType::PlayerArmy);	//플레이어 부대
	//std::map<std::string, GameObject*>& enemyTroop = DATACENTRE.RefObjects(ObjType::EnemyArmy);		//적 부대

public:

	void Init();
	void Release();
	void Update();
	void Render();

	void SetWhoBattles(Character* _attacker, Character* _victim) { attacker.charPtr = _attacker; victim.charPtr = _victim; }
	void SetBattleState(BattleState _state) { battleState = _state; }	//배틀 상태 바꿀때
	void SetBattleMode(BattleMode _mode) { battleMode = _mode; }		//배틀 모드 바꿀때

	const BattleState& GetBattleState() const { return battleState; }			//배틀 상태 가져오기
	const BattleMode& GetBattleMode() const { return battleMode; }				//배틀 모드 가져오기





	ExecuteBattle();
	~ExecuteBattle() {};
};

