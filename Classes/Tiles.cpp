#include "../stdafx.h"
#include "Tiles.h"

Tiles::Tiles() //�����ɶ��� �׳� ����·� ���
{
	status = TileStatus::none;
	position = { 0 };

	objectT = "";
	objTFrame = { 0 };

	terrain = "";
	terrainFrame = { 0 };

	movingT = "";
	movingtFrame = { 0 };

	checked = false;
	blueNum = 0;
	blueAlpha = 0;
	blueClicked = false;
}

void Tiles::IncreaseBlueNum()
{
	assert(blueNum < 2);
	blueNum++;
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
	if (blueNum == 0) blueAlpha = 0;
}



void Tiles::Init()
{
	status = TileStatus::none;
	position = { 0 };

	objectT = "";
	objTFrame = { 0 };

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
}

