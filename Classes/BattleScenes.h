#pragma once
#include "GameNode.h"

class BattleScenes : public GameNode
{
protected:
	enum class ingameStatus : UINT //�ΰ��� �� ���´� ��Ʋ���鸸 �������ִ´�
	{
		ESCMenu, 
		PlayerTurn, 
		EnemyTurn, 
		TurnChanging,
		StartPlacement
	};

	ingameStatus currentState;				//��� �������� ���� �ϳ��� ���¸� ������.
	ingameStatus previousState;				//UI�Ҹ����� ����� �������¸� ����Ѵ�

	class TileManager* tileManager;			//��� �������� Ÿ�� �Ŵ����� ������
	class TurnManager* turnManager;			//��� �������� �� �Ŵ����� ������
	class ESCMenu* escMenu;					//��� �������� ESC�޴��� ������
	class StartPlacement* startPlacement;	//��� �������� ���� UI�� ������
	//class Tiles* arrField = nullptr;		//��� �������� Ÿ���� ������
	class Player* player = nullptr;			//��� �������� �÷��̾ ������
	class Enemy* enemy = nullptr;			//��� �������� ���� ������
	class Cursor* cursor = nullptr;			//��� ���� Ŀ���� ������
	
	//��ESC �޴� ����
	void ESCManage();

public:
	BattleScenes();
	~BattleScenes() {};
};

