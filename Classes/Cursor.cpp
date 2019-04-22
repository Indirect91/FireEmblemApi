#include "../stdafx.h"
#include "Cursor.h"
#include "Tiles.h"

Cursor::Cursor()
{
	cursorFrame = 0;				//Ŀ����
	cursorCounter = 0;				//Ŀ�� �����ӿø� ī����
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
	//IMAGEMANAGER->FindImage("Ÿ��Ŀ��")->SetSize({ TILESIZE, TILESIZE });
	//IMAGEMANAGER->FindImage("Ÿ��Ŀ��")->FrameRender(arrField[i].GetPosition().left, arrField[i].GetPosition().top, , 0);
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