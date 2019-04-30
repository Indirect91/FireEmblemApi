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
	assert(cursor != nullptr); //Ŀ�� ����� �ȵ������� �Ͷ߸�
}

void SelectionUI::Release()
{
	//���⼭ ���Ҵ������ ����. ������ Ȥ�� ������ü��
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

	//����� �̵� ���
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

