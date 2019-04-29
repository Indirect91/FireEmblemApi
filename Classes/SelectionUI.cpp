#include "../stdafx.h"
#include "SelectionUI.h"
#include "Cursor.h"
#include "Character.h"

SelectionUI::SelectionUI()
{
	temp = { 500,200,600,300 };
}

void SelectionUI::Init()
{
}

void SelectionUI::Release()
{
}

void SelectionUI::Update()
{
	if (KEYMANAGER->IsOnceKeyDown(VK_UP))
	{
		temp.top -= 150;
		temp.bottom -= 150;
	}
	else if (KEYMANAGER->IsOnceKeyDown(VK_DOWN))
	{
		temp.top += 150;
		temp.bottom += 150;
	}
	else if (KEYMANAGER->IsOnceKeyDown(VK_RETURN))
	{
		auto ttmp = dynamic_cast<Cursor*>(DATACENTRE.GetCertainObject(ObjType::UI, "Cursor"));
		dynamic_cast<Character*>(DATACENTRE.GetCertainObject(ObjType::PlayerArmy, ttmp->GetCursorOccupied()))->DisableMoveRange();
		dynamic_cast<Character*>(DATACENTRE.GetCertainObject(ObjType::PlayerArmy, ttmp->GetCursorOccupied()))->SetActionTaken(true);

		//DATACENTRE.ClearObjects(ObjType::FoeTiles);
		//DATACENTRE.ClearObjects(ObjType::AllyTiles);
		ttmp->SetCursorOccupied("");
		ttmp->SetCursorTurn(ttmp->GetCursorTurnPrev());
		ttmp->SetCursorTurnPrev(IngameStatus::SelectionUI);
	}
}

void SelectionUI::Render()
{
	IMAGEMANAGER->FindImage("Blue")->SetSize({ 300, 100 });
	IMAGEMANAGER->FindImage("Blue")->Render(temp.left, temp.top);
	IMAGEMANAGER->FindImage("PhotoFrame")->SetSize({ WINSIZEX,WINSIZEY });
	IMAGEMANAGER->FindImage("PhotoFrame")->Render(0, 0);
}

