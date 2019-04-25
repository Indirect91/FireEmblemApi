#include "../stdafx.h"
#include "Character.h"
#include "Item.h"
#include "Tiles.h"
#include "Cursor.h"

//▼어짜피 출고 후 덮어씌워질 애들이긴 함
Character::Character()
{
	draggingIndex = index;
	draggindDirection = DraggingDirection::LEFT;
	charStatus = CharStatus::Idle;
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

	moveRangeCalculator = 0;
	isCalculated = false;
	tmpCursor = nullptr;
}

//▼먼저 커서를 활성화 시키고 이닛을 통해 연결
void Character::Init()
{
	tmpCursor = dynamic_cast<Cursor*>(DATACENTRE.GetCertainObject(ObjType::UI, "Cursor"));
}

void Character::Release()
{
	//아이템은 외부서 얻은걸 그냥 참조자나 포인터로 들고있는 방식이라 뉴할당을 여기서 안했음
}

void Character::Update()
{	
	CheckInCamera(); //카메라부터 체크

	//▼먼저 상태를 준다
	if (charStatus == CharStatus::IsClicked) //현 캐릭을 클릭 중이라
	{
		if(PointCompare(index, draggingIndex))
			charStatus = CharStatus::IsClicked;//클릭은 되었는데 아직 인덱스를 바꾼적이 없다면 유지
		else
			charStatus = CharStatus::IsDragging;//클릭 후 커서를 이동했을땐 드래깅
	} 
	else if(charStatus == CharStatus::IsDragging)
	{
		charStatus = CharStatus::IsDragging;//클릭 후 커서를 이동했을땐 드래깅
	}
	else if (PointCompare(DATACENTRE.GetCertainObject(ObjType::UI, "Cursor")->GetIndex(), index)) //커서 온 상태도 유지. 강제로 커서타겟이 바뀌었을때 사용됨
	{
		charStatus = CharStatus::IsCursorOn;
	}
	else //클릭된 상태도, 커서on 상태도 아니라면
	{	
		if (isInCamera) //그 상태 아닐때 최소 카메라 속에라도 들어왔다면
			{charStatus = CharStatus::Idle;}
		else
			{charStatus = CharStatus::Disable;}
	}




	//▼상태에 따른 행동
	switch (charStatus)
	{
		//▼클릭된 상태라면. 
	case CharStatus::IsClicked:
		//▼우클릭이나 S를 누르면 픽상태 해지
		if (KEYMANAGER->IsOnceKeyDown('S'))
		{
			charStatus = CharStatus::IsCursorOn;
			DisableMoveRange();
			tmpCursor->SetCursorOccupied("");
		}
		//TODO: 화살표
		draggingIndex = tmpCursor->GetIndex();
		if (!PointCompare(index, tmpCursor->GetIndex()))
		{
			charStatus = CharStatus::IsDragging;
			draggingIndex = tmpCursor->GetIndex();
		}


		break;

		//▼커서가 올라와있는 상태라면.
	case CharStatus::IsCursorOn:
		if (!isCalculated) //한번도 범위를 계산한적 없다면
		{
			ShowMoveRange(); //범위 계산해서 보여줌
			isCalculated = true; //한번 계산했다고 알림
		}

		//▼그 상태에서 마우스L버튼이나 A버튼이 들어오는지 체크
		if (KEYMANAGER->IsOnceKeyDown('A'))
		{
			charStatus = CharStatus::IsClicked; //눌렸다면 상태를 클릭된 상태로 바꿈

			for (auto& toThicken : blueTiles)
			{
				toThicken->SetBlueAlpha(0.8f);
			}
		}
		break;

	//▼ 아이들 상태일때
	case CharStatus::Idle:
		DisableMoveRange();
		break;

	//▼ 드래깅 상태일때
	case CharStatus::IsDragging:
		//▼우클릭이나 S를 누르면 픽상태 해지
		if (KEYMANAGER->IsOnceKeyDown('S'))
		{
			charStatus = CharStatus::IsCursorOn;
			DisableMoveRange();
			tmpCursor->SetCursorOccupied("");
			tmpCursor->SetIndex({ index.x,index.y });
			tmpCursor->SetPositionViaIndex();
		}

		draggingIndex = tmpCursor->GetIndex();

		break;
	case CharStatus::Disable:

		break;
	}


	SetPositionViaIndex(); //추후 변경 필요
}

//▼카메라 속에 캐릭터가 있는지 체크하는 함수. 반환하기도 함
BOOL Character::CheckInCamera()
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
	return isInCamera;
}

//▼이동 범위 보이게 함
void Character::ShowMoveRange()
{
	//▼현 캐릭터의 움직임을 계산함
	moveRangeCalculator = AdditionalMove + classMove + baseMove;

	//▼이동값에 음수가 들어오면 터뜨림
	assert(moveRangeCalculator > -1);

	MakeItBlue(index, moveRangeCalculator);

	//▼재귀를 통해 담긴 블루백터에 계산하고 남은 부산물 뒤처리
	for (auto& blueTile : blueTiles)
	{
		blueTile->IncreaseBlueNum();	//블루 참조 갯수 증가
		blueTile->SetIsChecked(false);	
	}
}
//▼이동 범위 안보이게 함
void Character::DisableMoveRange()
{
	for (auto &blueTile : blueTiles)
	{
		blueTile->DecreaseBlueNum();
	}
	isCalculated = false;
	blueTiles.clear();
}

//▼타일 보이게 색칠하는 재귀함수
void Character::MakeItBlue(POINT _pos, UINT _move)
{
	//▼들어온 첫 타일 처리
	Tiles* startTile = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string(_pos.y * TILECOLX + _pos.x)));
	startTile->SetIsChecked(_move);										//체크에 숫자 대입
	if (startTile->GetBlueAlpha() < 0.5f) startTile->SetBlueAlpha(0.2f);//진하게 처리된게 아니라면 알파 옅게 세팅
	blueTiles.insert(startTile);										//처리된 타일 Set에 담아줌

	//▼위 검사
	BOOL UpValidity = true;
	UpValidity &= (0 <= _pos.y-1);
	UpValidity &= _move > 0;
	if (UpValidity)
	{ 
		Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string((_pos.y - 1) * TILECOLX + _pos.x))); 
		UpValidity &= !checkTarget->GetIsChecked();
		UpValidity &= checkTarget->GetObjT() == "";

		if (UpValidity)
		{MakeItBlue({ _pos.x,_pos.y - 1 }, _move - 1);}
	}
	
	//▼좌 검사
	BOOL LeftValidity = true;
	LeftValidity &= (0 <= _pos.x-1);
	LeftValidity &= _move > 0;
	if (LeftValidity)
	{
		Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string(_pos.y  * TILECOLX + _pos.x-1)));
		LeftValidity &= !checkTarget->GetIsChecked();
		LeftValidity &= checkTarget->GetObjT() == "";

		if (LeftValidity)
		{
			MakeItBlue({ _pos.x - 1,_pos.y }, _move - 1);
		}
	}

	//▼아래 검사
	BOOL DownValidity = true;
	DownValidity &= (_pos.y + 1 < TILEROWY);
	DownValidity &= _move > 0;
	if (DownValidity)
	{
		Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string((_pos.y + 1) * TILECOLX + _pos.x)));
		DownValidity &= !checkTarget->GetIsChecked();
		DownValidity &= checkTarget->GetObjT() == "";

		if (DownValidity)
		{
			MakeItBlue({ _pos.x,_pos.y + 1 }, _move - 1);
		}
	}

	//▼우 검사
	BOOL RightValidity = true;
	RightValidity &= (_pos.x + 1 < TILECOLX);
	RightValidity &= _move > 0;
	if (RightValidity)
	{
		Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string(_pos.y * TILECOLX + _pos.x+1)));
		RightValidity &= !checkTarget->GetIsChecked();
		RightValidity &= checkTarget->GetObjT() == "";

		if (RightValidity)
			{MakeItBlue({ _pos.x + 1,_pos.y }, _move - 1);}
	}
}

//▼프레임돌리는 부분
void Character::AdjustFrame()
{
	frameCounter++; //프레임 항시 돌림

	//▼클릭된 상태일땐
	if (charStatus==CharStatus::IsClicked)
	{
		frameInterval = 2; //정말 빠르게 움직인다
	}
	//▼액션을 이미 취한 상태라면
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
	if(charStatus==CharStatus::IsCursorOn || charStatus == CharStatus::IsClicked)
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
		classMove = 4;
		break;
	case Occupation::Knight:
		classHp = 25;
		classAttack = 5;
		classDefence = 5;
		classLuck = 5;
		classMove = 6;
		break;
	case Occupation::Mage:
		classHp = 20;
		classAttack = 8;
		classDefence = 2;
		classLuck = 5;
		classMove = 4;
		break;
	case Occupation::Rogue:
		classHp = 20;
		classAttack = 8;
		classDefence = 2;
		classLuck = 10;
		classMove = 4;
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
	if (charStatus == CharStatus::IsDragging)
	{
		frameImg->SetSize({ TILESIZE, TILESIZE });
		frameImg->SetAlpha(0.7f);
		frameImg->RelativeFrameRender(draggingIndex.x * TILESIZE, draggingIndex.y * TILESIZE, frame.x + frameLoop, frame.y + (UINT)draggindDirection);
	}
}