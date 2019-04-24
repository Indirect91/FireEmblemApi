#include "../stdafx.h"
#include "Character.h"
#include "Item.h"
#include "Tiles.h"
#include "Cursor.h"


Character::Character()
{
	isInCamera = false;
	classes = Occupation::Infantary; //일단은 모든 캐릭은 보병으로 출고
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
	isCursorOn = false;
}


void Character::Init()
{
	isInCamera = false;
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
	isCursorOn = false;
}

void Character::Release()
{
	//아이템은 외부서 얻은걸 그냥 참조자나 포인터로 들고있는 방식이라 뉴할당을 여기서 안했음
}

void Character::Update()
{
	CheckInCamera();

	if (isInCamera) //카메라에 잡혀있을때만 업데이트
	{
		if(PointCompare(DATACENTRE.GetCertainObject(ObjType::UI,"Cursor")->GetIndex(),index))
		{
			isCursorOn = true;
			if (KEYMANAGER->IsOnceKeyDown(VK_LBUTTON || 'A'))
			{
				isClicked = true;
				ShowMoveRange();
				for (auto& toThicken : blueTiles)
				{
					toThicken->SetBlueAlpha(0.8f);
				}
			}
		}
		else
		{
			isCursorOn = false;
		}
		
		if (isClicked && KEYMANAGER->IsOnceKeyDown(VK_RBUTTON || 'S'))
		{
			isClicked = false;
			DisableMoveRange();
		}



		if (isCursorOn && !isCalculated) //마우스가 올라와있고 한번도 쇼레인지 계산 안한상태라면
		{
			ShowMoveRange(); //범위 계산해서 보여줌
			isCalculated = true; //한번 계산했다고 알림
		}
		else if (!isCursorOn && !isClicked) //마우스가 올라와있지 않고, 캐릭터 클릭상태도 아닐떄
		{
			isCalculated = false;
			DisableMoveRange();
		}
		SetPositionViaIndex(); //추후 변경 필요
	}
}

//▼카메라 속에 캐릭터가 있는지 체크하는 함수
void Character::CheckInCamera()
{
	isInCamera = false;
	for (auto& toCheck : DATACENTRE.RefObjects(ObjType::ClippedTile))
	{
		if (toCheck.first == std::to_string(index.y * TILECOLX + index.x))
		{
			isInCamera = true;
			break;
		}
	}
}

//▼이동 범위 보이게 함
void Character::ShowMoveRange()
{
	moveRangeCalculator = AdditionalMove + classMove + baseMove +2;
	assert(moveRangeCalculator > -1);

	MakeItBlue(index, moveRangeCalculator);

	for (auto& blueTile : blueTiles)
	{blueTile->SetIsChecked(false);}
}
//▼이동 범위 안보이게 함
void Character::DisableMoveRange()
{
	for (auto &blueTile : blueTiles)
	{
		blueTile->DecreaseBlueNum();
	}
	blueTiles.clear();
}

//▼타일 보이게 색칠하는 재귀함수
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
			if(checkTarget->GetBlueAlpha()<0.5f) checkTarget->SetBlueAlpha(0.2f);
			blueTiles.push_back(checkTarget); //나중에 파란처리 되어있는 

			MakeItBlue({ _pos.x,_pos.y - 1 }, _move - 1);
			MakeItBlue({ _pos.x - 1,_pos.y }, _move - 1);
			MakeItBlue({ _pos.x,_pos.y + 1 }, _move - 1);
			MakeItBlue({ _pos.x + 1,_pos.y }, _move - 1);
		}
	}
}

//▼프레임돌리는 부분
void Character::AdjustFrame()
{
	frameCounter++;

	if (isClicked)
	{
		frameInterval = 2;
	}
	else if (isActionTaken)
	{
		frameInterval = 16;
	}
	else
	{
		frameInterval = 7;
	}

	if (frameCounter % frameInterval == 0) //정해진 프레임간격에 따라서
	{
		frameLoop++; //프레임 이미지 증가
		if (frameLoop >= 4) frameLoop = 0; //모든 캐릭터 이미지는 0~3범위
	}

	//추후 UI로 뺄 부분
	if (isCursorOn||isClicked)
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
	portraitImg = IMAGEMANAGER->FindImage("초상화" + _CharName);
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
	if (isInCamera)
	{
		AdjustFrame(); //프레임 돌리는 부분은 턴이 아닐떄도 돌아가야하니 랜더에 상주 

		frameImg->SetSize({ TILESIZE, TILESIZE });
		frameImg->RelativeFrameRender(position.left, position.top, frame.x + frameLoop, frame.y);

		if (isActionTaken)
		{
			IMAGEMANAGER->FindImage("ActionTaken")->SetAlpha(0.5f);
			IMAGEMANAGER->FindImage("ActionTaken")->SetSize({ TILESIZE, TILESIZE });
			IMAGEMANAGER->FindImage("ActionTaken")->RelativeRender(position.left, position.top);
		}

		portraitImg->SetAlpha(portraitAlpha);
		if (index.x == 6 && index.y == 6)
		{
			portraitImg->Render(900, 300);
		}
		else
		{
			portraitImg->Render(900, 100);
		}
	}
}