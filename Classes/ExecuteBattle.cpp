#include "../stdafx.h"
#include "ExecuteBattle.h"
#include "Cursor.h"
#include "Character.h"

ExecuteBattle::ExecuteBattle()
{
	cursor = nullptr;
	victim = nullptr;
	attacker = nullptr;
	battleMode = BattleMode::Simplified;
	battleState = BattleState::AttackerMoving;
}


void ExecuteBattle::Init()
{
	cursor = dynamic_cast<Cursor*>(DATACENTRE.GetCertainObject(ObjType::UI, "Cursor"));
}

void ExecuteBattle::Release()
{
	//���Ҵ������ ����
}

void ExecuteBattle::Update()
{





	switch (battleState)
	{
	case ExecuteBattle::BattleState::AttackerMoving:

		attacker->SetStatus(Character::CharStatus::IsMoving);
		TODO: �̵���Ű��;
		break;
	case ExecuteBattle::BattleState::AttackerAttacking:
	{
		FLOAT faceEachOther = GetAngleDegree(attacker->GetIndex(), victim->GetIndex());

		if (faceEachOther == 0)
		{

		}
		else if (faceEachOther == 90)
		{

		}
		else if (faceEachOther == 180)
		{

		}
		else if (faceEachOther == 270)
		{

		}
		else
		{
			assert(false);
		}


		//1. ������
		//2. �ݰ�
		//3. ������ �ӵ��� 5�� �� ������ 2�� ����



		break;
	}
	case ExecuteBattle::BattleState::VictimAttacking:



		break;
	default:
		assert(false && "��Ʋ ������Ʈ ������� �ִµ�");
		break;
	}

}

void ExecuteBattle::Render()
{

}