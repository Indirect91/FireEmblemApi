#include "stdafx.h"
#include "Tiles.h"

Tiles::Tiles() //생성될때는 그냥 빈상태로 출고
{
	rc = { 0 };
	object = "";
	terrain = "";
	objX = 0;
	objY = 0;
	counter = 0;
}

void Tiles::Init()
{
	rc = { 0 };
	object = "";
	terrain = "";
	objX = 0;
	objY = 0;
	counter = 0;
}

void Tiles::Release()
{
	//뉴할당한거 없음
}

void Tiles::Update()
{
	//▼오브젝트를 들고있을때만 작동
	if (object != "")
	{
		counter++;
		if (counter % 5 == 0)
		{
			objX++;
			if (objX > 4) objX = 0;
		}
	}
}

void Tiles::Render()
{
	//▼타일에 지형이 깔려있을땐 타일을 보여줌
	if (terrain != "")
	{
		IMAGEMANAGER->FindImage(terrain)->Render(rc.left, rc.top);
		if (object != "")
		{
			IMAGEMANAGER->FindImage(object)->SetSize(IMAGEMANAGER->FindImage(object)->GetFrameSize());
			IMAGEMANAGER->FindImage(object)->FrameRender(rc.left, rc.top, objX, objY);
		}
	}
	else
	{
		D2DRENDERER->DrawRectangle(RelativeCameraRect(rc));
	}
}

