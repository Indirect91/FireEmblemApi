#include "stdafx.h"
#include "MapToolScene.h"




void MapToolScene::Init()
{
	SetCoverStatue::FadeIn;
	moveWindowRateX = 989.5;	//������ �̵���ų �ʱⰪ
	moveWindowRateAccX = 5; //������ �̵���ų ��ġ ����
	isMovingDone = false;
	IMAGEMANAGER->AddImage("MapToolBg1", L"IMAGE/MapToolScene/MapToolBg1.png");
	IMAGEMANAGER->AddImage("MapToolBg2", L"IMAGE/MapToolScene/MapToolBg2.png");
	
}

void MapToolScene::Release()
{
	//���Ҵ��Ѱ� ������ ���
}

void MapToolScene::Update()
{
	if(!isMovingDone) moveWindowFrame();
}

void MapToolScene::moveWindowFrame()
{
	//�������� 1600���� ���ߴ� �ڵ�
	if (moveWindowRateX >= 1600)
	{
		moveWindowRateX = 1600;
		isMovingDone = true;
	}
	else moveWindowRateX += moveWindowRateAccX;

	if (moveWindowRateAccX < 300) moveWindowRateAccX += 3.f;
	else if (moveWindowRateAccX >= 300) moveWindowRateAccX = 300;


	SetWindowPos(_hWnd, 0, 150, 130, moveWindowRateX, 900, SWP_NOMOVE);
	D2DRENDERER->GetRenderTarget()->Resize({ static_cast<UINT32>(moveWindowRateX),900 });
}

void MapToolScene::Render()
{
	//IMAGEMANAGER->FindImage("MapToolBg1")->SetSize({ 500, 640 });
	IMAGEMANAGER->FindImage("MapToolBg1")->Render(0, 0);
	IMAGEMANAGER->FindImage("MapToolBg2")->Render(0, 0);
}

