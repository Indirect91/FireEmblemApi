#include "../stdafx.h"
#include "Character.h"
#include "Item.h"


Character::Character()
{
	classes = Occupation::Infantary; //일단은 모든 캐릭은 보병으로 출고
	portaitImg = nullptr; 
	frameImg = nullptr;
	isActionTaken = false;
	frame = { 0 };

	attack = 0;
	defence = 0;
	luck = 0;
	move = 0;

	AdditionalAttack = 0;		//공격력
	AdditionalDefence = 0;		//방어력
	AdditionalLuck = 0;		//행운력. 회피율 및 크리티컬
	AdditionalMove = 0;		//이동력. 턴당 이동 가능한 타일 수

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
	
	portaitImg = IMAGEMANAGER->FindImage("초상화"+_CharName);
	frameImg = IMAGEMANAGER->FindImage("캐릭터" + _CharName);

}

void Character::Render()
{
	frameImg->FrameRender(this->position.left, this->position.top, 0, 0);
}
