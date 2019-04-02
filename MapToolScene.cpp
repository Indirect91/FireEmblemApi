#include "stdafx.h"
#include "MapToolScene.h"
#include "Tiles.h"



void MapToolScene::Init()
{
	SCENEMANAGER->RefCoverSpeed() = 0.035f;
	SetCoverStatue::FadeIn;
	moveWindowRateX = 789.5;	//윈도우 이동시킬 초기값
	moveWindowRateAccX = 3; //윈도우 이동시킬 수치 가속
	isMovingDone = false;
	IMAGEMANAGER->AddImage("MapToolBg1", L"IMAGE/MapToolScene/MapToolBg1.png");
	IMAGEMANAGER->AddImage("MapToolBg2", L"IMAGE/MapToolScene/MapToolBg2.png");
	IMAGEMANAGER->AddImage("MapToolTitle", L"IMAGE/MapToolScene/MaptoolTitle.png"); 
	IMAGEMANAGER->AddImage("MapToolMapbox", L"IMAGE/MapToolScene/MaptoolMapbox.png");
	IMAGEMANAGER->AddImage("MapToolMat2", L"IMAGE/MapToolScene/MaptoolMat2.png");
	IMAGEMANAGER->AddImage("oldpaper", L"IMAGE/MapToolScene/oldpaper.png");
	SOUNDMANAGER->addSound("맵툴BGM", "SOUND/FE/MapTool_Road Taken.mp3", true, true);
	paperScrollX = moveWindowRateX - IMAGEMANAGER->FindImage("MapToolBg2")->GetWidth();
	SOUNDMANAGER->pause("타이틀BGM");
	SOUNDMANAGER->play("맵툴BGM");
	SOUNDMANAGER->setFadeIn();
	
	////▼고를 이미지 4장
	//for (UINT i = 1; i < 5; i++)
	//{
	//	std::string tempIndex;
	//	std::string tempIndex2;
	//	tempIndex = std::to_string(i);
	//	tempIndex2 = (tempIndex += ".bmp").c_str();
	//	IMAGEMANAGER->AddImage(tempIndex, StringHelper::StringToWString(tempIndex2));
	//	
	//	tempImg->setX(i * 50 - 50);
	//	colours.push_back(tempIndex);
	//}


	arrField = new Tiles[fieldX_Column * fieldY_Row];
	
	for (int i = 0; i < fieldY_Row; i++)
	{
		for (int j = 0; j < fieldX_Column; j++)
		{
			arrField[i * fieldY_Row + j].Init();
			arrField[i * fieldY_Row + j].SetRc(RectMake(150+j * tileSize, 100+i * tileSize, tileSize, tileSize));
		}
	}
	tempC = { 0 };
	showArea = CAMERA.GetCameraRc();
}

void MapToolScene::Release()
{
	SAFE_DELETE_ARRAY(arrField);
}

void MapToolScene::Update()
{
	if(!isMovingDone) moveWindowFrame();
	moveCamera();

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

	if (moveWindowRateAccX < 300) moveWindowRateAccX += 1.5f;
	else if (moveWindowRateAccX >= 300) moveWindowRateAccX = 300;


	SetWindowPos(_hWnd, 0, 150, 130, moveWindowRateX, 900, SWP_NOMOVE);
	D2DRENDERER->GetRenderTarget()->Resize({ static_cast<UINT32>(moveWindowRateX),900 });
	paperScrollX = moveWindowRateX - IMAGEMANAGER->FindImage("MapToolBg2")->GetWidth();
}

void MapToolScene::moveCamera()
{
	if (KEYMANAGER->IsStayKeyDown(VK_RIGHT) || KEYMANAGER->IsStayKeyDown('D'))
	{
		if (CAMERA.GetCameraRc().right < 975)
		{
			CAMERA.RefCameraRc().left += 5;
			CAMERA.RefCameraRc().right += 5;
		}
	}

	if (KEYMANAGER->IsStayKeyDown(VK_LEFT) || KEYMANAGER->IsStayKeyDown('A'))
	{
		if (CAMERA.GetCameraRc().left > -20)
		{
			CAMERA.RefCameraRc().left -= 5;
			CAMERA.RefCameraRc().right -= 5;
		}
	}

	if (KEYMANAGER->IsStayKeyDown(VK_DOWN) || KEYMANAGER->IsStayKeyDown('S'))
	{
		if (CAMERA.GetCameraRc().top < 490)
		{
			CAMERA.RefCameraRc().top += 5;
			CAMERA.RefCameraRc().bottom += 5;
		}
	}

	if (KEYMANAGER->IsStayKeyDown(VK_UP) || KEYMANAGER->IsStayKeyDown('W'))
	{
		if (CAMERA.GetCameraRc().top > -45)
		{
			CAMERA.RefCameraRc().top -= 5;
			CAMERA.RefCameraRc().bottom -= 5;
		}
	}
	showArea = CAMERA.GetCameraRc();
	showArea.left += 150;
	showArea.right += 150;
	showArea.top += 100;
	showArea.bottom += 100;
}

void MapToolScene::Render()
{
	IMAGEMANAGER->FindImage("MapToolBg1")->Render(0, 0);

	for (int i = 0; i < fieldX_Column * fieldY_Row; i++)
	{
		if (IntersectRect(&tempC, &arrField[i].GetRc(), &showArea))
		{
			arrField[i].Render();
		}
	}

	IMAGEMANAGER->FindImage("MapToolMat2")->Render(1200, 140);
	IMAGEMANAGER->FindImage("MapToolMapbox")->Render(108, 60);
	IMAGEMANAGER->FindImage("MapToolTitle")->Render(800, 70, Pivot::Center);
	IMAGEMANAGER->FindImage("oldpaper")->SkewRender(WINSIZEX / 2, WINSIZEY / 2,-00, 0);
	IMAGEMANAGER->FindImage("oldpaper")->SkewRender(WINSIZEX / 2, WINSIZEY / 2,-2, 0);
	IMAGEMANAGER->FindImage("oldpaper")->SkewRender(WINSIZEX / 2, WINSIZEY / 2,-4, 0);
	IMAGEMANAGER->FindImage("oldpaper")->SkewRender(WINSIZEX / 2, WINSIZEY / 2,-6, 0);
	IMAGEMANAGER->FindImage("oldpaper")->SkewRender(WINSIZEX / 2, WINSIZEY / 2,-8, 0);
	IMAGEMANAGER->FindImage("oldpaper")->SkewRender(WINSIZEX / 2, WINSIZEY / 2,-10, 0);
	IMAGEMANAGER->FindImage("oldpaper")->SkewRender(WINSIZEX / 2, WINSIZEY / 2,-12, 0);
	IMAGEMANAGER->FindImage("oldpaper")->SkewRender(WINSIZEX / 2, WINSIZEY / 2,-14, 0);
	IMAGEMANAGER->FindImage("oldpaper")->SkewRender(WINSIZEX / 2, WINSIZEY / 2,-16, 0);
	IMAGEMANAGER->FindImage("oldpaper")->SkewRender(WINSIZEX / 2, WINSIZEY / 2,-18, 0);
	IMAGEMANAGER->FindImage("MapToolBg2")->Render(paperScrollX, 0);
}

