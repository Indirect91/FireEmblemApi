#pragma once
#include "Image.h"
class GameNode
{

protected:
	RECT tempC = { 0 };	//���ӳ��κ��� �Ļ��� ��� ���� �浹�� ��Ʈ�� ������
	enum class IngameStatus : UINT //�ΰ��� �� ���´� ��Ʋ���鸸 �������ִ´�
	{
		ESCMenu,
		EnemyTurn,
		PlayerTurn,
		SelectionUI,
		TurnChanging,
		StartPlacement,
	};
public:
	GameNode() {};
	virtual ~GameNode() {}; //���� �Ҹ��ڸ� ����, �ڽ��� �Ҹ��ڰ� �����°� �����Ѵ�

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0; 
	virtual void Release() = 0;
};

