#include "../stdafx.h"
#include "Tiles.h"

Tiles::Tiles() //�����ɶ��� �׳� ����·� ���
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
	//���Ҵ��Ѱ� ����
}

void Tiles::Update()
{

}

void Tiles::Render()
{

	//��Ÿ�Ͽ� ������ ��������� Ÿ���� ������
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

