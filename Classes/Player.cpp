#include "../stdafx.h"
#include "Player.h"
#include "Character.h"


Player::Player()
{
	DATACENTRE.LoadFromFile();
}


void Player::Init()
{
	//�÷��̾��� �δ�,������,���� �����ͼ��ͼ� �˾Ƽ� �����ڷ� ���ִ�
	DATACENTRE.LoadFromFile();
	toMove = { 0,0,800,WINSIZEY };
}

void Player::Release()
{
	//�÷��̾�� ����ִ� �δ�,�������� �ܺ� �����͸� �����ڷ� ����ֱ⿡ ���Ҵ��� ���� �ʾ���.

}

void Player::Update()
{
	
	if (KEYMANAGER->IsStayKeyDown(VK_RIGHT))
	{
		toMove.left += 5;
		toMove.right += 5;
	}
	else if (KEYMANAGER->IsStayKeyDown(VK_LEFT))
	{
		toMove.left -= 5;
		toMove.right -= 5;

	}
	if (KEYMANAGER->IsStayKeyDown(VK_UP))
	{
		toMove.top -= 5;
		toMove.bottom -= 5;
	}
	else if (KEYMANAGER->IsStayKeyDown(VK_DOWN))
	{
		toMove.top += 5;
		toMove.bottom += 5;
	}

	for (auto& character : playerTroop)
	{
		dynamic_cast<Character*>(character.second)->Update();
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

