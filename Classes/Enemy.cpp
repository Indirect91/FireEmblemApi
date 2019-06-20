#include "../stdafx.h"
#include "Enemy.h"
#include "Character.h"
#include "Cursor.h"


Enemy::Enemy()
{
	cursor=nullptr;
}


void Enemy::Init()
{
	cursor = dynamic_cast<Cursor*>(DATACENTRE.GetCertainObject(ObjType::UI, "Cursor"));
	std::vector<Character> duplicate;
	for (auto& characters : enemyTroop) 
	{ 
		dynamic_cast<Character*>(characters.second)->Init(); 
	}
}

void Enemy::Release()
{
	//���ʹ̼� ���� ����� ����
}

void Enemy::Update()
{
	if (cursor->GetCursorOccupied() != "")
	{
		dynamic_cast<Character*>(DATACENTRE.GetCertainObject(ObjType::EnemyArmy, cursor->GetCursorOccupied()))->Update();
	}
	else
	{
		for (auto& character : enemyTroop) 
		{
			if (dynamic_cast<Character*>(character.second)->GetStatus()!=Character::CharStatus::IsActed)
			{
				cursor->SetCursorOccupied(character.first);
				break;
			}
			//dynamic_cast<Character*>(character.second)->Update();
		}
	}






	CAMERA.Follow(cursor->GetPosition()); //�� �Ͽ��� ���� ĳ���͸� ���� �����ش�
}

void Enemy::Render()
{
	for (auto& character : enemyTroop)
	{
		dynamic_cast<Character*>(character.second)->Render();
	}
}

