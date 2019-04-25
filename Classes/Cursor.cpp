#include "../stdafx.h"
#include "Cursor.h"
#include "Tiles.h"


//▼커서 생성자
Cursor::Cursor()
{
	cursorFrame = 0;								//커서 프레임
	cursorCounter = 0;								//커서 프레임올릴 카운터
	cursorState = CursorState::cursorDeactivated;	//커서 비활성화 출고
	cursorColour = CursorColour::PlayerTurn;		//일단은 플레이어 턴 색으로 출고
	cursorOccupied = "";							//커서 점유한 캐릭터명
	prevLocation = index;							//커서 드래그시 시작지점
}

//▼커서 이닛
void Cursor::Init()
{
	this->index = {5,5};//일단 {5,5} 로 출고

	//▼비어있으면 어짜피 실행안됨. 현 플레이어의 부대가 따로 존재한다면 첫째 아군에 포인터를 맞춰줌
	for (auto& getFirst : DATACENTRE.RefObjects(ObjType::CurrentPlayerArmy))
	{
		this->index = getFirst.second->GetIndex();
		break;
	}

	cursorFrame = 0;				//커서 프레임
	cursorCounter = 0;				//커서가 너무 커서 프레임돌릴 카운터
	SetPositionViaIndex();			//커서 포지션도 인덱스에 맞게 설정
}

//▼커서 릴리즈
void Cursor::Release()
{
	//뉴할당 없음
}

//▼커서 업데이트
void Cursor::Update()
{
	//▼상태값에 따른 행동
	switch (cursorState)
	{
	case Cursor::CursorState::cursorActivated:
		for (auto& clipped : clippedTiles)
		{
			RECT RelativeRc = CAMERA.RelativeCameraRect(clipped.second->GetPosition());
			if (PtInRect(&RelativeRc, _ptMouse))
			{
				SetIndex(clipped.second->GetIndex());
				SetPositionViaIndex();
				break;
			}
		}
		break;
	case Cursor::CursorState::cursorDeactivated: 
		break;
	}
	CursorFrameManage();
}


//▼커서의 프레임 관리
void Cursor::CursorFrameManage()
{
	cursorCounter++;
	if (cursorCounter % 10 == 0)
	{
		cursorFrame++;
		if (cursorFrame > 3)cursorFrame = 0;
	}
}


//▼이동이 가능했다면 이동하고 성공시 True를 반환
BOOL Cursor::MoveUp()
{
	if (index.y == 0)
	{
		return false;
	}
	else
	{
		index.y -= 1;
		SetPositionViaIndex();
		return true;
	}
}

//▼이동이 가능했다면 이동하고 성공시 True를 반환
BOOL Cursor::MoveDown()
{
	if (index.y == TILEROWY)
	{
		return false;
	}
	else
	{
		index.y += 1;
		SetPositionViaIndex();
		return true;
	}
}

//▼이동이 가능했다면 이동하고 성공시 True를 반환
BOOL Cursor::MoveLeft()
{
	if (index.x == 0)
	{
		return false;
	}
	else
	{
		index.x -= 1;
		SetPositionViaIndex();
		return true;
	}
}

//▼이동이 가능했다면 이동하고 성공시 True를 반환
BOOL Cursor::MoveRight()
{
	if (index.x == TILECOLX)
	{
		return false;
	}
	else
	{
		index.x += 1;
		SetPositionViaIndex();
		return true;
	}
}

//▼랜더.
void Cursor::Render()
{	
	switch (cursorColour)
	{
	case Cursor::CursorColour::PlayerTurn:
		IMAGEMANAGER->FindImage("CursorBlue")->Render(_ptMouse.x, _ptMouse.y);
		break;
	case Cursor::CursorColour::EnemyTurn:
		IMAGEMANAGER->FindImage("CursorRed")->Render(_ptMouse.x, _ptMouse.y);
		break;
	case Cursor::CursorColour::UI:
		IMAGEMANAGER->FindImage("Cursor")->Render(_ptMouse.x, _ptMouse.y);
		break;
	}

	IMAGEMANAGER->FindImage("타일커서")->SetSize({ TILESIZE, TILESIZE });
	IMAGEMANAGER->FindImage("타일커서")->FrameRender(index.x * TILESIZE - CAMERA.GetCameraRc().left, index.y * TILESIZE - CAMERA.GetCameraRc().top, cursorFrame, 0);
		
	
}