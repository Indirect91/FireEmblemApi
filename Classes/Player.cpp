#include "../stdafx.h"
#include "Player.h"
#include "Character.h"


Player::Player()
{
	DATACENTRE.LoadFromFile();
}


void Player::Init()
{
	//플레이어의 부대는 데이터센터서 알아서 참조자로 들어가있다
	//플레이어의 아이템은 데이터센터서 알아서 참조자로 들어가있다
	//플레이어의 골드는 데이터 센터서 알아서 참조자로 들어가있다
	DATACENTRE.LoadFromFile();
	toMove = { 0,0,800,WINSIZEY };
}

void Player::Release()
{
	//플레이어에서 들고있는 부대,아이템은 외부 데이터를 참조자로 들고있기에 뉴할당을 하지 않았음.

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

