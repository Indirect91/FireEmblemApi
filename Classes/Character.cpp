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

	baseHp = 0;		
	baseAttack = 0;
	baseDefence = 0;
	baseLuck = 0;
	baseMove = 0;

	mouseOn = false;
	moveRangeCalculator = 0;
	isChecking = false;
}



void Character::Init()
{
	SetOccupation(Occupation::Infantary);
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
			if (this->isChecking ==false)
			{
				isChecking = true;
				ShowMoveRange();
			}
			else
			{
				this->isChecking = false;
				DisableRange();
			}
		}
	}
	else
		mouseOn = false;

	SetPositionViaIndex();
}


void Character::ShowMoveRange()
{
	moveRangeCalculator = AdditionalMove + classMove + baseMove +2;
	assert(moveRangeCalculator > -1);

	MakeItBlue(index, moveRangeCalculator);
}
void Character::DisableRange()
{
	for (auto &blueTile : blueTiles)
	{
		blueTile->SetIsChecked(false);
		blueTile->DecreaseBlueNum();
	}
	blueTiles.clear();
}
void Character::MakeItBlue(POINT _pos, UINT _move)
{
	if (((0 <= _pos.x) && (_pos.x < TILECOLX)) && (0 <= _pos.y) && (_pos.y < TILEROWY))
	{
		Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string(_pos.y * TILECOLX + _pos.x)));
		BOOL recursionContinue = true;
		recursionContinue &= checkTarget->GetIsChecked() == false;
		recursionContinue &= checkTarget->GetObj() == "";
		recursionContinue &= _move > 1;

		if (recursionContinue)
		{
			checkTarget->IncreaseBlueNum();
			checkTarget->SetIsChecked(true);
			blueTiles.push_back(checkTarget); //나중에 파란처리 되어있는 

			MakeItBlue({ _pos.x,_pos.y - 1 }, _move - 1);
			MakeItBlue({ _pos.x - 1,_pos.y }, _move - 1);
			MakeItBlue({ _pos.x,_pos.y + 1 }, _move - 1);
			MakeItBlue({ _pos.x + 1,_pos.y }, _move - 1);
		}
	}
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
	portaitImg = IMAGEMANAGER->FindImage("초상화" + _CharName);
	frameImg = IMAGEMANAGER->FindImage("캐릭터" + _CharName);
}

//▼인자값 받아온거 알아서 세팅
void Character::SetInitialChar(Occupation _job, POINT _frame, std::string _charName, POINT _index)
{
	SetOccupation(_job);
	SetFrame(_frame);
	SetImg(_charName);
	SetIndex(_index);
	SetPositionViaIndex();
}

void Character::Render()
{
	frameImg->SetSize({ TILESIZE, TILESIZE });
	if (mouseOn)
	{
		IMAGEMANAGER->FindImage("SelectedTile")->SetSize({ TILESIZE, TILESIZE });
		IMAGEMANAGER->FindImage("SelectedTile")->Render(position.left, position.top);
	}
	frameImg->FrameRender(this->position.left, this->position.top, frame.x, frame.y);
}