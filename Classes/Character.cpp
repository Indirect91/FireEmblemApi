#include "../stdafx.h"
#include "Character.h"
#include "Item.h"


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

	AdditionalAttack = 0;		//���ݷ�
	AdditionalDefence = 0;		//����
	AdditionalLuck = 0;		//����. ȸ���� �� ũ��Ƽ��
	AdditionalMove = 0;		//�̵���. �ϴ� �̵� ������ Ÿ�� ��

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

void Character::SetOccupation(Occupation _job)
{

}

void Character::SetFrame(POINT _frame)
{

}

void Character::SetImg(std::string _CharName)
{

}

void Character::Render()
{

}
