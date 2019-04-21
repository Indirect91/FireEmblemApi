#pragma once
#include "GameNode.h"

class BattleScenes : public GameNode
{
protected:
	enum class ingameStatus : UINT //�ΰ��� �� ���´� ��Ʋ���鸸 �������ִ´�
	{
		UI, PlayerTurn, EnemyTurn
	};

	ingameStatus currentState;				//��� �������� ���� �ϳ��� ���¸� ������.
	class TurnManager* turnManager;			//��� �������� �� �Ŵ����� ������
	class ESCMenu* escMenu;					//��� �������� ESC�޴��� ������
	class Tiles* arrField = nullptr;		//��� �������� Ÿ���� ������
	class Player* player = nullptr;			//��� �������� �÷��̾ ������
	class Enemy* enemy = nullptr;			//��� �������� ���� ������
	
	//��Ŀ�� ������ �̹��� ����
	UINT cursorFrame;						//Ŀ�� ������
	UINT cursorCounter;						//Ŀ�� �����ӿø� ī����
	void CursorFrameManage();				//Ŀ�� ������ ���� �Լ�

public:
	BattleScenes();
	~BattleScenes() {};
};

