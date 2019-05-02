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
	//뉴할당받은거 없음
}

void ExecuteBattle::Update()
{





	switch (battleState)
	{
	case ExecuteBattle::BattleState::AttackerMoving:

		attacker->SetStatus(Character::CharStatus::IsMoving);
		TODO: 이동시키기;
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


		//1. 공격자
		//2. 반격
		//3. 공격자 속도가 5가 더 높을시 2방 때림



		break;
	}
	case ExecuteBattle::BattleState::VictimAttacking:



		break;
	default:
		assert(false && "배틀 스테이트 덜만든거 있는듯");
		break;
	}

}

void ExecuteBattle::Render()
{

}