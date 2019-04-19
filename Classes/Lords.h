#pragma once
#include "GameObject.h"

//�屺��, ������ Ŭ����. ĳ���� �������� �ƴ� ������ �����ϴ� �÷��̾��
class Lords : public GameObject
{
protected: //���ַκ��� �Ļ��� �ڽĵ鸸 �δ븦 ������

	BOOL myTurn; //��� ���ֵ��� ������ ���� ������.

public:

	void SetTurnEnd() { myTurn = false; }
	void SetTurnStart() { myTurn = true; }
	void SetTurnStatue(BOOL _turn) { myTurn = _turn; }
	
	const BOOL &GetTurnStatus() const { return myTurn; }
	
	Lords();
	~Lords() {};
};

