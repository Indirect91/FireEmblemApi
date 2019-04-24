#include "../stdafx.h"
#include "Player.h"
#include "Character.h"
#include "Cursor.h"


Player::Player()
{
	DATACENTRE.LoadFromFile();
}


void Player::Init()
{
	//플레이어의 부대,아이템,골드는 데이터센터서 가져와 넣는다
	//DATACENTRE.LoadFromFile();
}

void Player::Release()
{
	//플레이어에서 들고있는 부대,아이템은 외부 데이터를 참조자로 들고있기에 뉴할당을 하지 않았음
}

void Player::Update()
{
	
	if (KEYMANAGER->IsToggleKey('M')) //토글키인 M이 눌려있는지 여부를 커서에게 알림
	{
		dynamic_cast<Cursor*>(cursor)->SetCursorStatus(Cursor::CursorState::cursorActivated);
	}
	else
	{
		dynamic_cast<Cursor*>(cursor)->SetCursorStatus(Cursor::CursorState::cursorDeactivated);
	}
	if (KEYMANAGER->IsOnceKeyDown(VK_RIGHT))
	{
		dynamic_cast<Cursor*>(cursor)->MoveRight();
	}
	else if (KEYMANAGER->IsOnceKeyDown(VK_LEFT))
	{
		dynamic_cast<Cursor*>(cursor)->MoveLeft();
	}
	if (KEYMANAGER->IsOnceKeyDown(VK_UP))
	{
		dynamic_cast<Cursor*>(cursor)->MoveUp();
	}
	else if (KEYMANAGER->IsOnceKeyDown(VK_DOWN))
	{
		dynamic_cast<Cursor*>(cursor)->MoveDown();
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

