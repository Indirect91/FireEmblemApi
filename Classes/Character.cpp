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
	occupation = Occupation::Infantary; //일단은 모든 캐릭은 보병으로 출고
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

	RangeCalculator = 0;
	isCalculated = false;
	tmpCursor = nullptr;
	dragValidity = false;
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

			for (auto& toThicken : AvailableTiles)
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
	//▼현 캐릭터의 행동 가능 범위를 표시함
	INT ActionRange = (((UINT)occupation) % (UINT)Occupation::RangeSeparater) + 1;  //사거리 or 힐범위
	INT MoveRange = AdditionalMove + classMove + baseMove;							//이동범위
	RangeCalculator = ActionRange + MoveRange;										//행동+이동범위

	//▼0이하의 수가 들어오면 터뜨림
	assert(ActionRange > 0);
	assert(MoveRange > 0);
	assert(RangeCalculator > 0);

	//▼연산용 임시 큐 생성. 큐 대신 스택 쓰면 Bfs(너비우선 알고리즘)가 아닌 Dfs()로 구현가능
	std::queue<Tiles *> BfsFloodFill;

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
		BOOL UpValidity = true;
		UpValidity &= (0 <= toExamen->GetIndex().y - 1);
		UpValidity &= toExamen->GetCheckedNum() > 0;
		if (UpValidity)
		{
			Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string((toExamen->GetIndex().y - 1) * TILECOLX + toExamen->GetIndex().x)));
			UpValidity &= !checkTarget->GetCheckedNum();
			UpValidity &= checkTarget->GetObjT() == "";

			if (UpValidity)
			{
				checkTarget->SetCheckedNum(toExamen->GetCheckedNum()-1);
				BfsFloodFill.push(checkTarget);
			}
		}

		//▼좌 검사
		BOOL LeftValidity = true;
		LeftValidity &= (0 <= toExamen->GetIndex().x - 1);
		LeftValidity &= toExamen->GetCheckedNum() > 0;
		if (LeftValidity)
		{
			Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string(toExamen->GetIndex().y * TILECOLX + toExamen->GetIndex().x - 1)));
			LeftValidity &= !checkTarget->GetCheckedNum();
			LeftValidity &= checkTarget->GetObjT() == "";

			if (LeftValidity)
			{
				checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
				BfsFloodFill.push(checkTarget);
			}
		}

		//▼아래 검사
		BOOL DownValidity = true;
		DownValidity &= (toExamen->GetIndex().y + 1 < TILEROWY);
		DownValidity &= toExamen->GetCheckedNum() > 0;
		if (DownValidity)
		{
			Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string((toExamen->GetIndex().y + 1) * TILECOLX + toExamen->GetIndex().x)));
			DownValidity &= !checkTarget->GetCheckedNum();
			DownValidity &= checkTarget->GetObjT() == "";

			if (DownValidity)
			{
				checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
				BfsFloodFill.push(checkTarget);
			}
		}

		//▼우 검사
		BOOL RightValidity = true;
		RightValidity &= (toExamen->GetIndex().x + 1 < TILECOLX);
		RightValidity &= toExamen->GetCheckedNum() > 0;
		if (RightValidity)
		{
			Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string(toExamen->GetIndex().y * TILECOLX + toExamen->GetIndex().x + 1)));
			RightValidity &= !checkTarget->GetCheckedNum();
			RightValidity &= checkTarget->GetObjT() == "";

			if (RightValidity)
			{
				checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
				BfsFloodFill.push(checkTarget);
			}
		}
	}

	//▼연산 통해 담긴 행동범위에 따라 액션
	for (auto& blueTile : AvailableTiles)
	{
		if (blueTile->GetBlueAlpha() < 0.5f) blueTile->SetBlueAlpha(0.2f);
		blueTile->IncreaseBlueNum();	//블루 참조 갯수 증가
		blueTile->SetCheckedNum(0);	
	}
}
//▼이동 범위 안보이게 함
void Character::DisableMoveRange()
{
	for (auto &blueTile : AvailableTiles)
	{
		blueTile->DecreaseBlueNum();
	}
	isCalculated = false;
	AvailableTiles.clear();
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
	this->occupation = _job;
	switch (this->occupation)
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
	case Occupation::Archer:
		classHp = 15;
		classAttack = 8;
		classDefence = 2;
		classLuck = 10;
		classMove = 4;
		break;
	default:
		assert("Occupation does not exist");
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