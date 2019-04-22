#include "../stdafx.h"
#include "Tiles.h"

Tiles::Tiles() //�����ɶ��� �׳� ����·� ���
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
	//���Ҵ��Ѱ� ����
}

void Tiles::Update()
{
	if (movingT != "")
	{
		//TODO: ������ ������ ������Ʈ ����
	}
}

void Tiles::Render()
{

	//��Ÿ�Ͽ� ������ ��������� Ÿ���� ������
	if (terrain != "")
	{
		IMAGEMANAGER->FindImage(terrain)->Render(position.left, position.top);
		//TODO : ������Ʈ �̸� ���߱�
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

