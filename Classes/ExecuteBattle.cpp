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
					FLOAT accuracy = attacker.charPtr->GetLuck();	//명중률
					FLOAT critical = 100 - accuracy;				//크리율
					attacker.hitDistinguisher = (rand() % 100) + 1;					//1~100 랜덤값
					
					attacker.hitDistinguisher = 5;		//테스트용. 크리
					//hitDistinguisher = 95;	//테스트용. 미스
					//hitDistinguisher = 80;	//테스트용. 통상

					if (attacker.hitDistinguisher < critical)//나온 랜덤값이 크리범위 이내면
					{ 
						attacker.attackStatus = AttackStatue::Critical;
					}
					else if (attacker.hitDistinguisher < accuracy)//나온 랜덤값이 크리티컬보단 크고 회피보단 낮을땐
					{
						attacker.attackStatus = AttackStatue::Normal;
					}
					else
					{
						attacker.attackStatus = AttackStatue::Miss;
					}


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
					
					victim.charPtr->SetMovingDirection(victim.attackingDirection);
					victim.charPtr->SetStatus(Character::CharStatus::IsAttacking);
					
					//이동 완료 후 일정 시간 후에 때림
					if (attacker.moveCounter > 20)
					{
						if (attacker.attackStatus == AttackStatue::Critical)
						{
							SOUNDMANAGER->play("CriticalInit");
							if(attacker.criticalCounter>10)


							Sleep(500);
							asd
						}
						attacker.moveCounter = 0;
						attacker.phase++; //다음단계 이동
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

					//▼여기 무브카운터 건드리면 이동거리 조절가능
					if (attacker.moveCounter < 8) attacker.moveCounter++;
					else attacker.phase++;//다음단계 이동
				}

				//▼ 데미지 계산부분
				else if (attacker.phase == 2)
				{
					if (attacker.attackStatus==AttackStatue::Critical) //크리일때
					{
						//▼ 크리티컬, 데미지 2배
						FLOAT calculateDamage = (attacker.charPtr->GetDamage() * 2) - (victim.charPtr->GetDefence());
						//calculateDamage = 0; //테스트용
						if (calculateDamage <= 0)
						{
							calculateDamage = 0;
							SOUNDMANAGER->play("CharacterDamage0");
						}
						else
						{
							//▼무기상성 //Swords > Axes //Axes > Lances //Lances > Swords
							if (attacker.charPtr->GetWeaponType() == WeaponType::Axe && victim.charPtr->GetWeaponType() == WeaponType::Sword)
							{
								calculateDamage = calculateDamage * 0.8f;
							}
							else if (attacker.charPtr->GetWeaponType() == WeaponType::Axe && victim.charPtr->GetWeaponType() == WeaponType::Lance)
							{
								calculateDamage = calculateDamage * 1.2f;
							}
							else if (attacker.charPtr->GetWeaponType() == WeaponType::Sword && victim.charPtr->GetWeaponType() == WeaponType::Lance)
							{
								calculateDamage = calculateDamage * 0.8f;
							}
							else if (attacker.charPtr->GetWeaponType() == WeaponType::Sword && victim.charPtr->GetWeaponType() == WeaponType::Axe)
							{
								calculateDamage = calculateDamage * 1.2f;
							}
							else if (attacker.charPtr->GetWeaponType() == WeaponType::Lance && victim.charPtr->GetWeaponType() == WeaponType::Axe)
							{
								calculateDamage = calculateDamage * 0.8f;
							}
							else if (attacker.charPtr->GetWeaponType() == WeaponType::Lance && victim.charPtr->GetWeaponType() == WeaponType::Sword)
							{
								calculateDamage = calculateDamage * 1.2f;
							}

							SOUNDMANAGER->play("CriticalAttack");
							CAMERA.SetShake(16);
						}
						

						victim.charPtr->SetCurrentHpSubtractByValue(calculateDamage);
						
					}
					else if (attacker.attackStatus == AttackStatue::Normal) //일반공격일때
					{
						//▼나온 랜덤값이 명중률 범위 이내면 힛. 통상 데미지 1배
						FLOAT calculateDamage = (attacker.charPtr->GetDamage()) - (victim.charPtr->GetDefence());
						if (calculateDamage <= 0)
						{
							calculateDamage = 0;
							SOUNDMANAGER->play("CharacterDamage0");
						}
						else
						{
							//▼무기상성 //Swords > Axes //Axes > Lances //Lances > Swords
							if (attacker.charPtr->GetWeaponType() == WeaponType::Axe && victim.charPtr->GetWeaponType() == WeaponType::Sword)
							{
								calculateDamage = calculateDamage * 0.8f;
							}
							else if (attacker.charPtr->GetWeaponType() == WeaponType::Axe && victim.charPtr->GetWeaponType() == WeaponType::Lance)
							{
								calculateDamage = calculateDamage * 1.2f;
							}
							else if (attacker.charPtr->GetWeaponType() == WeaponType::Sword && victim.charPtr->GetWeaponType() == WeaponType::Lance)
							{
								calculateDamage = calculateDamage * 0.8f;
							}
							else if (attacker.charPtr->GetWeaponType() == WeaponType::Sword && victim.charPtr->GetWeaponType() == WeaponType::Axe)
							{
								calculateDamage = calculateDamage * 1.2f;
							}
							else if (attacker.charPtr->GetWeaponType() == WeaponType::Lance && victim.charPtr->GetWeaponType() == WeaponType::Axe)
							{
								calculateDamage = calculateDamage * 0.8f;
							}
							else if (attacker.charPtr->GetWeaponType() == WeaponType::Lance && victim.charPtr->GetWeaponType() == WeaponType::Sword)
							{
								calculateDamage = calculateDamage * 1.2f;
							}
							SOUNDMANAGER->play("NormalAttack");
							CAMERA.SetShake(8);
						}
						victim.charPtr->SetCurrentHpSubtractByValue(calculateDamage);

					}
					else if(attacker.attackStatus == AttackStatue::Miss)	//이외 조건은 빗나감
					{
						SOUNDMANAGER->play("MissAttack");
					}
					else
					{
						assert(!"공격상태 문제있음");
					}

					//▼TODO 액션 후에 페이즈 증가
					attacker.phase++;
				}

				//▼돌아오는 코드
				else if (attacker.phase==3)
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
				else if (attacker.phase ==4)
				{
					//갔다왔다 모션 끝난 후


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