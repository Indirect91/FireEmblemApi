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
	//�÷��̾�� ���Ҵ� ���ذ� ����
}

void Player::Update()
{

}

void Player::Render()
{
	for (auto &character : playerTroop)
	{
		dynamic_cast<Character*>(character.second)->Render();
	}
}

