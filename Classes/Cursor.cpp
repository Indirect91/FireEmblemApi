#include "../stdafx.h"
#include "Cursor.h"
#include "Tiles.h"

Cursor::Cursor()
{
	cursorFrame = 0;				//커서가
	cursorCounter = 0;				//커서 프레임올릴 카운터
}

void Cursor::Init()
{
	for (auto& getFirst : DATACENTRE.GetObjects(ObjType::CurrentPlayerArmy))
	{
		this->index = getFirst.second->GetIndex();
		break;
	}
}

void Cursor::Release()
{

}

void Cursor::Update()
{
	CursorFrameManage();
}

void Cursor::Render()
{
	//IMAGEMANAGER->FindImage("타일커서")->SetSize({ TILESIZE, TILESIZE });
	//IMAGEMANAGER->FindImage("타일커서")->FrameRender(arrField[i].GetPosition().left, arrField[i].GetPosition().top, , 0);
}

void Cursor::CursorFrameManage()
{
	cursorCounter++;
	if (cursorCounter % 10 == 0)
	{
		cursorFrame++;
		if (cursorFrame > 3)cursorFrame = 0;
	}
}