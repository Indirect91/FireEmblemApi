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
	attacker.hitDistinguisher = (rand() % 100) + 1;					//1~100 ������
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
	victim.hitDistinguisher = (rand() % 100) + 1;					//1~100 ������
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
	//���Ҵ������ ����
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
			attacker.hitDistinguisher = (rand() % 100) + 1;					//1~100 ������
			cursor->SetCursorTurn(cursor->GetCursorTurnPrev());


			victim.charPtr->SetIsInBattle(true);
			victim.criticalFrameRenderX = 0;
			victim.critMotionCounter = 0;
			victim.moveCounter = 0;
			victim.hitDistinguisher = (rand() % 100) + 1;					//1~100 ������

			break;
		case ExecuteBattle::BattleState::AttackerAttacking:
		{

			//1. ������
			//2. �ݰ�
			//3. ������ �ӵ��� 5�� �� ������ 2�� ����

		//���� ���ݸ�����̶��
			if (!attacker.isActionDone)
			{
				if (attacker.phase == 0)
				{
					FLOAT accuracy = attacker.charPtr->GetLuck();	//���߷�
					FLOAT critical = 100 - accuracy;				//ũ����


					//attacker.hitDistinguisher = 5;	//�׽�Ʈ��. ũ��
					//attacker.hitDistinguisher = 95;	//�׽�Ʈ��. �̽�
					//attacker.hitDistinguisher = 80;	//�׽�Ʈ��. ���

					if (attacker.hitDistinguisher < critical)//���� �������� ũ������ �̳���
					{
						attacker.attackStatus = AttackStatue::Critical;
					}
					else if (attacker.hitDistinguisher < accuracy)//���� �������� ũ��Ƽ�ú��� ũ�� ȸ�Ǻ��� ������
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

					//��ũ��Ƽ���� �ƴϸ� �̵� �Ϸ� �� ���� �ð� �Ŀ� ����
					if (attacker.moveCounter > 20 && !(attacker.attackStatus == AttackStatue::Critical))
					{
						attacker.moveCounter = 0;
						attacker.phase++; //�����ܰ� �̵�
					}
					//��ũ��Ƽ���̸� ���� �ܰ�� �ѱ�
					else if (attacker.attackStatus == AttackStatue::Critical)
					{
						SOUNDMANAGER->play("CriticalInit"); //ũ��Ƽ�� ���� 1ȸ�� ������ְ�
						attacker.phase++; //�����ܰ� �̵�
					}
					//��Ʊ� ���� ũ��Ƽ�� ���� �� �����ð�
					else
					{
						attacker.moveCounter++;
					}
				}

				if (attacker.phase == 1)
				{
					//��ũ��Ƽ�� �����϶��� �ߵ��ϴ� �ڵ�
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
					//��ũ��Ƽ���� �ƴ� ���¸� �����ܰ�� �ѱ�
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
						assert(!"�ȸ���� �ִ°� üũ");
						break;
					}

					//�忩�� ����ī���� �ǵ帮�� �̵��Ÿ� ��������
					if (attacker.moveCounter < 8) attacker.moveCounter++;
					else attacker.phase++;//�����ܰ� �̵�
				}

				//�� ������ ���κ�
				else if (attacker.phase == 3)
				{
					if (attacker.attackStatus == AttackStatue::Critical) //ũ���϶�
					{
						//�� ũ��Ƽ��, ������ 2��
						FLOAT calculateDamage = (attacker.charPtr->GetDamage() * 2) - (victim.charPtr->GetDefence());
						//calculateDamage = 0; //�׽�Ʈ��
						//calculateDamage = 100; //�׽�Ʈ��
						if (calculateDamage <= 0)
						{
							calculateDamage = 0;
							SOUNDMANAGER->play("CharacterDamage0");
						}
						else
						{
							//�幫��� //Swords > Axes //Axes > Lances //Lances > Swords
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
					else if (attacker.attackStatus == AttackStatue::Normal) //�Ϲݰ����϶�
					{
						//�峪�� �������� ���߷� ���� �̳��� ��. ��� ������ 1��
						FLOAT calculateDamage = (attacker.charPtr->GetDamage()) - (victim.charPtr->GetDefence());

						//calculateDamage = 0; //�׽�Ʈ��
						//calculateDamage = 100; //�׽�Ʈ��

						if (calculateDamage <= 0)
						{
							calculateDamage = 0;
							SOUNDMANAGER->play("CharacterDamage0");
						}
						else
						{
							//�幫��� //Swords > Axes //Axes > Lances //Lances > Swords
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
					else if (attacker.attackStatus == AttackStatue::Miss)	//�̿� ������ ������
					{
						SOUNDMANAGER->play("MissAttack");
					}
					else
					{
						assert(!"���ݻ��� ��������");
					}

					//��TODO �׼� �Ŀ� ������ ����
					attacker.phase++;
				}

				//�嵹�ƿ��� �ڵ�
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
						assert(!"�ȸ���� �ִ°� üũ");
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
					//���ǰ��ڰ� �ݰݻ����϶�
					if (victim.charPtr->GetCurrentHealth() > 0 && victim.charPtr->GetAttackRange() >= attacker.charPtr->GetAttackRange())
						//if (false) //�׽�Ʈ
					{
						if (attacker.charPtr->GetSpeed() >= victim.charPtr->GetSpeed() + 5)
						{
							//���� ���� �Ŀ� �ѹ� �� �ľ��ϴ� ���� �׼� �ȳ����ٰ� false
							attacker.isActionDone = false;
						}
						else
						{
							//���̹� ������ ������ ����
							attacker.isActionDone = true;
						}
						battleState = BattleState::VictimAttacking;
					}

					//���ǰ��ڰ� ��Ÿ��� ������
					else if (victim.charPtr->GetCurrentHealth() > 0 && victim.charPtr->GetAttackRange() < attacker.charPtr->GetAttackRange() && attacker.charPtr->GetSpeed() < victim.charPtr->GetSpeed() + 5)
					{
						victim.charPtr->SetStatus(Character::CharStatus::Idle);
						battleState = BattleState::BattleEnd;
					}

					//���ǰ��ڰ� ��Ÿ��� ������ �ӵ���5�̻� ������, ������ 2Ÿ ������ �ǵ�������
					else if (victim.charPtr->GetCurrentHealth() > 0 && victim.charPtr->GetAttackRange() < attacker.charPtr->GetAttackRange() && attacker.charPtr->GetSpeed() >= victim.charPtr->GetSpeed() + 5 && attacker.attackCount == 0)
						//else if (attacker.attackCount==0) //�׽�Ʈ
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
					//���ǰ��ڰ� ��Ÿ�, �ӵ��� ������ 2Ÿ�� �̹� ����Ǿ�����
					else if (victim.charPtr->GetCurrentHealth() > 0 && victim.charPtr->GetAttackRange() < attacker.charPtr->GetAttackRange() && attacker.attackCount == 1)
						//else if (attacker.attackCount == 1) //�׽�Ʈ
					{
						victim.charPtr->SetStatus(Character::CharStatus::Idle);
						battleState = BattleState::BattleEnd;
					}

					//���ǰ��� ��������
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
			//������� �����ٸ�
			else
			{
				assert(!"������ �Ͽ� ��������");
			}


			break;
		}
		case ExecuteBattle::BattleState::VictimAttacking:

			//�常�� �ǰ����� �׼��� ������ �ʾҴٸ�
			if (!victim.isActionDone)
			{
				//�������� 0
				if (victim.phase == 0)
				{
					FLOAT accuracy = victim.charPtr->GetLuck();	//���߷�
					FLOAT critical = 100 - accuracy;				//ũ����


					//attacker.hitDistinguisher = 5;	//�׽�Ʈ��. ũ��
					//attacker.hitDistinguisher = 95;	//�׽�Ʈ��. �̽�
					//attacker.hitDistinguisher = 80;	//�׽�Ʈ��. ���

					if (victim.hitDistinguisher < critical)//���� �������� ũ������ �̳���
					{
						victim.attackStatus = AttackStatue::Critical;
					}
					else if (victim.hitDistinguisher < accuracy)//���� �������� ũ��Ƽ�ú��� ũ�� ȸ�Ǻ��� ������
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

					//��ũ��Ƽ���� �ƴϸ� �̵� �Ϸ� �� ���� �ð� �Ŀ� ����
					if (victim.moveCounter > 20 && !(victim.attackStatus == AttackStatue::Critical))
					{
						victim.moveCounter = 0;
						victim.phase++; //�����ܰ� �̵�
					}
					//��ũ��Ƽ���̸� ���� �ܰ�� �ѱ�
					else if (victim.attackStatus == AttackStatue::Critical)
					{
						SOUNDMANAGER->play("CriticalInit"); //ũ��Ƽ�� ���� 1ȸ�� ������ְ�
						victim.phase++; //�����ܰ� �̵�
					}
					//��Ʊ� ���� ũ��Ƽ�� ���� �� �����ð�
					else
					{
						victim.moveCounter++;
					}
				}

				if (victim.phase == 1)
				{
					//��ũ��Ƽ�� �����϶��� �ߵ��ϴ� �ڵ�
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
					//��ũ��Ƽ���� �ƴ� ���¸� �����ܰ�� �ѱ�
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
						assert(!"�ȸ���� �ִ°� üũ");
						break;
					}

					//�忩�� ����ī���� �ǵ帮�� �̵��Ÿ� ��������
					if (victim.moveCounter < 8) victim.moveCounter++;
					else victim.phase++;//�����ܰ� �̵�
				}

				//�� ������ ���κ�
				else if (victim.phase == 3)
				{
					if (victim.attackStatus == AttackStatue::Critical) //ũ���϶�
					{
						//�� ũ��Ƽ��, ������ 2��
						FLOAT calculateDamage = (victim.charPtr->GetDamage() * 2) - (attacker.charPtr->GetDefence());
						//calculateDamage = 0; //�׽�Ʈ��
						//calculateDamage = 100; //�׽�Ʈ��
						if (calculateDamage <= 0)
						{
							calculateDamage = 0;
							SOUNDMANAGER->play("CharacterDamage0");
						}
						else
						{
							//�幫��� //Swords > Axes //Axes > Lances //Lances > Swords
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
					else if (victim.attackStatus == AttackStatue::Normal) //�Ϲݰ����϶�
					{
						//�峪�� �������� ���߷� ���� �̳��� ��. ��� ������ 1��
						FLOAT calculateDamage = (victim.charPtr->GetDamage()) - (attacker.charPtr->GetDefence());

						//calculateDamage = 0; //�׽�Ʈ��
						//calculateDamage = 100; //�׽�Ʈ��

						if (calculateDamage <= 0)
						{
							calculateDamage = 0;
							SOUNDMANAGER->play("CharacterDamage0");
						}
						else
						{
							//�幫��� //Swords > Axes //Axes > Lances //Lances > Swords
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
					else if (victim.attackStatus == AttackStatue::Miss)	//�̿� ������ ������
					{
						SOUNDMANAGER->play("MissAttack");
					}
					else
					{
						assert(!"���ݻ��� ��������");
					}

					//��TODO �׼� �Ŀ� ������ ����
					victim.phase++;
				}

				//�嵹�ƿ��� �ڵ�
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
						assert(!"�ȸ���� �ִ°� üũ");
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
					//��ݰ����� ���� ����� ��Ұ�, �ݰݴ���� ������� ���ؿ´ٸ�
					if (attacker.charPtr->GetCurrentHealth() > 0 && attacker.isActionDone && attacker.charPtr->GetSpeed() + 5 > victim.charPtr->GetSpeed() && victim.attackCount == 0)
					{
						battleState = BattleState::BattleEnd;
					}
					//��ݰ����� ���� ����� ��Ұ�, �ݰݴ���� ������� �Ѵٸ�
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

					//��ݰ����� ���� ����� ��Ұ�, �ݰݴ���� �ӵ��� �� �����ں��� 5�̻� ������
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
					//��ݰ����� ���� ����� ��Ұ�, �ݰ��ڰ� 2Ÿ ��� ��������
					else if (attacker.charPtr->GetCurrentHealth() > 0 && attacker.charPtr->GetSpeed() + 5 <= victim.charPtr->GetSpeed() && victim.attackCount == 1)
					{
						battleState = BattleState::BattleEnd;
					}
					//������ڰ� ��������
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
			//������� �����ٸ�
			else
			{
				assert(!"������ �Ͽ� ��������");
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
			assert(false && "��Ʋ ������Ʈ ������� �ִµ�");
			break;
		}
		break;
	case ExecuteBattle::BattleMode::Full:
		break;
	default:
		assert(!"�����ϸ� �ȵ� ��Ʋ��尡 ����");
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