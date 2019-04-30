#include "../stdafx.h"
#include "SelectionUI.h"
#include "Cursor.h"
#include "Character.h"

SelectionUI::SelectionUI()
{
	selectionBoxList = { 500,200,600,300 };
	cursor = nullptr;
	photoFrameAlpha = 0;
}

void SelectionUI::Init()
{
	cursor = dynamic_cast<Cursor*>(DATACENTRE.GetCertainObject(ObjType::UI, "Cursor"));
	assert(cursor != nullptr); //커서 제대로 안들어왔으면 터뜨림
}

void SelectionUI::Release()
{
	//여기서 뉴할당받은거 없음. 포인터 혹은 정적개체들
}

void SelectionUI::Update()
{
	if (photoFrameAlpha < 1.f) photoFrameAlpha += 0.1f;

	if (KEYMANAGER->IsOnceKeyDown(VK_UP))
	{
		selectionBoxList.top -= 150;
		selectionBoxList.bottom -= 150;
	}
	else if (KEYMANAGER->IsOnceKeyDown(VK_DOWN))
	{
		selectionBoxList.top += 150;
		selectionBoxList.bottom += 150;
	}
	else if (KEYMANAGER->IsOnceKeyDown(VK_RETURN))
	{
		
		dynamic_cast<Character*>(DATACENTRE.GetCertainObject(ObjType::PlayerArmy, cursor->GetCursorOccupied()))->DisableMoveRange();
		dynamic_cast<Character*>(DATACENTRE.GetCertainObject(ObjType::PlayerArmy, cursor->GetCursorOccupied()))->SetActionTaken(true);

		//DATACENTRE.ClearObjects(ObjType::FoeTiles);
		//DATACENTRE.ClearObjects(ObjType::AllyTiles);
		cursor->SetCursorOccupied("");
		cursor->SetCursorTurn(cursor->GetCursorTurnPrev());
		cursor->SetCursorTurnPrev(IngameStatus::SelectionUI);
	}

	//▼모의 이동 취소
	else if (KEYMANAGER->IsOnceKeyDown('S'))
	{
		Character* toExamine = dynamic_cast<Character*>(DATACENTRE.GetCertainObject(ObjType::PlayerArmy, cursor->GetCursorOccupied()));
		toExamine->SetStatus(Character::CharStatus::IsDragging);
		
		toExamine->SetIndex(toExamine->GetMoveStartLocation());
		toExamine->SetPositionViaIndex();

		cursor->SetCursorTurn(cursor->GetCursorTurnPrev());
		cursor->SetCursorTurnPrev(IngameStatus::SelectionUI);
	}
}

void SelectionUI::Render()
{
	IMAGEMANAGER->FindImage("Blue")->SetSize({ 300, 100 });
	IMAGEMANAGER->FindImage("Blue")->Render(selectionBoxList.left, selectionBoxList.top);


	IMAGEMANAGER->FindImage("PhotoFrame")->SetSize({ 800,640 });
	IMAGEMANAGER->FindImage("PhotoFrame")->SetAlpha(photoFrameAlpha);
	IMAGEMANAGER->FindImage("PhotoFrame")->Render(0, 0);
}

