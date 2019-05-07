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


void ExecuteBattle::Revert()
{
	attacker.phase = 0;
	attacker.moveCounter = 0;
	attacker.criticalFrameRenderX = 0;
	attacker.critMotionCounter = 0;
	attacker.isActionDone = false;
	attacker.attackMotionSpeed = 3;
	attacker.hitDistinguisher = (rand() % 100) + 1;					//1~100 랜덤값
	attacker.attackCount = 0;
	attacker.charPtr = nullptr;
	attacker.attackingDirection = AttackingDirection::LEFT;
	attacker.attackStatus = AttackStatue::Normal;

	victim.phase = 0;
	victim.moveCounter = 0;
	victim.criticalFrameRenderX = 0;
	victim.critMotionCounter = 0;
	victim.isActionDone = false;
	victim.attackMotionSpeed = 3;
	victim.hitDistinguisher = (rand() % 100) + 1;					//1~100 랜덤값
	victim.attackCount = 0;
	victim.charPtr = nullptr;
	victim.attackingDirection = AttackingDirection::LEFT;
	victim.attackStatus = AttackStatue::Normal;
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
			attacker.criticalFrameRenderX = 0;
			attacker.critMotionCounter = 0;
			attacker.moveCounter = 0;
			attacker.hitDistinguisher = (rand() % 100) + 1;					//1~100 랜덤값
			cursor->SetCursorTurn(cursor->GetCursorTurnPrev());


			victim.charPtr->SetIsInBattle(true);
			victim.criticalFrameRenderX = 0;
			victim.critMotionCounter = 0;
			victim.moveCounter = 0;
			victim.hitDistinguisher = (rand() % 100) + 1;					//1~100 랜덤값

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


					//attacker.hitDistinguisher = 5;	//테스트용. 크리
					//attacker.hitDistinguisher = 95;	//테스트용. 미스
					//attacker.hitDistinguisher = 80;	//테스트용. 통상

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
					{
						assert(!"Wrong Angle");
					}

					victim.charPtr->SetMovingDirection(victim.attackingDirection);
					victim.charPtr->SetStatus(Character::CharStatus::IsAttacking);

					//▼크리티컬이 아니면 이동 완료 후 일정 시간 후에 때림
					if (attacker.moveCounter > 20 && !(attacker.attackStatus == AttackStatue::Critical))
					{
						attacker.moveCounter = 0;
						attacker.phase++; //다음단계 이동
					}
					//▼크리티컬이면 다음 단계로 넘김
					else if (attacker.attackStatus == AttackStatue::Critical)
					{
						SOUNDMANAGER->play("CriticalInit"); //크리티컬 사운드 1회만 재생해주고
						attacker.phase++; //다음단계 이동
					}
					//▼아까 말한 크리티컬 상태 외 일정시간
					else
					{
						attacker.moveCounter++;
					}
				}

				if (attacker.phase == 1)
				{
					//▼크리티컬 상태일때만 발동하는 코드
					if (attacker.attackStatus == AttackStatue::Critical)
					{
						if (attacker.critMotionCounter < 20)
						{
							attacker.moveCounter++;
							if (attacker.moveCounter % 2 == 0)
							{
								attacker.criticalFrameRenderX++;
							}
							if (attacker.criticalFrameRenderX > 6)
							{
								attacker.criticalFrameRenderX = 9;
								attacker.critMotionCounter++;
							}
						}
						else
						{
							attacker.moveCounter = 0;
							attacker.phase++;
						}
					}
					//▼크리티컬이 아닌 상태면 다음단계로 넘김
					else
					{
						attacker.phase++;
						attacker.moveCounter = 0;
					}
				}

				else if (attacker.phase == 2)
				{
					switch (attacker.attackingDirection)
					{
					case GameObject::DraggingDirection::IDLE:
						break;
					case GameObject::DraggingDirection::LEFT:
						attacker.charPtr->SetPosition(moveLeft(attacker.charPtr->GetPosition(), attacker.attackMotionSpeed));
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
				else if (attacker.phase == 3)
				{
					if (attacker.attackStatus == AttackStatue::Critical) //크리일때
					{
						//▼ 크리티컬, 데미지 2배
						FLOAT calculateDamage = (attacker.charPtr->GetDamage() * 2) - (victim.charPtr->GetDefence());
						//calculateDamage = 0; //테스트용
						//calculateDamage = 100; //테스트용
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

						//calculateDamage = 0; //테스트용
						//calculateDamage = 100; //테스트용

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
					else if (attacker.attackStatus == AttackStatue::Miss)	//이외 조건은 빗나감
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
				else if (attacker.phase == 4)
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
					if (attacker.moveCounter >= 0)
					{
						attacker.moveCounter--;
					}
					else
					{
						attacker.phase++;
						attacker.charPtr->SetPositionViaIndex();
					}
				}
				else if (attacker.phase == 5)
				{
					//▼피격자가 반격상태일때
					if (victim.charPtr->GetCurrentHealth() > 0 && victim.charPtr->GetAttackRange() >= attacker.charPtr->GetAttackRange())
						//if (false) //테스트
					{
						if (attacker.charPtr->GetSpeed() >= victim.charPtr->GetSpeed() + 5)
						{
							//▼상대 공격 후에 한번 더 쳐야하니 아직 액션 안끝났다고 false
							attacker.isActionDone = false;
						}
						else
						{
							//▼이번 공격이 마지막 공격
							attacker.isActionDone = true;
						}
						battleState = BattleState::VictimAttacking;
					}

					//▼피격자가 사거리만 딸릴시
					else if (victim.charPtr->GetCurrentHealth() > 0 && victim.charPtr->GetAttackRange() < attacker.charPtr->GetAttackRange() && attacker.charPtr->GetSpeed() < victim.charPtr->GetSpeed() + 5)
					{
						victim.charPtr->SetStatus(Character::CharStatus::Idle);
						battleState = BattleState::BattleEnd;
					}

					//▼피격자가 사거리도 딸리고 속도도5이상 딸릴시, 공격자 2타 때리게 되돌려보냄
					else if (victim.charPtr->GetCurrentHealth() > 0 && victim.charPtr->GetAttackRange() < attacker.charPtr->GetAttackRange() && attacker.charPtr->GetSpeed() >= victim.charPtr->GetSpeed() + 5 && attacker.attackCount == 0)
						//else if (attacker.attackCount==0) //테스트
					{
						attacker.hitDistinguisher = (rand() % 100) + 1;
						attacker.attackCount++;
						attacker.phase = 0;
						attacker.moveCounter = 0;
						attacker.criticalFrameRenderX = 0;
						attacker.critMotionCounter = 0;
						attacker.isActionDone = false;
						attacker.attackStatus = AttackStatue::Normal;

					}
					//▼피격자가 사거리, 속도가 딸리고 2타가 이미 진행되었을시
					else if (victim.charPtr->GetCurrentHealth() > 0 && victim.charPtr->GetAttackRange() < attacker.charPtr->GetAttackRange() && attacker.attackCount == 1)
						//else if (attacker.attackCount == 1) //테스트
					{
						victim.charPtr->SetStatus(Character::CharStatus::Idle);
						battleState = BattleState::BattleEnd;
					}

					//▼피격자 뒤졌을시
					else if (victim.charPtr->GetCurrentHealth() == 0)
					{
						cursor->SetCursorOccupied("");
						attacker.charPtr->SetStatus(Character::CharStatus::IsActed);
						victim.charPtr->SetStatus(Character::CharStatus::IsDying);
						cursor->SetCursorTurn(cursor->GetCursorTurnPrev());
						cursor->SetIsOtherUnitOn("", OwnedBy::Nobody);
						battleState = BattleState::AttackerMoving;
						SOUNDMANAGER->play("CharacterDead");
						battleState = BattleState::BattleEnd;
						//attacker.phase++;

						Revert();
					}
				}
			}
			//▼공격이 끝났다면
			else
			{
				assert(!"공격자 턴에 문제있음");
			}


			break;
		}
		case ExecuteBattle::BattleState::VictimAttacking:

			//▼만약 피격자의 액션이 끝나지 않았다면
			if (!victim.isActionDone)
			{
				//▼페이즈 0
				if (victim.phase == 0)
				{
					FLOAT accuracy = victim.charPtr->GetLuck();	//명중률
					FLOAT critical = 100 - accuracy;				//크리율


					//attacker.hitDistinguisher = 5;	//테스트용. 크리
					//attacker.hitDistinguisher = 95;	//테스트용. 미스
					//attacker.hitDistinguisher = 80;	//테스트용. 통상

					if (victim.hitDistinguisher < critical)//나온 랜덤값이 크리범위 이내면
					{
						victim.attackStatus = AttackStatue::Critical;
					}
					else if (victim.hitDistinguisher < accuracy)//나온 랜덤값이 크리티컬보단 크고 회피보단 낮을땐
					{
						victim.attackStatus = AttackStatue::Normal;
					}
					else
					{
						victim.attackStatus = AttackStatue::Miss;
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
					{
						assert(!"Wrong Angle");
					}

					//victim.charPtr->SetMovingDirection(victim.attackingDirection);
					//victim.charPtr->SetStatus(Character::CharStatus::IsAttacking);

					//▼크리티컬이 아니면 이동 완료 후 일정 시간 후에 때림
					if (victim.moveCounter > 20 && !(victim.attackStatus == AttackStatue::Critical))
					{
						victim.moveCounter = 0;
						victim.phase++; //다음단계 이동
					}
					//▼크리티컬이면 다음 단계로 넘김
					else if (victim.attackStatus == AttackStatue::Critical)
					{
						SOUNDMANAGER->play("CriticalInit"); //크리티컬 사운드 1회만 재생해주고
						victim.phase++; //다음단계 이동
					}
					//▼아까 말한 크리티컬 상태 외 일정시간
					else
					{
						victim.moveCounter++;
					}
				}

				if (victim.phase == 1)
				{
					//▼크리티컬 상태일때만 발동하는 코드
					if (victim.attackStatus == AttackStatue::Critical)
					{
						if (victim.critMotionCounter < 20)
						{
							victim.moveCounter++;
							if (victim.moveCounter % 2 == 0)
							{
								victim.criticalFrameRenderX++;
							}
							if (victim.criticalFrameRenderX > 6)
							{
								victim.criticalFrameRenderX = 9;
								victim.critMotionCounter++;
							}
						}
						else
						{
							victim.moveCounter = 0;
							victim.phase++;
						}
					}
					//▼크리티컬이 아닌 상태면 다음단계로 넘김
					else
					{
						victim.phase++;
						victim.moveCounter = 0;
					}
				}

				else if (victim.phase == 2)
				{
					switch (victim.attackingDirection)
					{
					case GameObject::DraggingDirection::IDLE:
						break;
					case GameObject::DraggingDirection::LEFT:
						victim.charPtr->SetPosition(moveLeft(victim.charPtr->GetPosition(), victim.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::RIGHT:
						victim.charPtr->SetPosition(moveRight(victim.charPtr->GetPosition(), victim.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::BOTTOM:
						victim.charPtr->SetPosition(moveDown(victim.charPtr->GetPosition(), victim.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::TOP:
						victim.charPtr->SetPosition(moveUp(victim.charPtr->GetPosition(), victim.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::LEFTBOTTOM:
						victim.charPtr->SetPosition(moveDownLeft(victim.charPtr->GetPosition(), victim.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::RIGHTBOTTOM:
						victim.charPtr->SetPosition(moveDownRight(victim.charPtr->GetPosition(), victim.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::LEFTTOP:
						victim.charPtr->SetPosition(moveUpLeft(victim.charPtr->GetPosition(), victim.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::RIGHTTOP:
						victim.charPtr->SetPosition(moveUpRight(victim.charPtr->GetPosition(), victim.attackMotionSpeed));
						break;
					default:
						assert(!"안만든거 있는가 체크");
						break;
					}

					//▼여기 무브카운터 건드리면 이동거리 조절가능
					if (victim.moveCounter < 8) victim.moveCounter++;
					else victim.phase++;//다음단계 이동
				}

				//▼ 데미지 계산부분
				else if (victim.phase == 3)
				{
					if (victim.attackStatus == AttackStatue::Critical) //크리일때
					{
						//▼ 크리티컬, 데미지 2배
						FLOAT calculateDamage = (victim.charPtr->GetDamage() * 2) - (attacker.charPtr->GetDefence());
						//calculateDamage = 0; //테스트용
						//calculateDamage = 100; //테스트용
						if (calculateDamage <= 0)
						{
							calculateDamage = 0;
							SOUNDMANAGER->play("CharacterDamage0");
						}
						else
						{
							//▼무기상성 //Swords > Axes //Axes > Lances //Lances > Swords
							if (victim.charPtr->GetWeaponType() == WeaponType::Axe && attacker.charPtr->GetWeaponType() == WeaponType::Sword)
							{
								calculateDamage = calculateDamage * 0.8f;
							}
							else if (victim.charPtr->GetWeaponType() == WeaponType::Axe && attacker.charPtr->GetWeaponType() == WeaponType::Lance)
							{
								calculateDamage = calculateDamage * 1.2f;
							}
							else if (victim.charPtr->GetWeaponType() == WeaponType::Sword && attacker.charPtr->GetWeaponType() == WeaponType::Lance)
							{
								calculateDamage = calculateDamage * 0.8f;
							}
							else if (victim.charPtr->GetWeaponType() == WeaponType::Sword && attacker.charPtr->GetWeaponType() == WeaponType::Axe)
							{
								calculateDamage = calculateDamage * 1.2f;
							}
							else if (victim.charPtr->GetWeaponType() == WeaponType::Lance && attacker.charPtr->GetWeaponType() == WeaponType::Axe)
							{
								calculateDamage = calculateDamage * 0.8f;
							}
							else if (victim.charPtr->GetWeaponType() == WeaponType::Lance && attacker.charPtr->GetWeaponType() == WeaponType::Sword)
							{
								calculateDamage = calculateDamage * 1.2f;
							}

							SOUNDMANAGER->play("CriticalAttack");
							CAMERA.SetShake(16);
						}


						attacker.charPtr->SetCurrentHpSubtractByValue(calculateDamage);

					}
					else if (victim.attackStatus == AttackStatue::Normal) //일반공격일때
					{
						//▼나온 랜덤값이 명중률 범위 이내면 힛. 통상 데미지 1배
						FLOAT calculateDamage = (victim.charPtr->GetDamage()) - (attacker.charPtr->GetDefence());

						//calculateDamage = 0; //테스트용
						//calculateDamage = 100; //테스트용

						if (calculateDamage <= 0)
						{
							calculateDamage = 0;
							SOUNDMANAGER->play("CharacterDamage0");
						}
						else
						{
							//▼무기상성 //Swords > Axes //Axes > Lances //Lances > Swords
							if (victim.charPtr->GetWeaponType() == WeaponType::Axe && attacker.charPtr->GetWeaponType() == WeaponType::Sword)
							{
								calculateDamage = calculateDamage * 0.8f;
							}
							else if (victim.charPtr->GetWeaponType() == WeaponType::Axe && attacker.charPtr->GetWeaponType() == WeaponType::Lance)
							{
								calculateDamage = calculateDamage * 1.2f;
							}
							else if (victim.charPtr->GetWeaponType() == WeaponType::Sword && attacker.charPtr->GetWeaponType() == WeaponType::Lance)
							{
								calculateDamage = calculateDamage * 0.8f;
							}
							else if (victim.charPtr->GetWeaponType() == WeaponType::Sword && attacker.charPtr->GetWeaponType() == WeaponType::Axe)
							{
								calculateDamage = calculateDamage * 1.2f;
							}
							else if (victim.charPtr->GetWeaponType() == WeaponType::Lance && attacker.charPtr->GetWeaponType() == WeaponType::Axe)
							{
								calculateDamage = calculateDamage * 0.8f;
							}
							else if (victim.charPtr->GetWeaponType() == WeaponType::Lance && attacker.charPtr->GetWeaponType() == WeaponType::Sword)
							{
								calculateDamage = calculateDamage * 1.2f;
							}
							SOUNDMANAGER->play("NormalAttack");
							CAMERA.SetShake(8);
						}
						attacker.charPtr->SetCurrentHpSubtractByValue(calculateDamage);

					}
					else if (victim.attackStatus == AttackStatue::Miss)	//이외 조건은 빗나감
					{
						SOUNDMANAGER->play("MissAttack");
					}
					else
					{
						assert(!"공격상태 문제있음");
					}

					//▼TODO 액션 후에 페이즈 증가
					victim.phase++;
				}

				//▼돌아오는 코드
				else if (victim.phase == 4)
				{
					switch (victim.attackingDirection)
					{
					case GameObject::DraggingDirection::IDLE:
						break;
					case GameObject::DraggingDirection::LEFT:
						victim.charPtr->SetPosition(moveRight(victim.charPtr->GetPosition(), victim.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::RIGHT:
						victim.charPtr->SetPosition(moveLeft(victim.charPtr->GetPosition(), victim.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::BOTTOM:
						victim.charPtr->SetPosition(moveUp(victim.charPtr->GetPosition(), victim.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::TOP:
						victim.charPtr->SetPosition(moveDown(victim.charPtr->GetPosition(), victim.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::LEFTBOTTOM:
						victim.charPtr->SetPosition(moveUpRight(victim.charPtr->GetPosition(), victim.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::RIGHTBOTTOM:
						victim.charPtr->SetPosition(moveUpLeft(victim.charPtr->GetPosition(), victim.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::LEFTTOP:
						victim.charPtr->SetPosition(moveDownRight(victim.charPtr->GetPosition(), victim.attackMotionSpeed));
						break;
					case GameObject::DraggingDirection::RIGHTTOP:
						victim.charPtr->SetPosition(moveDownLeft(victim.charPtr->GetPosition(), victim.attackMotionSpeed));
						break;
					default:
						assert(!"안만든거 있는가 체크");
						break;
					}
					if (victim.moveCounter >= 0)
					{
						victim.moveCounter--;
					}
					else
					{
						victim.phase++;
						victim.charPtr->SetPositionViaIndex();
					}
				}
				else if (victim.phase == 5)
				{
					//▼반격자의 공격 대상이 살았고, 반격대상이 재공격을 안해온다면
					if (attacker.charPtr->GetCurrentHealth() > 0 && attacker.isActionDone && attacker.charPtr->GetSpeed() + 5 > victim.charPtr->GetSpeed() && victim.attackCount == 0)
					{
						battleState = BattleState::BattleEnd;
					}
					//▼반격자의 공격 대상이 살았고, 반격대상이 재공격을 한다면
					else if (attacker.charPtr->GetCurrentHealth() > 0 && !(attacker.isActionDone) && victim.attackCount == 0)
					{
						attacker.phase = 0;
						attacker.attackCount = 1;
						attacker.moveCounter = 0;
						attacker.criticalFrameRenderX = 0;
						attacker.critMotionCounter = 0;
						attacker.hitDistinguisher = (rand() % 100) + 1;
						attacker.isActionDone = false;
						attacker.attackStatus = AttackStatue::Normal;
						victim.phase++;
						battleState = BattleState::AttackerAttacking;
					}

					//▼반격자의 공격 대상이 살았고, 반격대상의 속도가 원 공격자보다 5이상 빠를때
					else if (attacker.charPtr->GetCurrentHealth() > 0 && attacker.charPtr->GetSpeed() + 5 <= victim.charPtr->GetSpeed() && victim.attackCount == 0)
					{
						victim.phase = 0;
						victim.attackCount++;
						victim.moveCounter = 0;
						victim.criticalFrameRenderX = 0;
						victim.critMotionCounter = 0;
						victim.hitDistinguisher = (rand() % 100) + 1;
						victim.isActionDone = false;
						victim.attackStatus = AttackStatue::Normal;

					}
					//▼반격자의 공격 대상이 살았고, 반격자가 2타 모두 때렸을때
					else if (attacker.charPtr->GetCurrentHealth() > 0 && attacker.charPtr->GetSpeed() + 5 <= victim.charPtr->GetSpeed() && victim.attackCount == 1)
					{
						battleState = BattleState::BattleEnd;
					}
					//▼공격자가 뒤졌을시
					else if (attacker.charPtr->GetCurrentHealth() <= 0)
					{
						//attacker.charPtr->SetStatus(Character::CharStatus::IsDying);
						//cursor->SetIsOtherUnitOn("", OwnedBy::Nobody);
						//SOUNDMANAGER->play("CharacterDead");
						//battleState = BattleState::AttackerMoving;
						//battleState = BattleState::BattleEnd;
						cursor->SetCursorOccupied("");
						cursor->SetCursorTurn(cursor->GetCursorTurnPrev());
						cursor->SetIsOtherUnitOn("", OwnedBy::Nobody);
						SOUNDMANAGER->play("CharacterDead");
						battleState = BattleState::AttackerMoving;
						attacker.charPtr->SetStatus(Character::CharStatus::IsDying);

						Revert();
					}
				}
				else if (victim.phase == 6)
				{
					battleState = BattleState::BattleEnd;
				}
			}
			//▼공격이 끝났다면
			else
			{
				assert(!"공격자 턴에 문제있음");
			}

			break;
		case ExecuteBattle::BattleState::BattleEnd:
			if (attacker.charPtr->GetStatus() != Character::CharStatus::IsDying)
			{
				attacker.charPtr->SetStatus(Character::CharStatus::IsActed);
			}
			cursor->SetCursorOccupied("");
			cursor->SetIsOtherUnitOn("", OwnedBy::Nobody);
			cursor->SetCursorTurn(cursor->GetCursorTurnPrev());
			battleState = BattleState::AttackerMoving;

			Revert();



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
	if (attacker.attackStatus == AttackStatue::Critical && attacker.criticalFrameRenderX < 8)
	{
		IMAGEMANAGER->FindImage("Blink")->SetSize(IMAGEMANAGER->FindImage("Blink")->GetFrameSize());
		IMAGEMANAGER->FindImage("Blink")->RelativeFrameRender(attacker.charPtr->GetPosition().left - 80, attacker.charPtr->GetPosition().top - 80, attacker.criticalFrameRenderX, 0);
	}
}