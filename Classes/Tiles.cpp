#include "../stdafx.h"
#include "Tiles.h"

Tiles::Tiles() //생성될때는 그냥 빈상태로 출고
{
	status = TileStatus::none;
	position = { 0 };
	isOccupied = false;

	object = "";
	objFrame = {0};

	terrain = "";
	terrainFrame = { 0 };

	movingT = "";
	movingtFrame = { 0 };

}

void Tiles::Init()
{
	status = TileStatus::none;
	position = { 0 };
	isOccupied = false;

	object = "";
	objFrame = { 0 };

	terrain = "";
	terrainFrame = { 0 };

	movingT = "";
	movingtFrame = { 0 };
}

void Tiles::Release()
{
	//뉴할당한거 없음
}

void Tiles::Update()
{

}

void Tiles::Render()
{

	//▼타일에 지형이 깔려있을땐 타일을 보여줌
	if (terrain != "")
	{
		IMAGEMANAGER->FindImage(terrain)->Render(position.left, position.top);
		if (object != "")
		{
			IMAGEMANAGER->FindImage(object)->SetSize(IMAGEMANAGER->FindImage(object)->GetFrameSize());
			IMAGEMANAGER->FindImage(object)->FrameRender(position.left, position.top, objFrame.x, objFrame.y);
		}
	}
	D2DRENDERER->DrawRectangle(CAMERA.RelativeCameraRect(position));
}

