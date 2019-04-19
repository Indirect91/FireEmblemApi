#include "../stdafx.h"
#include "Player.h"
#include "Character.h"


Player::Player()
{
	DATACENTRE.LoadFromFile();
	playerGold = DATACENTRE.GetCertainObject(ObjType::Gold, "playerGold");
}


void Player::Init()
{
	for (auto& units : playerTroop) units.second->Init();
}

void Player::Release()
{
	//플레이어에서 뉴할당 해준거 없음
}

void Player::Update()
{
	if (KEYMANAGER->IsOnceKeyDown(VK_RIGHT))
	{
		for (auto& character : playerTroop)
		{
			if (character.first == "Chrome")
			{
				dynamic_cast<Character*>(character.second)->SetIndex({ dynamic_cast<Character*>(character.second)->GetIndex().x+1, dynamic_cast<Character*>(character.second)->GetIndex().y });
			}
		}
	}
	else if (KEYMANAGER->IsOnceKeyDown(VK_LEFT))
	{
		for (auto& character : playerTroop)
		{
			if (character.first == "Chrome")
			{
				dynamic_cast<Character*>(character.second)->SetIndex({ dynamic_cast<Character*>(character.second)->GetIndex().x - 1, dynamic_cast<Character*>(character.second)->GetIndex().y });
			}
		}
	}
	else if (KEYMANAGER->IsOnceKeyDown(VK_UP))
	{
		for (auto& character : playerTroop)
		{
			if (character.first == "Chrome")
			{
				dynamic_cast<Character*>(character.second)->SetIndex({ dynamic_cast<Character*>(character.second)->GetIndex().x, dynamic_cast<Character*>(character.second)->GetIndex().y-1 });
			}
		}
	}
	else if (KEYMANAGER->IsOnceKeyDown(VK_DOWN))
	{
		for (auto& character : playerTroop)
		{
			if (character.first == "Chrome")
			{
				dynamic_cast<Character*>(character.second)->SetIndex({ dynamic_cast<Character*>(character.second)->GetIndex().x , dynamic_cast<Character*>(character.second)->GetIndex().y +1 });
			}
		}
	}


	for (auto& character : playerTroop)
	{
		dynamic_cast<Character*>(character.second)->Update();
	}
}

void Player::Render()
{
	for (auto &character : playerTroop)
	{
		dynamic_cast<Character*>(character.second)->Render();
	}
}

