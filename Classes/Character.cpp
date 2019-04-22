#include "../stdafx.h"
#include "Character.h"
#include "Item.h"
#include "Tiles.h"


Character::Character()
{
	classes = Occupation::Infantary; //�ϴ��� ��� ĳ���� �������� ���
	portraitImg = nullptr;
	portraitAlpha = 0.f;
	frameImg = nullptr;
	item = nullptr;

	isActionTaken = false;
	frame = { 0 };
	frameLoop = 0;
	frameCounter = 0;
	frameInterval = 5;

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

	isClicked = false;
	moveRangeCalculator = 0;
	isCalculated = false;
	isMouseOn = false;
}


void Character::Init()
{
	classes = Occupation::Infantary;
	portraitImg = nullptr;
	portraitAlpha = 0.f;
	frameImg = nullptr;
	item = nullptr;

	isActionTaken = false;
	frame = { 0 };
	frameLoop = 0;
	frameCounter = 0;
	frameInterval = 5;

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

	isClicked = false;
	moveRangeCalculator = 0;
	isCalculated = false;
	isMouseOn = false;
}

void Character::Release()
{
	//�������� �ܺμ� ������ �׳� �����ڳ� �����ͷ� ����ִ� ����̶� ���Ҵ��� ���⼭ ������
}

void Character::Update()
{
	if (PtInRect(&position, _ptMouse))
	{
		isMouseOn = true;
		
		if (KEYMANAGER->IsOnceKeyDown(VK_LBUTTON))
		{
			isClicked = true;
			ShowMoveRange();
			for (auto &toThicken: blueTiles)
			{
				toThicken->SetBlueAlpha(0.8);
			}
		}
	}
	else if (isClicked && KEYMANAGER->IsOnceKeyDown(VK_RBUTTON))
	{
		isClicked = false;
		DisableMoveRange();
	}
	else
	{
		isMouseOn = false;
	}


	if (isMouseOn && !isCalculated) //���콺�� �ö���ְ� �ѹ��� ����� ��� ���ѻ��¶��
	{
		ShowMoveRange(); //���� ����ؼ� ������
		isCalculated = true; //�ѹ� ����ߴٰ� �˸�
	}
	else if (!isMouseOn && !isClicked) //���콺�� �ö������ �ʰ�, ĳ���� Ŭ�����µ� �ƴҋ�
	{
		isCalculated = false;
		DisableMoveRange();
	}

	
	SetPositionViaIndex(); //���� ���� �ʿ�
}

//���̵� ���� ���̰� ��
void Character::ShowMoveRange()
{
	moveRangeCalculator = AdditionalMove + classMove + baseMove +2;
	assert(moveRangeCalculator > -1);

	MakeItBlue(index, moveRangeCalculator);

	for (auto& blueTile : blueTiles)
	{blueTile->SetIsChecked(false);}
}
//���̵� ���� �Ⱥ��̰� ��
void Character::DisableMoveRange()
{
	for (auto &blueTile : blueTiles)
	{blueTile->DecreaseBlueNum();}
	blueTiles.clear();
}

//��Ÿ�� ���̰� ��ĥ�ϴ� ����Լ�
BOOL Character::MakeItBlue(POINT _pos, UINT _move)
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
			checkTarget->SetBlueAlpha(0.2f);
			blueTiles.push_back(checkTarget); //���߿� �Ķ�ó�� �Ǿ��ִ� 

			MakeItBlue({ _pos.x,_pos.y - 1 }, _move - 1);
			MakeItBlue({ _pos.x - 1,_pos.y }, _move - 1);
			MakeItBlue({ _pos.x,_pos.y + 1 }, _move - 1);
			MakeItBlue({ _pos.x + 1,_pos.y }, _move - 1);
		}
	}
}

//�������ӵ����� �κ�
void Character::AdjustFrame()
{
	frameCounter++;

	if (isClicked) frameInterval = 2;
	else frameInterval = 7;

	if (frameCounter % frameInterval == 0) //5�����Ӹ���.
	{
		frameLoop++; //������ �̹��� ����
		if (frameLoop >= 4) frameLoop = 0; //��� ĳ���� �̹����� 0~3����
	}

	if (isMouseOn||isClicked)
	{
		portraitAlpha += 0.1f;
		if(portraitAlpha>=1) portraitAlpha = 1;
	}
	else
	{
		portraitAlpha -= 0.1f;
		if (portraitAlpha <= 0) portraitAlpha = 0;
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
	portraitImg = IMAGEMANAGER->FindImage("�ʻ�ȭ" + _CharName);
	frameImg = IMAGEMANAGER->FindImage("ĳ����" + _CharName);
}

//�����ڰ� �޾ƿ°� �˾Ƽ� ����
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
	AdjustFrame(); //������ ������ �κ��� ���� �ƴҋ��� ���ư����ϴ� ������ ���� 
	frameImg->SetSize({ TILESIZE, TILESIZE });
	frameImg->FrameRender(this->position.left, this->position.top, frame.x + frameLoop, frame.y);

	portraitImg->SetAlpha(portraitAlpha);
	if(index.x ==6 && index.y == 6) portraitImg-> Render(700, 300);
	else portraitImg->Render(700, 100);
}