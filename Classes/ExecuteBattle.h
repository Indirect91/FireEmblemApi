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

private:

	class Character* attacker = nullptr;	//����
	class Character* victim = nullptr;		//�İ�(Ȥ�� �Ϲ������� �ĸ�����)
	class Cursor* cursor = nullptr;			//Ŀ���� �ʹ� Ŀ��
	BattleMode battleMode;					//��Ʋ ���
	BattleState battleState;				//��Ʋ ������Ʈ


	std::map<std::string, GameObject*>& playerTroop = DATACENTRE.RefObjects(ObjType::PlayerArmy);	//�÷��̾� �δ�
	std::map<std::string, GameObject*>& enemyTroop = DATACENTRE.RefObjects(ObjType::EnemyArmy);		//�� �δ�

public:

	void Init();
	void Release();
	void Update();
	void Render();

	void SetWhoBattles(Character* _attacker, Character* _victim) { attacker = _attacker; victim = _victim; }
	void SetBattleState(BattleState _state) { battleState = _state; }	//��Ʋ ���� �ٲܶ�
	void SetBattleMode(BattleMode _mode) { battleMode = _mode; }		//��Ʋ ��� �ٲܶ�

	const BattleState & GetBattleState() const { return battleState; }			//��Ʋ ���� ��������
	const BattleMode & GetBattleMode() const { return battleMode; }				//��Ʋ ��� ��������





	ExecuteBattle();
	~ExecuteBattle() {};
};

