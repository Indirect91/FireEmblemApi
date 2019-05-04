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
		INT phase = 0;						//���� ����� ��¡
		INT moveCounter = 0;				//���� �̵���ġ
		INT criticalFrameRenderX = 0;		//ũ����ǿ�
		INT critMotionCounter = 0;			//ũ��Ƽ�� ���������
		BOOL isActionDone = false;			//���� �� �ߴ���
		INT attackMotionSpeed = 3;			//�̵� �͸����� �ӵ�
		INT hitDistinguisher;				//��,�̽�,ũ�� �Ǵ�
		INT attackCount = 0;				//����Ƚ�� ī����
		class Character* charPtr = nullptr;	//ĳ���� ������
		AttackingDirection attackingDirection = AttackingDirection::LEFT;
		AttackStatue attackStatus = AttackStatue::Normal;
	};


private:
	Opponents attacker;						//������
	Opponents victim;						//�ǰݴ��

	class Cursor* cursor = nullptr;			//Ŀ���� �ʹ� Ŀ��
	BattleMode battleMode;					//��Ʋ ���
	BattleState battleState;				//��Ʋ ������Ʈ
	
	
	//std::map<std::string, GameObject*>& playerTroop = DATACENTRE.RefObjects(ObjType::PlayerArmy);	//�÷��̾� �δ�
	//std::map<std::string, GameObject*>& enemyTroop = DATACENTRE.RefObjects(ObjType::EnemyArmy);		//�� �δ�
	void Revert();
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

