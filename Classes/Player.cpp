#include "../stdafx.h"
#include "Player.h"


Player::Player()
{
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

}

