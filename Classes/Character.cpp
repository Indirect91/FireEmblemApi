#include "../stdafx.h"
#include "Character.h"



Character::Character()
{
	classes = Occupation::Infantary; //�ϴ��� ��� ĳ���� �������� ���
	portaitImg = nullptr; 
	frameImg = nullptr;
	isActionTaken = false;
	frameX = 0;
	frameY = 0;

	attack = 0;
	defence = 0;
	luck = 0;
	move = 0;

}

void Character::Init()
{
}

void Character::Release()
{
}

void Character::Update()
{
}

void Character::Render()
{
}

