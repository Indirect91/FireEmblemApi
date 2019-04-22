#include "../stdafx.h"
#include "Cursor.h"
#include "Tiles.h"



Cursor::Cursor()
{
	cursorFrame = 0;				//Ŀ����
	cursorCounter = 0;				//Ŀ�� �����ӿø� ī����
	isCursorVisible = false;
}

void Cursor::Init()
{
	for (auto& getFirst : DATACENTRE.GetObjects(ObjType::CurrentPlayerArmy))
	{
		this->index = getFirst.second->GetIndex();
		break;
	}
	cursorFrame = 0;				//Ŀ����
	cursorCounter = 0;				//Ŀ�� �����ӿø� ī����
	isCursorVisible = false;
}

void Cursor::Release()
{

}

void Cursor::Update()
{
	CursorFrameManage();
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

void Cursor::Render()
{
	if (isCursorVisible)
	{
		IMAGEMANAGER->FindImage("Ÿ��Ŀ��")->SetSize({ TILESIZE, TILESIZE });
		IMAGEMANAGER->FindImage("Ÿ��Ŀ��")->FrameRender(index.x * TILESIZE, index.y * TILESIZE, cursorFrame, 0);
	}
}