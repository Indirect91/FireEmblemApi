#include "../stdafx.h"
#include "Tiles.h"

Tiles::Tiles() //생성될때는 그냥 빈상태로 출고
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
	routeNum = 0;			//적 위치 추적에만 사용될 숫자

	ZeroMemory(&purpleTile, sizeof(purpleTile));
	ZeroMemory(&greenTile, sizeof(greenTile));
	ZeroMemory(&blueTile, sizeof(blueTile));
	ZeroMemory(&redTile, sizeof(redTile));
	
	//blueClicked = false;

	
}

//▼빨강 파랑 초록 갯수 증가
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

//▼빨강파랑초록 숫자 강제설정
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

//▼빨강 파랑 초록 참조 갯수 감소
void Tiles::DecreaseBlueNum()
{
	blueTile.Num--;
	assert(blueTile.Num >= 0 || "BlueTile negative"); //음수만 아니면 됨
	if (blueTile.Num == 0) blueTile.Alpha = 0;
}
void Tiles::DecreaseRedNum()
{
	redTile.Num--;
	assert(redTile.Num >= 0|| "RedTile negative"); //음수만 아니면 됨
	if (redTile.Num == 0) redTile.Alpha = 0;
}
void Tiles::DecreaseGreenNum()
{
	greenTile.Num--;
	assert(greenTile.Num >= 0 || "GreenTile negative"); //음수만 아니면 됨
	if (greenTile.Num == 0) greenTile.Alpha = 0;
}

void Tiles::DecreasePurpleNum()
{
	purpleTile.Num--;
	assert(purpleTile.Num >= 0 || "PurpleTile negative"); //음수만 아니면 됨
	if (purpleTile.Num == 0) purpleTile.Alpha = 0;
}

//▼타일 이닛될떄
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
	//뉴할당한거 없음
}

void Tiles::Update()
{
	if (movingT != "")
	{
		//TODO: 프레임 돌리는 업데이트 진행
	}
}
