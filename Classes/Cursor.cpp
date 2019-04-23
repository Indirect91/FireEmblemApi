#include "../stdafx.h"
#include "Cursor.h"
#include "Tiles.h"



Cursor::Cursor()
{
	cursorFrame = 0;				//커서가
	cursorCounter = 0;				//커서 프레임올릴 카운터
	isCursorVisible = false;
}

void Cursor::Init()
{
	for (auto& getFirst : DATACENTRE.RefObjects(ObjType::CurrentPlayerArmy))
	{
		this->index = getFirst.second->GetIndex();
		break;
	}
	this->index = {50,50};
	cursorFrame = 0;				//커서가 너무 커서
	cursorCounter = 0;				//커서 프레임올릴 카운터
	isCursorVisible = false;
}

void Cursor::Release()
{

}

void Cursor::Update()
{
	CursorFrameManage();

	for (auto& clipped : clippedTiles)
	{
		RECT RelativeRc = CAMERA.RelativeCameraRect(clipped.second->GetPosition());
		if (PtInRect(&RelativeRc, _ptMouse))
		{
			isCursorVisible = true;
			SetIndex(clipped.second->GetIndex());
			SetPositionViaIndex();
			break;
		}
		else
		{
			isCursorVisible = false;
		}
	}
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
		IMAGEMANAGER->FindImage("타일커서")->SetSize({ TILESIZE, TILESIZE });
		IMAGEMANAGER->FindImage("타일커서")->FrameRender(index.x * TILESIZE - CAMERA.GetCameraRc().left, index.y * TILESIZE - CAMERA.GetCameraRc().top, cursorFrame, 0);
		IMAGEMANAGER->FindImage("Cursor")->Render(_ptMouse.x, _ptMouse.y);
	}
}