#pragma once
#include "GameNode.h"

class BattleScenes : public GameNode
{

protected:

	IngameStatus currentState;						//��� �������� ���� �ϳ��� ���¸� ������.
	IngameStatus previousState;						//UI�Ҹ����� ����� �������¸� ����Ѵ�
	
	class StartPlacement* startPlacement = nullptr;	//��� �������� ���� UI�� ������
	class ExecuteBattle* battleManager = nullptr;	//��� �������� ������ ������
	class TileManager* tileManager = nullptr;		//��� �������� Ÿ�� �Ŵ����� ������
	class TurnManager* turnManager = nullptr;		//��� �������� �� �Ŵ����� ������
	class SelectionUI* selectionUI = nullptr;		//��� �������� ����UI�� ������
	class UserInteface* ingameUI = nullptr;			//��� �������� �ΰ��ӿ� UI�� ������
	class ESCMenu* escMenu = nullptr;				//��� �������� ESC�޴��� ������
	class Cursor* cursor = nullptr;					//��� ���� Ŀ���� ������
	class Player* player = nullptr;					//��� �������� �÷��̾ ������
	class Enemy* enemy = nullptr;					//��� �������� ���� ������
	
	//��ESC �޴� ����
	void ESCManage();

public:
	BattleScenes();
	~BattleScenes() {};
};

