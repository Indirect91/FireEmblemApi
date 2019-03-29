#include "stdafx.h"
#include "MapToolScene.h"




void MapToolScene::Init()
{
	SetCoverStatue::FadeIn;
	moveWindowRateX = 989.5;	//윈도우 이동시킬 초기값
	moveWindowRateAccX = 5; //윈도우 이동시킬 수치 가속
	isMovingDone = false;
}

void MapToolScene::Release()
{
	//뉴할당한거 없으니 비움
}

void MapToolScene::Update()
{
	if(!isMovingDone) moveWindowFrame();
}

void MapToolScene::Render()
{

}

void MapToolScene::moveWindowFrame()
{
	//▼윈도우 1600까지 맞추는 코드
	if (moveWindowRateX >= 1600)
	{
		moveWindowRateX = 1600;
		isMovingDone = true;
	}
	else moveWindowRateX += moveWindowRateAccX;

	if (moveWindowRateAccX <300) moveWindowRateAccX += 3.f;
	else if (moveWindowRateAccX >= 300) moveWindowRateAccX = 300;


	SetWindowPos(_hWnd, 0, 150, 130, moveWindowRateX, 900, SWP_NOMOVE);
}