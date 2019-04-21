#pragma once
#include "GameObject.h"

//�屺��, ������ Ŭ����. ĳ���� �������� �ƴ� ������ �����ϴ� �÷��̾��
class Lords : public GameObject
{
protected: //���ַκ��� �Ļ��� �ڽĵ鸸 �δ븦 ������

	BOOL turnFlag; //��� ���ֵ��� ������ ���� ������.

public:

	void SetTurnEnd() { turnFlag = false; }
	void SetTurnStart() { turnFlag = true; }
	void SetTurnStatue(BOOL _turn) { turnFlag = _turn; }
	
	const BOOL &GetTurnStatus() const { return turnFlag; }
	
	Lords();
	~Lords() {};
};

