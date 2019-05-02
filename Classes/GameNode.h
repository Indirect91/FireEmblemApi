#pragma once
#include "Image.h"
class GameNode
{

protected:
	RECT tempC = { 0 };	//���ӳ��κ��� �Ļ��� ��� ���� �浹�� ��Ʈ�� ������

	//���ΰ��� ����
	enum class IngameStatus : UINT
	{
		ESCMenu,
		EnemyTurn,
		PlayerTurn,
		SelectionUI,
		TurnChanging,
		StartPlacement,
		ExecutingBattle,
	};

	//�幫�� Ÿ��
	enum class WeaponType
	{
		Bow,				//Ȱ
		Axe,				//����
		Tomb,				//������
		Sword,				//Į
		Lance,				//â
		Staff,				//������
	};

public:
	GameNode() {};
	virtual ~GameNode() {}; //���� �Ҹ��ڸ� ����, �ڽ��� �Ҹ��ڰ� �����°� �����Ѵ�

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0; 
	virtual void Release() = 0;
};

