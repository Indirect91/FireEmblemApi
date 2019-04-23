#include "../stdafx.h"
#include "Player.h"
#include "Character.h"


Player::Player()
{
	DATACENTRE.LoadFromFile();
}


void Player::Init()
{
	//�÷��̾��� �δ�� �����ͼ��ͼ� �˾Ƽ� �����ڷ� ���ִ�
	//�÷��̾��� �������� �����ͼ��ͼ� �˾Ƽ� �����ڷ� ���ִ�
	//�÷��̾��� ���� ������ ���ͼ� �˾Ƽ� �����ڷ� ���ִ�
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
		//for (auto& character : playerTroop)
		//{
		//	if (character.first == "Chrome")
		//	{
		//		dynamic_cast<Character*>(character.second)->SetIndex({ dynamic_cast<Character*>(character.second)->GetIndex().x+1, dynamic_cast<Character*>(character.second)->GetIndex().y });
		//	}
		//}
		toMove.left += 5;
		toMove.right += 5;


	}
	else if (KEYMANAGER->IsStayKeyDown(VK_LEFT))
	{
		//for (auto& character : playerTroop)
		//{
		//	if (character.first == "Chrome")
		//	{
		//		dynamic_cast<Character*>(character.second)->SetIndex({ dynamic_cast<Character*>(character.second)->GetIndex().x - 1, dynamic_cast<Character*>(character.second)->GetIndex().y });
		//	}
		//}

		toMove.left -= 5;
		toMove.right -= 5;

	}
	if (KEYMANAGER->IsStayKeyDown(VK_UP))
	{
		//for (auto& character : playerTroop)
		//{
		//	if (character.first == "Chrome")
		//	{
		//		dynamic_cast<Character*>(character.second)->SetIndex({ dynamic_cast<Character*>(character.second)->GetIndex().x, dynamic_cast<Character*>(character.second)->GetIndex().y-1 });
		//	}
		//}

		toMove.top -= 5;
		toMove.bottom -= 5;
	}
	else if (KEYMANAGER->IsStayKeyDown(VK_DOWN))
	{
		//for (auto& character : playerTroop)
		//{
		//	if (character.first == "Chrome")
		//	{
		//		dynamic_cast<Character*>(character.second)->SetIndex({ dynamic_cast<Character*>(character.second)->GetIndex().x , dynamic_cast<Character*>(character.second)->GetIndex().y +1 });
		//	}
		//}

		toMove.top += 5;
		toMove.bottom += 5;
	}

	for (auto& character : playerTroop)
	{
		dynamic_cast<Character*>(character.second)->Update();
	}

	CAMERA.SetCamera(toMove);
}

void Player::Render()
{
	for (auto &character : playerTroop)
	{
		dynamic_cast<Character*>(character.second)->Render();
	}
}

