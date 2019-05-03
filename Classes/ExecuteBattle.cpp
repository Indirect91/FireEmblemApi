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
			cursor->SetCursorTurn(cursor->GetCursorTurnPrev());

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
					attacker.hitDistinguisher = (rand() % 100) + 1;					//1~100 ������
					
					attacker.hitDistinguisher = 5;		//�׽�Ʈ��. ũ��
					//hitDistinguisher = 95;	//�׽�Ʈ��. �̽�
					//hitDistinguisher = 80;	//�׽�Ʈ��. ���

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
						{assert(!"Wrong Angle");}
					
					victim.charPtr->SetMovingDirection(victim.attackingDirection);
					victim.charPtr->SetStatus(Character::CharStatus::IsAttacking);
					
					//�̵� �Ϸ� �� ���� �ð� �Ŀ� ����
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
						attacker.phase++; //�����ܰ� �̵�
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
						assert(!"�ȸ���� �ִ°� üũ");
						break;
					}

					//�忩�� ����ī���� �ǵ帮�� �̵��Ÿ� ��������
					if (attacker.moveCounter < 8) attacker.moveCounter++;
					else attacker.phase++;//�����ܰ� �̵�
				}

				//�� ������ ���κ�
				else if (attacker.phase == 2)
				{
					if (attacker.attackStatus==AttackStatue::Critical) //ũ���϶�
					{
						//�� ũ��Ƽ��, ������ 2��
						FLOAT calculateDamage = (attacker.charPtr->GetDamage() * 2) - (victim.charPtr->GetDefence());
						//calculateDamage = 0; //�׽�Ʈ��
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
					else if(attacker.attackStatus == AttackStatue::Miss)	//�̿� ������ ������
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
						assert(!"�ȸ���� �ִ°� üũ");
						break;
					}
					if (attacker.moveCounter >= 0) attacker.moveCounter--;
					else attacker.phase++;
				}
				else if (attacker.phase ==4)
				{
					//���ٿԴ� ��� ���� ��


					//exit(0);
				}
			}
			//������� �����ٸ�
			else
			{

			}


			break;
		}
		case ExecuteBattle::BattleState::VictimAttacking:



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
	
}