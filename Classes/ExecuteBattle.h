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
		INT phase = 0;						//���� ����� ��¡
		INT moveCounter = 0;				//���� �̵���ġ
		BOOL isActionDone = false;			//���� �� �ߴ���
		INT attackMotionSpeed = 3;
		class Character* charPtr = nullptr;	//ĳ���� ������
		AttackingDirection attackingDirection = AttackingDirection::LEFT;
	};


private:
	Opponents attacker;						//������
	Opponents victim;						//�ǰݴ��

	class Cursor* cursor = nullptr;			//Ŀ���� �ʹ� Ŀ��
	BattleMode battleMode;					//��Ʋ ���
	BattleState battleState;				//��Ʋ ������Ʈ

	//std::map<std::string, GameObject*>& playerTroop = DATACENTRE.RefObjects(ObjType::PlayerArmy);	//�÷��̾� �δ�
	//std::map<std::string, GameObject*>& enemyTroop = DATACENTRE.RefObjects(ObjType::EnemyArmy);		//�� �δ�

public:

	void Init();
	void Release();
	void Update();
	void Render();

	void SetWhoBattles(Character* _attacker, Character* _victim) { attacker.charPtr = _attacker; victim.charPtr = _victim; }
	void SetBattleState(BattleState _state) { battleState = _state; }	//��Ʋ ���� �ٲܶ�
	void SetBattleMode(BattleMode _mode) { battleMode = _mode; }		//��Ʋ ��� �ٲܶ�

	const BattleState& GetBattleState() const { return battleState; }			//��Ʋ ���� ��������
	const BattleMode& GetBattleMode() const { return battleMode; }				//��Ʋ ��� ��������





	ExecuteBattle();
	~ExecuteBattle() {};
};

