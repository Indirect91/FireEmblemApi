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

	arrowT = "";
	arrowtFrame = { 0 };

	isStartLocation = false;
	checkedNum = 0;
	routeNum = 0;			//�� ��ġ �������� ���� ����

	ZeroMemory(&purpleTile, sizeof(purpleTile));
	ZeroMemory(&greenTile, sizeof(greenTile));
	ZeroMemory(&blueTile, sizeof(blueTile));
	ZeroMemory(&redTile, sizeof(redTile));
	
	//blueClicked = false;

	
}

//�廡�� �Ķ� �ʷ� ���� ����
void Tiles::IncreaseBlueNum()
{
	blueTile.Num++;
}
void Tiles::IncreaseRedNum()
{
	redTile.Num++;
}
void Tiles::IncreaseGreenNum()
{
	greenTile.Num++;
}

void Tiles::IncreasePurpleNum()
{
	purpleTile.Num++;
}

//�廡���Ķ��ʷ� ���� ��������
void Tiles::SetBlueNum(INT _blue)
{
	assert(_blue >= 0);
	blueTile.Num = _blue;
}
void Tiles::SetGreenNum(INT _green)
{
	assert(_green >= 0);
	greenTile.Num = _green;
}
void Tiles::SetRedNum(INT _red)
{
	assert(_red >= 0);
	redTile.Num = _red;
}

void Tiles::SetPurpleNum(INT _Purple)
{
	assert(_Purple >= 0);
	purpleTile.Num = _Purple;
}

//�廡�� �Ķ� �ʷ� ���� ���� ����
void Tiles::DecreaseBlueNum()
{
	blueTile.Num--;
	assert(blueTile.Num >= 0 || "BlueTile negative"); //������ �ƴϸ� ��
	if (blueTile.Num == 0) blueTile.Alpha = 0;
}
void Tiles::DecreaseRedNum()
{
	redTile.Num--;
	assert(redTile.Num >= 0|| "RedTile negative"); //������ �ƴϸ� ��
	if (redTile.Num == 0) redTile.Alpha = 0;
}
void Tiles::DecreaseGreenNum()
{
	greenTile.Num--;
	assert(greenTile.Num >= 0 || "GreenTile negative"); //������ �ƴϸ� ��
	if (greenTile.Num == 0) greenTile.Alpha = 0;
}

void Tiles::DecreasePurpleNum()
{
	purpleTile.Num--;
	assert(purpleTile.Num >= 0 || "PurpleTile negative"); //������ �ƴϸ� ��
	if (purpleTile.Num == 0) purpleTile.Alpha = 0;
}

//��Ÿ�� �̴ֵɋ�
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

	checkedNum = 0;

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
