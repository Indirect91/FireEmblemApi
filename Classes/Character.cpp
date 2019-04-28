#include "../stdafx.h"
#include "Character.h"
#include "Item.h"
#include "Tiles.h"
#include "Cursor.h"

//▼생성자
Character::Character()
{
	whosChar = OwnedBy::Ally;
	draggingIndex = index;
	draggindDirection = DraggingDirection::LEFT;
	charStatus = CharStatus::Idle;
	isInCamera = false;
	occupation = Occupation::Archer;
	portraitImg = nullptr;
	portraitAlpha = 0.f;
	frameImg = nullptr;
	item = nullptr;

	isActionTaken = false;
	frame = { 0 };
	frameLoop = 0;
	frameCounter = 0;
	frameInterval = 5;

	occupationData = { 0 };
	ZeroMemory(&additionalData, sizeof(additionalData));
	memset(&baseData, 0, sizeof(baseData));

	RangeCalculator = 0;
	isMoveCalculated = false;
	isRangeCalculated = false;
	cursor = nullptr;
	dragValidity = false;
	isActionTaken = false;
}

//▼편리한 캐릭 생성을 위한 생성자 오버로딩
Character::Character(Occupation _job, std::string _charName, POINT _index, OwnedBy _whos) : whosChar(_whos)
{
	index = _index;
	draggingIndex = _index;
	draggindDirection = DraggingDirection::LEFT;
	charStatus = CharStatus::Idle;
	isInCamera = false;
	occupation = _job;
	portraitImg = nullptr;
	portraitAlpha = 0.f;
	frameImg = nullptr;
	item = nullptr;
	isActionTaken = false;
	frame.x = static_cast<UINT>(_whos);
	frame.y = static_cast<UINT>(_job);
	frameLoop = 0;
	frameCounter = 0;
	frameInterval = 5;

	occupationData = { 0 };
	ZeroMemory(&additionalData, sizeof(additionalData));
	memset(&baseData, 0, sizeof(baseData));

	RangeCalculator = 0;
	isMoveCalculated = false;
	isRangeCalculated = false;
	cursor = nullptr;
	dragValidity = false;
	isActionTaken = false;

	SetOccupation(_job);
	SetImg(_charName);
	SetPositionViaIndex();
}

//▼먼저 커서를 활성화 시키고 이닛을 통해 연결
void Character::Init()
{
	cursor = dynamic_cast<Cursor*>(DATACENTRE.GetCertainObject(ObjType::UI, "Cursor"));
	assert(cursor != nullptr); //커서 제대로 안들어왔으면 터뜨림
	isActionTaken = false;
}

void Character::Release()
{
	//아이템은 외부서 얻은걸 그냥 참조자나 포인터로 들고있는 방식이라 뉴할당을 여기서 안했음
}

void Character::Update()
{
	CheckInCamera(); //카메라부터 체크

	if (cursor->GetCursorTurn() == Cursor::CursorTurn::PlayerTurn && whosChar == OwnedBy::Player)
	{
		//▼먼저 상태를 준다
		if (isActionTaken) //행동을 취한적이 있는 캐릭터라면
		{
			charStatus = CharStatus::IsActed;
		} //행동한 상태로 유지
		else if (charStatus == CharStatus::IsClicked) //현 캐릭을 클릭 중이라
		{
			if (PointCompare(index, draggingIndex))
				charStatus = CharStatus::IsClicked;//클릭은 되었는데 아직 인덱스를 바꾼적이 없다면 유지
			else
				charStatus = CharStatus::IsDragging;//클릭 후 커서를 이동했을땐 드래깅
		}
		else if (charStatus == CharStatus::IsDragging)
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
			{
				charStatus = CharStatus::Idle;
			}
			else
			{
				charStatus = CharStatus::Disable;
			}
		}
	}
	else if (cursor->GetCursorTurn() == Cursor::CursorTurn::PlayerTurn && whosChar == OwnedBy::Enemy)
	{
		charStatus = CharStatus::IsCheckingOut;
	}
	else if (cursor->GetCursorTurn() == Cursor::CursorTurn::EnemyTurn)
	{

	}
	else
	{
		assert(false && "Turn Error");
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
			cursor->SetCursorOccupied("");
		}
		//TODO: 화살표
		draggingIndex = cursor->GetIndex();
		if (!PointCompare(index, cursor->GetIndex()))
		{
			charStatus = CharStatus::IsDragging;
			draggingIndex = cursor->GetIndex();
		}


		break;

		//▼커서가 올라와있는 상태라면.
	case CharStatus::IsCursorOn:
		if (!isMoveCalculated) //한번도 범위를 계산한적 없다면
		{
			ShowMoveRange(&Character::BlueRedShow); //범위 계산해서 보여줌
			isMoveCalculated = true; //한번 계산했다고 알림
		}

		//▼그 상태에서 마우스L버튼이나 A버튼이 들어오는지 체크
		if (KEYMANAGER->IsOnceKeyDown('A'))
		{
			charStatus = CharStatus::IsClicked; //눌렸다면 상태를 클릭된 상태로 바꿈

			for (auto& toThickenBlue : blueTiles)
			{
				toThickenBlue->SetBlueAlpha(0.8f);
			}
			for (auto& toThickenRed : redTiles)
			{
				toThickenRed->SetRedAlpha(0.8f);
			}
			for (auto& toThickenGreen : greenTiles)
			{
				toThickenGreen->SetGreenAlpha(0.8f);
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
			cursor->SetCursorOccupied("");
			cursor->SetIndex({ index.x,index.y });
			cursor->SetPositionViaIndex();
		}

		draggingIndex = cursor->GetIndex();

		break;
	case CharStatus::Disable:

		break;
	case CharStatus::IsActed:
		//TODO : ESC메뉴 연결점
		break;
	case CharStatus::IsCheckingOut:

		if (!isMoveCalculated) //한번도 범위를 계산한적 없다면
		{
			ShowMoveRange(&Character::BlueRedShow); //범위 계산해서 보여줌
			isMoveCalculated = true; //한번 계산했다고 알림
		}

		break;

	default:
		assert((BOOL)charStatus || "missing character status"); //스탯 안만든거 있다는 뜻이니 오면 터뜨림
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
			return isInCamera;
			break;
		}
	}
	return isInCamera;
}

//▼이동 범위 보이게 함
void Character::ShowMoveRange(void (Character:: * colourShow)(INT))
{

	//▼현 캐릭터의 행동 가능 범위를 표시함
	INT ActionRange = additionalData.Range + occupationData.Range + baseData.Range;  //사거리 or 힐범위
	INT MoveRange = additionalData.Move + occupationData.Move + baseData.Move;		//이동범위
	RangeCalculator = ActionRange + MoveRange;										//행동+이동범위

	//▼0이하의 수가 들어오면 터뜨림
	assert(ActionRange > 0);
	assert(MoveRange > 0);
	assert(RangeCalculator > 0);

	//▼연산용 임시 큐 생성. 큐 대신 스택 쓰면 Bfs(너비우선 알고리즘)가 아닌 Dfs()로 구현가능
	std::queue<Tiles*> BfsFloodFill;

	//▼시작 타일을 하나 넣어줌
	Tiles* initTile = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string(index.y * TILECOLX + index.x)));
	initTile->SetCheckedNum(RangeCalculator);//검사대상에 아까 계산해둔 최초 이동값 넣어둠
	BfsFloodFill.push(initTile);			//시작 타일 하나 넣어주고 시작

	while (!BfsFloodFill.empty())
	{
		//▼액션파트
		auto toExamen = BfsFloodFill.front();   //큐의 첫째 요소 확인 후 담아둠
		BfsFloodFill.pop();						//큐 첫째요소 터뜨림
		AvailableTiles.insert(toExamen);		//사용 가능한 타일에 넣어둠

		//▼위 검사
		bool UpValidity = true;
		UpValidity &= (0 <= toExamen->GetIndex().y - 1);
		UpValidity &= toExamen->GetCheckedNum() > 1;
		if (UpValidity)
		{
			Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string((toExamen->GetIndex().y - 1) * TILECOLX + toExamen->GetIndex().x)));
			UpValidity &= !checkTarget->GetCheckedNum();
			UpValidity &= checkTarget->GetObjT() == "";
			if (UpValidity)
			{
				for (auto& isEnemy : DATACENTRE.RefObjects(ObjType::EnemyArmy))
				{
					if (PointCompare(checkTarget->GetIndex(), isEnemy.second->GetIndex()))
					{
						redTiles.insert(checkTarget);
						UpValidity &= false;
					}
					else
					{
						UpValidity &= true;
					}
				}
			}
			if (UpValidity)
			{
				checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
				BfsFloodFill.push(checkTarget);
			}
		}

		//▼좌 검사
		bool LeftValidity = true;
		LeftValidity &= (0 <= toExamen->GetIndex().x - 1);
		LeftValidity &= toExamen->GetCheckedNum() > 1;
		if (LeftValidity)
		{
			Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string(toExamen->GetIndex().y * TILECOLX + toExamen->GetIndex().x - 1)));
			LeftValidity &= !checkTarget->GetCheckedNum();
			LeftValidity &= checkTarget->GetObjT() == "";
			if (LeftValidity)
			{
				for (auto& isEnemy : DATACENTRE.RefObjects(ObjType::EnemyArmy))
				{
					if (PointCompare(checkTarget->GetIndex(), isEnemy.second->GetIndex()))
					{
						redTiles.insert(checkTarget);
						LeftValidity &= false;
					}
					else
					{
						LeftValidity &= true;
					}
				}
			}

			if (LeftValidity)
			{
				checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
				BfsFloodFill.push(checkTarget);
			}
		}

		//▼아래 검사
		bool DownValidity = true;
		DownValidity &= (toExamen->GetIndex().y + 1 < TILEROWY);
		DownValidity &= toExamen->GetCheckedNum() > 1;
		if (DownValidity)
		{
			Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string((toExamen->GetIndex().y + 1) * TILECOLX + toExamen->GetIndex().x)));
			DownValidity &= !checkTarget->GetCheckedNum();
			DownValidity &= checkTarget->GetObjT() == "";
			if (DownValidity)
			{
				for (auto& isEnemy : DATACENTRE.RefObjects(ObjType::EnemyArmy))
				{
					if (PointCompare(checkTarget->GetIndex(), isEnemy.second->GetIndex()))
					{
						redTiles.insert(checkTarget);
						DownValidity &= false;
					}
					else
					{
						DownValidity &= true;
					}
				}
			}

			if (DownValidity)
			{
				checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
				BfsFloodFill.push(checkTarget);
			}
		}

		//▼우 검사
		bool RightValidity = true;
		RightValidity &= (toExamen->GetIndex().x + 1 < TILECOLX);
		RightValidity &= toExamen->GetCheckedNum() > 1;
		if (RightValidity)
		{
			Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string(toExamen->GetIndex().y * TILECOLX + toExamen->GetIndex().x + 1)));
			RightValidity &= !checkTarget->GetCheckedNum();
			RightValidity &= checkTarget->GetObjT() == "";
			if (RightValidity)
			{
				for (auto& isEnemy : DATACENTRE.RefObjects(ObjType::EnemyArmy))
				{
					if (PointCompare(checkTarget->GetIndex(), isEnemy.second->GetIndex()))
					{
						redTiles.insert(checkTarget);
						RightValidity &= false;
					}
					else
					{
						RightValidity &= true;
					}
				}
			}

			if (RightValidity)
			{
				checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
				BfsFloodFill.push(checkTarget);
			}
		}
	}
	(this->*colourShow)(ActionRange);
}
//▼이동 범위 안보이게 함
void Character::DisableMoveRange()
{
	if (isMoveCalculated)
	{
		for (auto& redTile : redTiles)
		{
			redTile->DecreaseRedNum();
		}

		for (auto& greenTile : greenTiles)
		{
			greenTile->DecreaseGreenNum();
		}

		for (auto& blueTile : blueTiles)
		{

			blueTile->DecreaseBlueNum();
		}
		blueTiles.clear();
		redTiles.clear();
		greenTiles.clear();

		AvailableTiles.clear();
		isMoveCalculated = false;
	}
}

void Character::DisableActionRange()
{
	if (isRangeCalculated)
	{
		for (auto& purpleTile : purpleTiles)
		{
			purpleTile->DecreasePurpleNum();
		}

		//isCalculated = false;
		isRangeCalculated = false;
	}
	purpleTiles.clear();
	blueTiles.clear();
	redTiles.clear();
	greenTiles.clear();

	AvailableTiles.clear();
}


void Character::ShowActionRange()
{
	ShowMoveRange(&Character::PurpleShow);
	isRangeCalculated = true;
}

//▼프레임돌리는 부분
void Character::AdjustFrame()
{
	frameCounter++; //프레임 항시 돌림

	//▼클릭된 상태일땐
	if (charStatus == CharStatus::IsClicked)
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
	if (charStatus == CharStatus::IsCursorOn || charStatus == CharStatus::IsClicked)
	{
		portraitAlpha += 0.1f;
		if (portraitAlpha >= 1) portraitAlpha = 1;
	}
	else
	{
		portraitAlpha -= 0.1f;
		if (portraitAlpha <= 0) portraitAlpha = 0;
	}
}

void Character::BlueRedShow(INT _actionRange)
{
	//▼연산 통해 담긴 행동범위에 따라 액션
	for (auto& toCal : AvailableTiles)
	{
		if (toCal->GetCheckedNum() <= _actionRange)
		{
			if (occupation == Occupation::Cleric || occupation == Occupation::WarCleric)
			{
				if (toCal->GetGreenAlpha() < 0.5f) toCal->SetGreenAlpha(0.4f);
				toCal->IncreaseGreenNum();	//그린 참조 갯수 증가
				greenTiles.insert(toCal);
				toCal->SetCheckedNum(0);
			}
			else
			{
				redTiles.insert(toCal);
			}
		}
		else
		{
			if (toCal->GetBlueAlpha() < 0.5f) toCal->SetBlueAlpha(0.4f);
			toCal->IncreaseBlueNum();	//블루 참조 갯수 증가
			toCal->SetCheckedNum(0);
			blueTiles.insert(toCal);
		}
	}
	for (auto& redTile : redTiles)
	{
		if (redTile->GetRedAlpha() < 0.5f) redTile->SetRedAlpha(0.4f);
		redTile->IncreaseRedNum();	//레드 참조 갯수 증가
		redTile->SetCheckedNum(0);
	}
}

void Character::PurpleShow(INT _actionRange)
{
	//▼연산 통해 담긴 행동범위에 따라 액션
	for (auto& purpleTile : AvailableTiles)
	{
		if (purpleTile->GetPurpleAlpha() < 0.5f) purpleTile->SetPurpleAlpha(0.4f);
		purpleTile->IncreasePurpleNum();	//레드 참조 갯수 증가
		purpleTile->SetCheckedNum(0);
		purpleTiles.insert(purpleTile);
	}
}


//▼직업 세팅용
void Character::SetOccupation(Occupation _job)
{
	this->occupation = _job;
	switch (this->occupation)
	{
	case Occupation::Swordsman:
		occupationData.Range = 1;
		occupationData.Health = 25;
		occupationData.Attack = 5;
		occupationData.Defence = 5;
		occupationData.Luck = 90;
		occupationData.Move = 4;
		break;
	case Occupation::Knight:
		occupationData.Range = 1;
		occupationData.Health = 25;
		occupationData.Attack = 5;
		occupationData.Defence = 5;
		occupationData.Luck = 90;
		occupationData.Move = 6;
		break;
	case Occupation::Mage:
		occupationData.Range = 2;
		occupationData.Health = 20;
		occupationData.Attack = 8;
		occupationData.Defence = 2;
		occupationData.Luck = 80;
		occupationData.Move = 4;
		break;
	case Occupation::Assassin:
		occupationData.Range = 1;
		occupationData.Health = 20;
		occupationData.Attack = 8;
		occupationData.Defence = 2;
		occupationData.Luck = 95;
		occupationData.Move = 4;
		break;
	case Occupation::Archer:
		occupationData.Range = 2;
		occupationData.Health = 15;
		occupationData.Attack = 8;
		occupationData.Defence = 2;
		occupationData.Luck = 70;
		occupationData.Move = 4;
		break;
	case Occupation::Cleric:
		occupationData.Range = 1;
		occupationData.Health = 15;
		occupationData.Attack = 2;
		occupationData.Defence = 1;
		occupationData.Luck = 100;
		occupationData.Move = 4;
		break;
	case Occupation::Sniper:
		occupationData.Range = 3;
		occupationData.Health = 15;
		occupationData.Attack = 8;
		occupationData.Defence = 2;
		occupationData.Luck = 70;
		occupationData.Move = 4;
		break;
	default:
		assert(false && "Occupation does not exist"); //존재하지 않는 직업 들어오면 터뜨림
		break;
	}
}

void Character::SetFrameAuto(Occupation _job, OwnedBy _whos)
{
	frame.x = static_cast<UINT>(_whos);
	frame.y = static_cast<UINT>(_job);
}

//▼주어진 이미지 이름으로 세팅
void Character::SetImg(std::string _CharName)
{
	portraitImg = IMAGEMANAGER->FindImage("초상화" + _CharName);
	frameImg = IMAGEMANAGER->FindImage("캐릭터" + _CharName);
}

//▼인자값 받아온거 알아서 세팅
void Character::SetInitialChar(Occupation _job, std::string _charName, POINT _index, OwnedBy _whos)
{
	whosChar = _whos;
	SetOccupation(_job);
	SetFrameAuto(_job, _whos);
	SetImg(_charName);
	SetIndex(_index);
	SetPositionViaIndex();
}

//▼그리기
void Character::Render()
{
	if (CheckInCamera())
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