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
	draggindDirection = DraggingDirection::IDLE;
	movingDirection = MovingDirection::IDLE;
	charStatus = CharStatus::Idle;
	isInCamera = false;
	occupation = Occupation::Archer;
	portraitImg = nullptr;
	portraitAlpha = 0.f;
	frameImg = nullptr;
	item = nullptr;
	draggingStarted = nullptr;
	isActionTaken = false;
	frame = { 0 };
	frameLoop = 0;
	frameCounter = 0;
	frameInterval = 5;

	occupationData = { 0 };
	ZeroMemory(&additionalData, sizeof(additionalData));
	memset(&baseData, 0, sizeof(baseData));
	draggingIndexPrev = {0};
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
	draggindDirection = DraggingDirection::IDLE;
	movingDirection = MovingDirection::IDLE;
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

	if (cursor->GetCursorTurn() == IngameStatus::PlayerTurn && whosChar == OwnedBy::Player)
	{
		//▼먼저 상태를 준다
		if (isActionTaken) //행동을 취한적이 있는 캐릭터라면
		{
			charStatus = CharStatus::IsActed;
		} //행동한 상태로 유지
		else if(charStatus == CharStatus::IsMoving)
		{
			charStatus = CharStatus::IsMoving;
		}
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
	else if (cursor->GetCursorTurn() == IngameStatus::PlayerTurn && whosChar == OwnedBy::Enemy)
	{
		charStatus = CharStatus::IsCheckingOut;
	}
	else if (cursor->GetCursorTurn() == IngameStatus::EnemyTurn)
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

			DATACENTRE.ClearObjects(ObjType::BlueTiles);
			DATACENTRE.ClearObjects(ObjType::FoeTiles);
			DATACENTRE.ClearObjects(ObjType::AllyTiles);

			cursor->SetCursorOccupied("");
		}
		draggingIndex = cursor->GetIndex();
		if (!PointCompare(index, cursor->GetIndex()))
		{
			charStatus = CharStatus::IsDragging;

			draggingIndex = cursor->GetIndex();
			draggingIndexPrev = draggingIndex;
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

			for (auto& toThickenRed : redTiles)
			{
				toThickenRed->SetRedAlpha(0.8f);
			}
			for (auto& toThickenGreen : greenTiles)
			{
				toThickenGreen->SetGreenAlpha(0.8f);
			}
			for (auto& toThickenBlue : blueTiles)
			{
				toThickenBlue->SetBlueAlpha(0.8f);
				DATACENTRE.AddObj(ObjType::BlueTiles, TwoDimentionArrayToOneString(toThickenBlue->GetIndex(), TILECOLX), toThickenBlue);
			}
			for (auto& Ally : allyTiles)
			{
				DATACENTRE.AddObj(ObjType::AllyTiles, TwoDimentionArrayToOneString(Ally->GetIndex(), TILECOLX), Ally);
			}
			for (auto& Enemy : foeTiles)
			{
				DATACENTRE.AddObj(ObjType::FoeTiles, TwoDimentionArrayToOneString(Enemy->GetIndex(), TILECOLX), Enemy);
			}
		}
		break;

	//▼ 아이들 상태일때
	case CharStatus::Idle:
		DisableMoveRange();
		break;

	//▼ 드래깅 상태일때
	case CharStatus::IsDragging:
	{
		//▼우클릭이나 S를 누르면 픽상태 해지
		if (KEYMANAGER->IsOnceKeyDown('S'))
		{
			charStatus = CharStatus::IsCursorOn;
			DisableMoveRange();
			cursor->SetCursorOccupied("");
			cursor->SetIndex({ index.x,index.y });
			cursor->SetPositionViaIndex();

			DATACENTRE.ClearObjects(ObjType::BlueTiles);
			DATACENTRE.ClearObjects(ObjType::FoeTiles);
			DATACENTRE.ClearObjects(ObjType::AllyTiles);
			for (auto& arrowTiles : toMove)
			{
				arrowTiles->SetArrowT("");
			}
			toMove.clear();
			break;
		}

		//▼시작 세팅
		draggingIndex = cursor->GetIndex(); //플레이어 위치
		draggingStarted = nullptr;			//시작지점 비워줌
		if (DATACENTRE.CheckObjectExistance(ObjType::BlueTiles, TwoDimentionArrayToOneString(draggingIndex, TILECOLX)))
		{
			draggingStarted = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::BlueTiles, TwoDimentionArrayToOneString(draggingIndex, TILECOLX)));
			draggingIndexPrev = draggingIndex;
		}
		if (draggingStarted == nullptr)
			{draggingStarted = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::FoeTiles, TwoDimentionArrayToOneString(draggingIndex, TILECOLX)));}
		assert(draggingStarted!=nullptr);

		auto MoveContainer = DATACENTRE.RefObjects(ObjType::BlueTiles);
		for (auto& arrowTiles : toMove)
		{
			arrowTiles->SetArrowT("");
		}
		toMove.clear();
		
		//▼구현 
		while (!PointCompare(draggingStarted->GetIndex(), this->index))
		{
			toMove.push_back(draggingStarted);
			int toFind = draggingStarted->GetRouteNum();
			for (auto& availableTile : MoveContainer)
			{
				assert((dynamic_cast<Tiles*>(availableTile.second)) != nullptr);
				if ((dynamic_cast<Tiles*>(availableTile.second)->GetRouteNum() == toFind + 1)
					&& (((availableTile.second->GetIndex().x == draggingStarted->GetIndex().x+1) && (availableTile.second->GetIndex().y == draggingStarted->GetIndex().y))
					|| ((availableTile.second->GetIndex().x == draggingStarted->GetIndex().x-1) && (availableTile.second->GetIndex().y == draggingStarted->GetIndex().y))
					|| ((availableTile.second->GetIndex().x == draggingStarted->GetIndex().x) && (availableTile.second->GetIndex().y-1 == draggingStarted->GetIndex().y))
					|| ((availableTile.second->GetIndex().x == draggingStarted->GetIndex().x) && (availableTile.second->GetIndex().y+1 == draggingStarted->GetIndex().y))))
				{
					draggingStarted = dynamic_cast<Tiles*>(availableTile.second);
					break;
				}
			}
		}
		toMove.push_back(dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::BlueTiles, TwoDimentionArrayToOneString(index, TILECOLX))));

		if (toMove.size() > 1)
		{
			for (UINT i = 0; i < toMove.size(); i++)
			{
				toMove[i]->SetArrowT("MoveArrow");
				if (i == 0 && (i + 1 < toMove.size()))
				{
					if (toMove[i + 1]->GetIndex().x > toMove[i]->GetIndex().x)
					{
						toMove[i]->SetArrowtFrame({ 0,2 });
						draggindDirection = DraggingDirection::LEFT;
					}
					else if (toMove[i + 1]->GetIndex().x < toMove[i]->GetIndex().x)
					{
						toMove[i]->SetArrowtFrame({ 1,2 });
						draggindDirection = DraggingDirection::RIGHT;
					}
					else if (toMove[i + 1]->GetIndex().y > toMove[i]->GetIndex().y)
					{
						toMove[i]->SetArrowtFrame({ 3,2 });
						draggindDirection = DraggingDirection::UP;
					}
					else
					{
						toMove[i]->SetArrowtFrame({ 2,2 });
						draggindDirection = DraggingDirection::DOWN;
					}
				}
				else if ((i - 1 >= 0) && i == toMove.size() - 1)
				{
					if (toMove[i - 1]->GetIndex().x > toMove[i]->GetIndex().x)
						toMove[i]->SetArrowtFrame({ 0,3 });
					else if (toMove[i - 1]->GetIndex().x < toMove[i]->GetIndex().x)
						toMove[i]->SetArrowtFrame({ 1,3 });
					else if (toMove[i - 1]->GetIndex().y > toMove[i]->GetIndex().y)
						toMove[i]->SetArrowtFrame({ 3,3 });
					else
						toMove[i]->SetArrowtFrame({ 2,3 });
				}
				else
				{
					if (toMove[i - 1]->GetIndex().x == toMove[i]->GetIndex().x && toMove[i + 1]->GetIndex().x== toMove[i]->GetIndex().x)
						toMove[i]->SetArrowtFrame({ 3,1 });
					else if (toMove[i - 1]->GetIndex().y == toMove[i]->GetIndex().y && toMove[i + 1]->GetIndex().y == toMove[i]->GetIndex().y)
						toMove[i]->SetArrowtFrame({ 0,1 });

					if (index.y > cursor->GetIndex().y)
					{
						if (toMove[i - 1]->GetIndex().x > toMove[i]->GetIndex().x
						&& toMove[i - 1]->GetIndex().y == toMove[i]->GetIndex().y
						&& toMove[i + 1]->GetIndex().x == toMove[i]->GetIndex().x
						&& toMove[i + 1]->GetIndex().y > toMove[i]->GetIndex().y)

						toMove[i]->SetArrowtFrame({ 0,0 });

						else if (toMove[i - 1]->GetIndex().x < toMove[i]->GetIndex().x
						&& toMove[i - 1]->GetIndex().y == toMove[i]->GetIndex().y
						&& toMove[i + 1]->GetIndex().x == toMove[i]->GetIndex().x
						&& toMove[i + 1]->GetIndex().y > toMove[i]->GetIndex().y)
					
						toMove[i]->SetArrowtFrame({ 1,0 });

						else if (toMove[i - 1]->GetIndex().x == toMove[i]->GetIndex().x
							&& toMove[i - 1]->GetIndex().y < toMove[i]->GetIndex().y
							&& toMove[i + 1]->GetIndex().x > toMove[i]->GetIndex().x
							&& toMove[i + 1]->GetIndex().y == toMove[i]->GetIndex().y)

							toMove[i]->SetArrowtFrame({ 3,0 });

						else if (toMove[i - 1]->GetIndex().x == toMove[i]->GetIndex().x
							&& toMove[i - 1]->GetIndex().y < toMove[i]->GetIndex().y
							&& toMove[i + 1]->GetIndex().x < toMove[i]->GetIndex().x
							&& toMove[i + 1]->GetIndex().y == toMove[i]->GetIndex().y)

							toMove[i]->SetArrowtFrame({ 2,0 });
						//TODO:추후 추가
					}
					else
					{
						if (toMove[i - 1]->GetIndex().x < toMove[i]->GetIndex().x
							&& toMove[i - 1]->GetIndex().y == toMove[i]->GetIndex().y
							&& toMove[i + 1]->GetIndex().x == toMove[i]->GetIndex().x
							&& toMove[i + 1]->GetIndex().y < toMove[i]->GetIndex().y)

							toMove[i]->SetArrowtFrame({ 2,0 });

						else if (toMove[i - 1]->GetIndex().x > toMove[i]->GetIndex().x
							&& toMove[i - 1]->GetIndex().y == toMove[i]->GetIndex().y
							&& toMove[i + 1]->GetIndex().x == toMove[i]->GetIndex().x
							&& toMove[i + 1]->GetIndex().y < toMove[i]->GetIndex().y)

							toMove[i]->SetArrowtFrame({ 3,0 });

						else if (toMove[i - 1]->GetIndex().x == toMove[i]->GetIndex().x
							&& toMove[i - 1]->GetIndex().y > toMove[i]->GetIndex().y
							&& toMove[i + 1]->GetIndex().x > toMove[i]->GetIndex().x
							&& toMove[i + 1]->GetIndex().y == toMove[i]->GetIndex().y)

							toMove[i]->SetArrowtFrame({ 0,0 });

						else if (toMove[i - 1]->GetIndex().x == toMove[i]->GetIndex().x
							&& toMove[i - 1]->GetIndex().y > toMove[i]->GetIndex().y
							&& toMove[i + 1]->GetIndex().x < toMove[i]->GetIndex().x
							&& toMove[i + 1]->GetIndex().y == toMove[i]->GetIndex().y)

							toMove[i]->SetArrowtFrame({ 1,0 });
						else if (toMove[i - 1]->GetIndex().x == toMove[i]->GetIndex().x
							&& toMove[i - 1]->GetIndex().y < toMove[i]->GetIndex().y
							&& toMove[i + 1]->GetIndex().x > toMove[i]->GetIndex().x
							&& toMove[i + 1]->GetIndex().y == toMove[i]->GetIndex().y)

							toMove[i]->SetArrowtFrame({ 3,0 });
						//TODO:추후 추가
					}
				}
			}
		}

		//▼드래깅 도중 A키를 누르면 이동지역 확정지음
		if (KEYMANAGER->IsOnceKeyDown('A'))
		{
			this->charStatus = CharStatus::IsMoving;
		}

		break;
	}
	case CharStatus::Disable:

		break;
	case CharStatus::IsMoving:
		for (auto& arrowTiles : toMove)
		{
			arrowTiles->SetArrowT("");
		}
		if (toMove.size() > 0)
		{
			if (DATACENTRE.CheckObjectExistance(ObjType::FoeTiles, TwoDimentionArrayToOneString(toMove.back()->GetIndex(), TILECOLX)))
			{
				float directionAngle = floor(GetAngleDegree(RectLeftTopOnly(position), RectLeftTopOnly(toMove.back()->GetPosition())) + 0.5f);
				if (directionAngle == 0)
				{movingDirection = MovingDirection::RIGHT;}
				else if (directionAngle == 90)
				{movingDirection = MovingDirection::LEFT;}
				else if (directionAngle == 180)
				{movingDirection = MovingDirection::UP;}
				else if (directionAngle == 270)
				{movingDirection = MovingDirection::DOWN;}
				this->toMove.pop_back();
			}
			else if (PointCompare(RectLeftTopOnly(position), RectLeftTopOnly(toMove.back()->GetPosition())))
			{
				index = toMove.back()->GetIndex();
				this->toMove.pop_back();
			}
			else
			{
				float directionAngle = floor(GetAngleDegree(RectLeftTopOnly(position), RectLeftTopOnly(toMove.back()->GetPosition())) + 0.5f);
				if(directionAngle==0)
				{
					position.left += 4;
					position.right += 4;
					movingDirection = MovingDirection::RIGHT;
				}
				else if(directionAngle ==90)
				{ 
					position.top -= 4;
					position.bottom -= 4;
					movingDirection = MovingDirection::LEFT;
				}
				else if (directionAngle == 180)
				{
					position.left -= 4;
					position.right -= 4;
					movingDirection = MovingDirection::UP;
				}
				else if(directionAngle==270)
				{
					position.top += 4;
					position.bottom += 4;
					movingDirection = MovingDirection::DOWN;
				}
				else
				{
					assert(false && "wrong angle");
				}

			}
		}
		else
		{
			cursor->SetCursorTurn(IngameStatus::SelectionUI);
			cursor->SetCursorTurnPrev(IngameStatus::PlayerTurn);
		}

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

	//TODO
	//GameObject::SetPositionViaIndex();
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
	initTile->SetRouteNum(RangeCalculator);//검사대상에 아까 계산해둔 최초 이동값 넣어둠
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
		//▼첫째 조건이 참이라면
		if (UpValidity)
		{
			Tiles* checkTarget = dynamic_cast<Tiles*>(DATACENTRE.GetCertainObject(ObjType::Tile, std::to_string((toExamen->GetIndex().y - 1) * TILECOLX + toExamen->GetIndex().x)));
			UpValidity &= !checkTarget->GetCheckedNum();	//체크넘버 가져옴
			UpValidity &= checkTarget->GetObjT() == "";		//장애물 있는지 가져옴
			//▼이번에도 참이라면
			if (UpValidity)
			{
				//▼아군의 캐릭터였다면
				if (whosChar == OwnedBy::Player)
				{
					//▼상대방을 마주쳤을시, 상대방 리스트에 담음
					for (auto& isEnemy : DATACENTRE.RefObjects(ObjType::EnemyArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isEnemy.second->GetIndex()))
						{
							redTiles.insert(checkTarget);
							foeTiles.insert(checkTarget);
							checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
							UpValidity &= false;
						}
					}
					for (auto& isAlly : DATACENTRE.RefObjects(ObjType::PlayerArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isAlly.second->GetIndex()))
						{
							allyTiles.insert(checkTarget);
						}
					}

				}
				else if (whosChar == OwnedBy::Enemy)
				{
					for (auto& isEnemy : DATACENTRE.RefObjects(ObjType::PlayerArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isEnemy.second->GetIndex()))
						{
							redTiles.insert(checkTarget);
							foeTiles.insert(checkTarget);
							checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
							UpValidity &= false;
						}
					}
					for (auto& isAlly : DATACENTRE.RefObjects(ObjType::EnemyArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isAlly.second->GetIndex()))
						{
							allyTiles.insert(checkTarget);
						}
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
				if (whosChar == OwnedBy::Player)
				{
					for (auto& isEnemy : DATACENTRE.RefObjects(ObjType::EnemyArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isEnemy.second->GetIndex()))
						{
							redTiles.insert(checkTarget);
							foeTiles.insert(checkTarget);
							checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
							LeftValidity &= false;
						}
					}
					for (auto& isAlly : DATACENTRE.RefObjects(ObjType::PlayerArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isAlly.second->GetIndex()))
						{
							allyTiles.insert(checkTarget);
						}
					}

				}
				else if (whosChar == OwnedBy::Enemy)
				{
					for (auto& isEnemy : DATACENTRE.RefObjects(ObjType::PlayerArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isEnemy.second->GetIndex()))
						{
							redTiles.insert(checkTarget);
							foeTiles.insert(checkTarget);
							checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
							LeftValidity &= false;
						}
					}
					for (auto& isAlly : DATACENTRE.RefObjects(ObjType::EnemyArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isAlly.second->GetIndex()))
						{
							allyTiles.insert(checkTarget);
						}
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
				if (whosChar == OwnedBy::Player)
				{
					for (auto& isEnemy : DATACENTRE.RefObjects(ObjType::EnemyArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isEnemy.second->GetIndex()))
						{
							redTiles.insert(checkTarget);
							foeTiles.insert(checkTarget);
							checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
							DownValidity &= false;
						}
					}
					for (auto& isAlly : DATACENTRE.RefObjects(ObjType::PlayerArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isAlly.second->GetIndex()))
						{
							allyTiles.insert(checkTarget);
						}
					}

				}
				else if (whosChar == OwnedBy::Enemy)
				{
					for (auto& isEnemy : DATACENTRE.RefObjects(ObjType::PlayerArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isEnemy.second->GetIndex()))
						{
							redTiles.insert(checkTarget);
							foeTiles.insert(checkTarget);
							checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
							DownValidity &= false;
						}
					}
					for (auto& isAlly : DATACENTRE.RefObjects(ObjType::EnemyArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isAlly.second->GetIndex()))
						{
							allyTiles.insert(checkTarget);
						}
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
				if (whosChar == OwnedBy::Player)
				{
					for (auto& isEnemy : DATACENTRE.RefObjects(ObjType::EnemyArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isEnemy.second->GetIndex()))
						{
							redTiles.insert(checkTarget);
							foeTiles.insert(checkTarget);
							checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
							RightValidity &= false;
						}
					}
					for (auto& isAlly : DATACENTRE.RefObjects(ObjType::PlayerArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isAlly.second->GetIndex()))
						{
							allyTiles.insert(checkTarget);
						}
					}

				}
				else if (whosChar == OwnedBy::Enemy)
				{
					for (auto& isEnemy : DATACENTRE.RefObjects(ObjType::PlayerArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isEnemy.second->GetIndex()))
						{
							redTiles.insert(checkTarget);
							foeTiles.insert(checkTarget);
							checkTarget->SetCheckedNum(toExamen->GetCheckedNum() - 1);
							RightValidity &= false;
						}
					}
					for (auto& isAlly : DATACENTRE.RefObjects(ObjType::EnemyArmy))
					{
						if (PointCompare(checkTarget->GetIndex(), isAlly.second->GetIndex()))
						{
							allyTiles.insert(checkTarget);
						}
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
//▼이동 및 공격범위 파란색 안보이게 처리
void Character::DisableMoveRange()
{
	//▼해당 기능은 한번이라도 범위표시 연산이 있었을때만 작동
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
		DATACENTRE.ClearObjects(ObjType::BlueTiles);
		DATACENTRE.ClearObjects(ObjType::FoeTiles);
		DATACENTRE.ClearObjects(ObjType::AllyTiles);
		redTiles.clear();
		greenTiles.clear();

		AvailableTiles.clear();
		isMoveCalculated = false;
	}
}

//▼(공+이동) 보라색 행동범위 표시한거 끌 때 사용
void Character::DisableActionRange()
{
	//▼해당 기능은 한번이라도 범위표시 연산이 있었을때만 작동
	if (isRangeCalculated)
	{
		for (auto& purpleTile : purpleTiles)
		{
			purpleTile->DecreasePurpleNum();
		}

		isRangeCalculated = false;
	}
	purpleTiles.clear();
	blueTiles.clear();
	redTiles.clear();
	greenTiles.clear();

	AvailableTiles.clear();
}

//▼공+이동 범위 통합표시
void Character::ShowActionRange()
{
	//▼함수포인터를 넘겨서 각기 다른 기능 수행
	ShowMoveRange(&Character::PurpleShow);
	isRangeCalculated = true; //계산했다고 알림
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

//▼ 공격범위인 빨강과 이동범위 파란색을 둘 다 보여주는 기능
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
				toCal->SetRouteNum(toCal->GetCheckedNum());
				toCal->SetCheckedNum(0);
			}
			else
			{
				redTiles.insert(toCal);
				toCal->SetRouteNum(toCal->GetCheckedNum());
				toCal->SetCheckedNum(0);
			}
		}
		else
		{
			if (toCal->GetBlueAlpha() < 0.5f) toCal->SetBlueAlpha(0.4f);
			toCal->IncreaseBlueNum();	//블루 참조 갯수 증가
			toCal->SetRouteNum(toCal->GetCheckedNum());
			toCal->SetCheckedNum(0);
			blueTiles.insert(toCal);
		}
	}
	for (auto& redTile : redTiles)
	{
		if (redTile->GetRedAlpha() < 0.5f) redTile->SetRedAlpha(0.4f);
		redTile->IncreaseRedNum();	//레드 참조 갯수 증가
		redTile->SetRouteNum(redTile->GetCheckedNum());
		redTile->SetCheckedNum(0);
	}
}

//▼공+이동범위 통합인 보라색을 보여주는 기능
void Character::PurpleShow(INT _actionRange)
{
	//▼연산 통해 담긴 행동범위에 따라 액션
	for (auto& purpleTile : AvailableTiles)
	{
		if (purpleTile->GetPurpleAlpha() < 0.5f) purpleTile->SetPurpleAlpha(0.4f);
		purpleTile->IncreasePurpleNum();	//레드 참조 갯수 증가
		purpleTile->SetRouteNum(purpleTile->GetCheckedNum());
		purpleTile->SetCheckedNum(0);
		purpleTiles.insert(purpleTile);
	}

	for (auto& purpleEnemyTile : foeTiles)
	{
		if (purpleEnemyTile->GetPurpleAlpha() < 0.5f) purpleEnemyTile->SetPurpleAlpha(0.4f);
		purpleEnemyTile->IncreasePurpleNum();	//레드 참조 갯수 증가
		purpleEnemyTile->SetRouteNum(purpleEnemyTile->GetCheckedNum());
		purpleEnemyTile->SetCheckedNum(0);
		purpleTiles.insert(purpleEnemyTile);
	}
}


//▼직업 세팅용. 들어올 시 해당하는 스텟을 넣어줌. 존재하지 않는 직업이 들어올 시 터뜨림
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
		occupationData.Move = 5;
		break;
	case Occupation::Knight:
		occupationData.Range = 1;
		occupationData.Health = 25;
		occupationData.Attack = 5;
		occupationData.Defence = 5;
		occupationData.Luck = 90;
		occupationData.Move = 7;
		break;
	case Occupation::Mage:
		occupationData.Range = 2;
		occupationData.Health = 20;
		occupationData.Attack = 8;
		occupationData.Defence = 2;
		occupationData.Luck = 80;
		occupationData.Move = 5;
		break;
	case Occupation::Assassin:
		occupationData.Range = 1;
		occupationData.Health = 20;
		occupationData.Attack = 8;
		occupationData.Defence = 2;
		occupationData.Luck = 95;
		occupationData.Move = 5;
		break;
	case Occupation::Archer:
		occupationData.Range = 2;
		occupationData.Health = 15;
		occupationData.Attack = 8;
		occupationData.Defence = 2;
		occupationData.Luck = 70;
		occupationData.Move = 5;
		break;
	case Occupation::Cleric:
		occupationData.Range = 1;
		occupationData.Health = 15;
		occupationData.Attack = 2;
		occupationData.Defence = 1;
		occupationData.Luck = 100;
		occupationData.Move = 5;
		break;
	case Occupation::Sniper:
		occupationData.Range = 3;
		occupationData.Health = 15;
		occupationData.Attack = 8;
		occupationData.Defence = 2;
		occupationData.Luck = 70;
		occupationData.Move = 5;
		break;
	default:
		assert(false && "Occupation does not exist"); //존재하지 않는 직업 들어오면 터뜨림
		break;
	}
}

//▼소유주와 직업에 따라서 알아서 넣어짐
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
	//▼카메라 속에 있을때만 랜더한다
	if (CheckInCamera())
	{
		AdjustFrame(); //프레임 돌리는 부분은 턴이 아닐떄도 돌아가야하니 랜더에 상주 

		frameImg->SetSize({ TILESIZE, TILESIZE }); //프레임랜더라 사이즈 세팅
		if (charStatus == CharStatus::IsMoving) 
			{ frameImg->RelativeFrameRender(position.left, position.top, frame.x + frameLoop, frame.y + (INT)movingDirection); } //카메라 상대 랜더
		else 
			{ frameImg->RelativeFrameRender(position.left, position.top, frame.x + frameLoop, frame.y); } //카메라 상대 랜더

		if (isActionTaken) //행동을 취했었다면
		{
			IMAGEMANAGER->FindImage("ActionTaken")->SetAlpha(0.5f);
			IMAGEMANAGER->FindImage("ActionTaken")->SetSize({ TILESIZE, TILESIZE });
			IMAGEMANAGER->FindImage("ActionTaken")->RelativeRender(position.left, position.top); //어두운 그림자로 덮어버림
		}

		//▼추 후 UI로 뺼 부분
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
	//▼캐릭터 범위를 표시 후 드래깅중일때 예상 이동경로에 캐릭터를 그림
	if (charStatus == CharStatus::IsDragging)
	{
		frameImg->SetSize({ TILESIZE, TILESIZE });
		frameImg->SetAlpha(0.5f);
		frameImg->RelativeFrameRender(draggingIndexPrev.x * TILESIZE, draggingIndexPrev.y * TILESIZE, frame.x + frameLoop, frame.y + (UINT)draggindDirection);
	}
	for (auto asd : toMove)
	{
		//D2DRENDERER->DrawRectangle(CAMERA.RelativeCameraRect(asd->GetPosition()),D2DRenderer::DefaultBrush::Red, 5);
		D2DRENDERER->RenderText(CAMERA.RelativeCameraRect(asd->GetPosition()).left, CAMERA.RelativeCameraRect(asd->GetPosition()).top, std::to_wstring(asd->GetIndex().x) + L", " + std::to_wstring(asd->GetIndex().y), 20,D2DRenderer::DefaultBrush::Red);
		D2DRENDERER->RenderText(CAMERA.RelativeCameraRect(asd->GetPosition()).right, CAMERA.RelativeCameraRect(asd->GetPosition()).bottom, std::to_wstring(asd->GetRouteNum()), 20, D2DRenderer::DefaultBrush::Black);
	}
}