#include "../stdafx.h"
#include "ExecuteBattle.h"
#include "Cursor.h"
#include "Character.h"

ExecuteBattle::ExecuteBattle()
{
	cursor = nullptr;

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
	switch (battleMode)
	{
	case ExecuteBattle::BattleMode::Simplified:

		switch (battleState)
		{
		case ExecuteBattle::BattleState::AttackerMoving:

			attacker.charPtr->SetStatus(Character::CharStatus::IsMoving);
			attacker.charPtr->SetIsInBattle(true);
			cursor->SetCursorTurn(cursor->GetCursorTurnPrev());

			break;
		case ExecuteBattle::BattleState::AttackerAttacking:
		{

			//1. 공격자
			//2. 반격
			//3. 공격자 속도가 5가 더 높을시 2방 때림

		//아직 공격모션중이라면
			if (!attacker.isActionDone)
			{
				if (attacker.phase == 0)
				{
					FLOAT faceEachOther = GetAngleDegree(attacker.charPtr->GetIndex(), victim.charPtr->GetIndex());

					if (faceEachOther >= 337.5 && faceEachOther <= 360.f || faceEachOther >= 0 && faceEachOther < 22.5f)
					{
						attacker.charPtr->SetMovingDirection(MovingDirection::RIGHT);
						attacker.attackingDirection = AttackingDirection::RIGHT;
						victim.attackingDirection = AttackingDirection::LEFT;
					}
					else if (faceEachOther >= 22.5f && faceEachOther < 67.5f)
					{
						attacker.charPtr->SetMovingDirection(MovingDirection::RIGHTTOP);
						attacker.attackingDirection = AttackingDirection::RIGHTTOP;
						victim.attackingDirection = AttackingDirection::LEFTBOTTOM;
					}
					else if (faceEachOther >= 67.5f && faceEachOther < 112.5f)
					{
						attacker.charPtr->SetMovingDirection(MovingDirection::TOP);
						attacker.attackingDirection = AttackingDirection::TOP;
						victim.attackingDirection = AttackingDirection::BOTTOM;
					}
					else if (faceEachOther >= 112.5f && faceEachOther < 157.5f)
					{
						attacker.charPtr->SetMovingDirection(MovingDirection::LEFTTOP);
						attacker.attackingDirection = AttackingDirection::LEFTTOP;
						victim.attackingDirection = AttackingDirection::RIGHTBOTTOM;
					}
					else if (faceEachOther >= 157.5f && faceEachOther < 202.5)
					{
						attacker.charPtr->SetMovingDirection(MovingDirection::LEFT);
						attacker.attackingDirection = AttackingDirection::LEFT;
						victim.attackingDirection = AttackingDirection::RIGHT;
					}
					else if (faceEachOther >= 202.5 && faceEachOther < 247.5)
					{
						attacker.charPtr->SetMovingDirection(MovingDirection::RIGHTBOTTOM);
						attacker.attackingDirection = AttackingDirection::RIGHTBOTTOM;
						victim.attackingDirection = AttackingDirection::LEFTTOP;
					}
					else if (faceEachOther >= 247.5 && faceEachOther < 292.5)
					{
						attacker.charPtr->SetMovingDirection(MovingDirection::BOTTOM);
						attacker.attackingDirection = AttackingDirection::BOTTOM;
						victim.attackingDirection = AttackingDirection::TOP;
					}
					else if (faceEachOther >= 292.5 && faceEachOther < 337.5)
					{
						attacker.charPtr->SetMovingDirection(MovingDirection::RIGHTBOTTOM);
						attacker.attackingDirection = AttackingDirection::RIGHTBOTTOM;
						victim.attackingDirection = AttackingDirection::LEFTTOP;
					}
					else
						{assert(!"Wrong Angle");}
					
					todo, 포인터에 알려주기
					victim.charPtr->SetStatus(Character::CharStatus::IsAttacking);

					if (attacker.moveCounter > 20)
					{
						attacker.moveCounter = 0;
						victim.charPtr->SetStatus(Character::CharStatus::IsAttacking);
						attacker.phase++;
					}
					else
					{
						attacker.moveCounter++;
					}
					
				}
				if (attacker.phase == 1)
				{
					
					switch (attacker.attackingDirection)
					{
					case GameObject::DraggingDirection::IDLE:
						break;
					case GameObject::DraggingDirection::LEFT:
						attacker.charPtr->SetPosition(moveLeft(attacker.charPtr->GetPosition(),attacker.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::RIGHT:
						attacker.charPtr->SetPosition(moveRight(attacker.charPtr->GetPosition(), attacker.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::BOTTOM:
						attacker.charPtr->SetPosition(moveDown(attacker.charPtr->GetPosition(), attacker.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::TOP:
						attacker.charPtr->SetPosition(moveUp(attacker.charPtr->GetPosition(), attacker.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::LEFTBOTTOM:
						attacker.charPtr->SetPosition(moveDownLeft(attacker.charPtr->GetPosition(), attacker.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::RIGHTBOTTOM:
						attacker.charPtr->SetPosition(moveDownRight(attacker.charPtr->GetPosition(), attacker.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::LEFTTOP:
						attacker.charPtr->SetPosition(moveUpLeft(attacker.charPtr->GetPosition(), attacker.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::RIGHTTOP:
						attacker.charPtr->SetPosition(moveUpRight(attacker.charPtr->GetPosition(), attacker.attackMotionSpeed));
						break;
					default:
						assert(!"안만든거 있는가 체크");
						break;
					}
					if (attacker.moveCounter < 8) attacker.moveCounter++;
					else attacker.phase++;
				}
				else if (attacker.phase==2)
				{
					switch (attacker.attackingDirection)
					{
					case GameObject::DraggingDirection::IDLE:
						break;
					case GameObject::DraggingDirection::LEFT:
						attacker.charPtr->SetPosition(moveRight(attacker.charPtr->GetPosition(), attacker.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::RIGHT:
						attacker.charPtr->SetPosition(moveLeft(attacker.charPtr->GetPosition(), attacker.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::BOTTOM:
						attacker.charPtr->SetPosition(moveUp(attacker.charPtr->GetPosition(), attacker.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::TOP:
						attacker.charPtr->SetPosition(moveDown(attacker.charPtr->GetPosition(), attacker.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::LEFTBOTTOM:
						attacker.charPtr->SetPosition(moveUpRight(attacker.charPtr->GetPosition(), attacker.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::RIGHTBOTTOM:
						attacker.charPtr->SetPosition(moveUpLeft(attacker.charPtr->GetPosition(), attacker.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::LEFTTOP:
						attacker.charPtr->SetPosition(moveDownRight(attacker.charPtr->GetPosition(), attacker.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::RIGHTTOP:
						attacker.charPtr->SetPosition(moveDownLeft(attacker.charPtr->GetPosition(), attacker.attackMotionSpeed));
						break;
					default:
						assert(!"안만든거 있는가 체크");
						break;
					}
					if (attacker.moveCounter >= 0) attacker.moveCounter--;
					else attacker.phase++;
				}
				else
				{
					//공격 모션 끝난 후
					//exit(0);
				}
			}
			//▼공격이 끝났다면
			else
			{

			}


			break;
		}
		case ExecuteBattle::BattleState::VictimAttacking:



			break;
		default:
			assert(false && "배틀 스테이트 덜만든거 있는듯");
			break;
		}
		break;
	case ExecuteBattle::BattleMode::Full:
		break;
	default:
		assert(!"존재하면 안될 배틀모드가 있음");
		break;
	}

}

void ExecuteBattle::Render()
{

}