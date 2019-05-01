#include "../stdafx.h"
#include "SelectionUI.h"
#include "Cursor.h"
#include "Character.h"

SelectionUI::SelectionUI()
{
	
	selectionBox.selectionBoxList = { 500,200,600,300 };
	selectionBox.selectionBoxTomove = {};
	battlePredict.allyRenderPosition = {100,100};
	battlePredict.enemyRenderPosition = { 500,100 };
	cursor = nullptr;
	photoFrameAlpha = 0;
	toShow = ToShow::Idle;
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


	switch (toShow)
	{
	case SelectionUI::ToShow::SelectionBox:

		if (KEYMANAGER->IsOnceKeyDown(VK_UP))
		{
			selectionBox.selectionBoxList.top -= 150;
			selectionBox.selectionBoxList.bottom -= 150;
		}
		else if (KEYMANAGER->IsOnceKeyDown(VK_DOWN))
		{
			selectionBox.selectionBoxList.top += 150;
			selectionBox.selectionBoxList.bottom += 150;
		}
		else if (KEYMANAGER->IsOnceKeyDown('A'))
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


		break;
	case SelectionUI::ToShow::BattlePredict:
	{
		if (KEYMANAGER->IsOnceKeyDown('S'))
		{
			cursor->SetCursorTurn(cursor->GetCursorTurnPrev());
			cursor->SetCursorTurnPrev(IngameStatus::SelectionUI);
		}




		break;
	}
	default:
		assert(false && "SelectionUI not made yet");
		break;
	}



}

void SelectionUI::Render()
{

	switch (toShow)
	{
	case SelectionUI::ToShow::SelectionBox:
		IMAGEMANAGER->FindImage("Blue")->SetSize({ 300, 100 });
		IMAGEMANAGER->FindImage("Blue")->Render(selectionBox.selectionBoxList.left, selectionBox.selectionBoxList.top);

		break;
	case SelectionUI::ToShow::BattlePredict:
		IMAGEMANAGER->FindImage("초상화" + battlePredict.allyPtr->GetName())->Render(battlePredict.allyRenderPosition.x, battlePredict.allyRenderPosition.y);
		IMAGEMANAGER->FindImage("초상화" + battlePredict.enemyPtr->GetName())->Render(battlePredict.enemyRenderPosition.x, battlePredict.enemyRenderPosition.y);
		break;
	default:
		assert(false && "보여주는 UI에 에러있음");
		break;
	}


	IMAGEMANAGER->FindImage("PhotoFrame")->SetSize({ 800,640 });
	IMAGEMANAGER->FindImage("PhotoFrame")->SetAlpha(photoFrameAlpha);
	IMAGEMANAGER->FindImage("PhotoFrame")->Render(0, 0);
}

