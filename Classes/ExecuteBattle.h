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
		BattleEnd,
	};

	enum class AttackStatue
	{
		Normal, Critical, Miss,
	};

	struct Opponents
	{
		INT phase = 0;						//공격 모션을 상징
		INT moveCounter = 0;				//공격 이동수치
		INT criticalFrameRenderX = 0;		//크리모션용
		INT critMotionCounter = 0;			//크리티컬 모션중인지
		BOOL isActionDone = false;			//공격 다 했는지
		INT attackMotionSpeed = 3;			//이동 와리가리 속도
		INT hitDistinguisher;				//힛,미스,크리 판단
		INT attackCount = 0;				//공격횟수 카운터
		class Character* charPtr = nullptr;	//캐릭터 포인터
		AttackingDirection attackingDirection = AttackingDirection::LEFT;
		AttackStatue attackStatus = AttackStatue::Normal;
	};


private:
	Opponents attacker;						//공격자
	Opponents victim;						//피격대상

	BattleMode battleMode;					//배틀 방식
	BattleState battleState;				//배틀 스테이트
	
	
	//std::map<std::string, GameObject*>& playerTroop = DATACENTRE.RefObjects(ObjType::PlayerArmy);	//플레이어 부대
	//std::map<std::string, GameObject*>& enemyTroop = DATACENTRE.RefObjects(ObjType::EnemyArmy);		//적 부대
	void Revert();
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

