#include "../stdafx.h"
#include "Character.h"
#include "Item.h"
#include "Tiles.h"


Character::Character()
{
	classes = Occupation::Infantary; //일단은 모든 캐릭은 보병으로 출고
	portaitImg = nullptr; 
	frameImg = nullptr;
	isActionTaken = false;
	frame = { 0 };

	classHp = 0;
	classAttack = 0;
	classDefence = 0;
	classLuck = 0;
	classMove = 0;

	AdditionalHp = 0;
	AdditionalAttack = 0;		
	AdditionalDefence = 0;	
	AdditionalLuck = 0;		
	AdditionalMove = 0;	

	mouseOn = false;
}

void Character::재귀(POINT _index)
{
	this->index;
	//TODO
	TODO dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string(_index.y * TILEROWY + _index.x)))->SetStatus(Tiles::TileStatus::blue);
	TODO dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string(_index.y * TILEROWY + _index.x)))->SetStatus(Tiles::TileStatus::blue);

	//재귀();
}

void Character::Init()
{

}

void Character::Release()
{

}

void Character::Update()
{
	if (PtInRect(&position, _ptMouse))
	{
		mouseOn = true;
		
		if (KEYMANAGER->IsOnceKeyDown(VK_LBUTTON))
		{
			재귀(index);
		}
	}
	else
		mouseOn = false;
}

void Character::SetOccupation(Occupation _job)
{
	this->classes = _job;
	switch (this->classes)
	{
	case Occupation::Infantary:
		classHp = 25;
		classAttack = 5;
		classDefence = 5;
		classLuck = 5;
		classMove = 5;
		break;
	case Occupation::Knight:
		classHp = 25;
		classAttack = 5;
		classDefence = 5;
		classLuck = 5;
		classMove = 8;
		break;
	case Occupation::mage:
		classHp = 20;
		classAttack = 8;
		classDefence = 2;
		classLuck = 5;
		classMove = 5;
		break;
	case Occupation::Rogue:
		classHp = 20;
		classAttack = 8;
		classDefence = 2;
		classLuck = 10;
		classMove = 5;
		break;
	}
}

void Character::SetImg(std::string _CharName)
{
	portaitImg = IMAGEMANAGER->FindImage("초상화"+_CharName);
	frameImg = IMAGEMANAGER->FindImage("캐릭터" + _CharName);
}

void Character::Render()
{
	frameImg->SetSize({ TILESIZE, TILESIZE });
	if (mouseOn)
	{
		D2DRENDERER->FillRectangle(position, D2DRenderer::DefaultBrush::Green);
		IMAGEMANAGER->FindImage("SelectedTile")->SetSize({ TILESIZE, TILESIZE });
		IMAGEMANAGER->FindImage("SelectedTile")->Render(position.left, position.top);
	}
	frameImg->FrameRender(this->position.left, this->position.top, 0, 0);
}
