#include "../stdafx.h"
#include "Tiles.h"

Tiles::Tiles() //생성될때는 그냥 빈상태로 출고
{
	status = TileStatus::none;
	position = { 0 };

	object = "";
	objFrame = { 0 };

	terrain = "";
	terrainFrame = { 0 };

	movingT = "";
	movingtFrame = { 0 };

	checked = false;
	blueNum = 0;
	blueAlpha = 0;
	blueClicked = false;
}

void Tiles::SetBlueNum(INT _blue)
{
	assert(_blue >= 0);
	blueNum = _blue;
}

void Tiles::DecreaseBlueNum()
{

	blueNum--;
	assert(blueNum >= 0);
}



void Tiles::Init()
{
	status = TileStatus::none;
	position = { 0 };

	object = "";
	objFrame = { 0 };

	terrain = "";
	terrainFrame = { 0 };

	movingT = "";
	movingtFrame = { 0 };

	checked = false;
	blueNum = 0;
	blueAlpha = 0;
	blueClicked = false;
}

void Tiles::Release()
{
	//뉴할당한거 없음
}

void Tiles::Update()
{
	if (movingT != "")
	{
		//TODO: 프레임 돌리는 업데이트 진행
	}
}

void Tiles::Render()
{

	//▼타일에 지형이 깔려있을땐 타일을 보여줌
	if (terrain != "")
	{
		IMAGEMANAGER->FindImage(terrain)->Render(position.left, position.top);
		//TODO : 오브젝트 이름 맞추기
		//if (object != "")
		//{
		//	IMAGEMANAGER->FindImage(object)->SetSize(IMAGEMANAGER->FindImage(object)->GetFrameSize());
		//	IMAGEMANAGER->FindImage(object)->FrameRender(position.left, position.top, objFrame.x, objFrame.y);
		//}
	}
	D2DRENDERER->DrawRectangle(CAMERA.RelativeCameraRect(position));

	//std::wstring asd= std::to_wstring(index.x);
	//asd += L", ";
	//asd += std::to_wstring(index.y);
	//D2DRENDERER->RenderText(position.left, position.top, asd, 10);
}

