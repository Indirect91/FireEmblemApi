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
	//�÷��̾��� �δ�,������,���� �����ͼ��ͼ� ������ �ִ´�
	//DATACENTRE.LoadFromFile();
}

void Player::Release()
{
	//�÷��̾�� ����ִ� �δ�,�������� �ܺ� �����͸� �����ڷ� ����ֱ⿡ ���Ҵ��� ���� �ʾ���
}

void Player::Update()
{
	
	if (KEYMANAGER->IsToggleKey('M')) //���Ű�� M�� �����ִ��� ���θ� Ŀ������ �˸�
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

