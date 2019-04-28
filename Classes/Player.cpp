#include "../stdafx.h"
#include "Player.h"
#include "Character.h"
#include "Cursor.h"
#include "Gold.h"

//���ϴ� ����� ���
Player::Player()
{
	playerGold = nullptr;
	cursor = nullptr;
	enemyRangeDetector = false;
	enemyRangeCalculated = "";
	enemyRangeCalculatedPrev = "";
}

//���̴ֽ� �����ͼ��Ϳ��� �������� �����
void Player::Init()
{
	//�÷��̾��� �δ�,������,���� �����ͼ��ͼ� ������ �ִ´�
	playerGold = dynamic_cast<Gold*>(DATACENTRE.GetCertainObject(ObjType::Gold, "playerGold"));
	cursor = dynamic_cast<Cursor*>(DATACENTRE.GetCertainObject(ObjType::UI, "Cursor"));
	for (auto& characters : playerTroop) { dynamic_cast<Character*>(characters.second)->Init(); }
	enemyRangeDetector = false;
	enemyRangeCalculated = "";
	enemyRangeCalculatedPrev = "";
}

//�帱����. ���Ҵ� ������ ���⼱ ����
void Player::Release()
{
	//�÷��̾�� ����ִ� �δ�,�������� �ܺ� �����͸� �����ڷ� ����ֱ⿡ ���Ҵ��� ���� �ʾ���
}

//�������Ʈ
void Player::Update()
{
	//���÷��̾��� ������Ʈ�� ���� ���� Ŀ���� �������� ĳ���Ͱ� �����Ҷ� Ű ������� �ش� ĳ���Ϳ��� �ѱ�
	if (cursor->GetCursorOccupied() != "")
	{
		//TODO:���� ť��Ʈ�÷��̾�ƹ̷� ��ü �ʿ�
		dynamic_cast<Character*>(DATACENTRE.GetCertainObject(ObjType::PlayerArmy, cursor->GetCursorOccupied()))->Update();

	}
	else
	{
		for (auto& character : playerTroop) //���� ��ƲƮ������ �ٲܰ�
		{
			if (dynamic_cast<Character*>(character.second)->GetIsClicked())
			{
				cursor->SetCursorOccupied(character.first);
				break;
			}
			dynamic_cast<Character*>(character.second)->Update();
		}
	}
	//�����Ű�� M�� �����ִ��� ���θ� Ŀ������ �˸�
	if (KEYMANAGER->IsToggleKey('M')) 
	{
		cursor->SetCursorStatus(Cursor::CursorState::cursorActivated);
	}
	else
	{
		cursor->SetCursorStatus(Cursor::CursorState::cursorDeactivated);
	}

	//��Ű���� ���� Ŀ�� �̵�. ���
	if (KEYMANAGER->IsOnceKeyDown(VK_RIGHT))
	{
		cursor->MoveRight();
	}
	else if (KEYMANAGER->IsOnceKeyDown(VK_LEFT))
	{
		cursor->MoveLeft();
	}
	if (KEYMANAGER->IsOnceKeyDown(VK_UP))
	{
		cursor->MoveUp();
	}
	else if (KEYMANAGER->IsOnceKeyDown(VK_DOWN))
	{
		cursor->MoveDown();
	}

	if (KEYMANAGER->IsOnceKeyDown('D'))
	{
		if (!enemyRangeDetector)
		{
			for (auto& eachEnemy : DATACENTRE.RefObjects(ObjType::EnemyArmy))
			{
				dynamic_cast<Character*>(eachEnemy.second)->ShowActionRange();
			}
			enemyRangeDetector = true;
		}
		else
		{
			enemyRangeDetector = false;
			for (auto& eachEnemy : DATACENTRE.RefObjects(ObjType::EnemyArmy))
			{
				dynamic_cast<Character*>(eachEnemy.second)->DisableActionRange();
			}
		}

	}

	enemyRangeCalculated = "";
	for (auto& enemyEncounter : DATACENTRE.RefObjects(ObjType::EnemyArmy))
	{
		if (PointCompare(cursor->GetIndex(), enemyEncounter.second->GetIndex()))
		{
			if (enemyRangeCalculated == "")
			{
				enemyRangeCalculated = enemyEncounter.first;
				enemyRangeCalculatedPrev = enemyRangeCalculated;
				break;
			}
		}
	}
	
	if (enemyRangeCalculated != "")
	{
		if (cursor->GetCursorOccupied() == "")
		{
			dynamic_cast<Character*>(DATACENTRE.GetCertainObject(ObjType::EnemyArmy, enemyRangeCalculated))->Update();
		}
	}
	else if (enemyRangeCalculated=="" && enemyRangeCalculatedPrev!="")
	{
		dynamic_cast<Character*>(DATACENTRE.GetCertainObject(ObjType::EnemyArmy, enemyRangeCalculatedPrev))->DisableMoveRange();
		enemyRangeCalculatedPrev = "";
	}


	CAMERA.Follow(cursor->GetPosition());
}

void Player::Render()
{
	for (auto &character : playerTroop)
	{
		dynamic_cast<Character*>(character.second)->Render();
	}
}

