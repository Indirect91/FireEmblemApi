#pragma once
#include "GameNode.h"

class BattleScenes : public GameNode
{

protected:

	IngameStatus currentState;					//��� �������� ���� �ϳ��� ���¸� ������.
	IngameStatus previousState;					//UI�Ҹ����� ����� �������¸� ����Ѵ�

	class TileManager* tileManager;				//��� �������� Ÿ�� �Ŵ����� ������
	class TurnManager* turnManager;				//��� �������� �� �Ŵ����� ������
	class ESCMenu* escMenu;						//��� �������� ESC�޴��� ������
	class StartPlacement* startPlacement;		//��� �������� ���� UI�� ������
	class Player* player = nullptr;				//��� �������� �÷��̾ ������
	class Enemy* enemy = nullptr;				//��� �������� ���� ������
	class Cursor* cursor = nullptr;				//��� ���� Ŀ���� ������
	class UserInteface* ingameUI = nullptr;		//��� �������� �ΰ��ӿ� UI�� ������
	class SelectionUI* selectionUI = nullptr;	//��� �������� ����UI�� ������
	
	//��ESC �޴� ����
	void ESCManage();

public:
	BattleScenes();
	~BattleScenes() {};
};

