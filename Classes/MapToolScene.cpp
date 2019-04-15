#include "../stdafx.h"
#include "MapToolScene.h"
#include "Tiles.h"



void MapToolScene::Init()
{
	//▼윈도우 및 커버 초기화
	SCENEMANAGER->RefCoverSpeed() = 0.035f;
	SetCoverStatue::FadeIn;
	moveWindowRateX = 789.5;	//윈도우 이동시킬 초기값
	moveWindowRateAccX = 3; //윈도우 이동시킬 수치 가속
	isMovingDone = false;
	paperScrollX = moveWindowRateX - IMAGEMANAGER->FindImage("MapToolBg2")->GetWidth();
	moveAngle = 0;

	//▼음악 초기화. 맵툴은 타이틀서부터밖에 못넘어온다
	SOUNDMANAGER->pause("타이틀BGM");
	SOUNDMANAGER->play("맵툴BGM");
	SOUNDMANAGER->setFadeIn();
	
	//▼눌린키 네모 초기화
	RightLeft.pos = { -50,-50 };
	RightLeft.pressed = false;
	UpDown.pos = { -50,-50 };
	UpDown.pressed = false;
	
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


	//▼카메라 및 타일들 초기화
	CAMERA.SetCamera({0,0,815,688});
	arrField = new Tiles[TILECOLX * TILEROWY];
	
	for (int i = 0; i < TILEROWY; i++)
	{
		for (int j = 0; j < TILECOLX; j++)
		{
			arrField[i * TILEROWY + j].Init(); //생성된 타일 초기화
			arrField[i * TILEROWY + j].SetPosition(RectMake(j * TILESIZE,i * TILESIZE, TILESIZE, TILESIZE));
		}
	}
	tempC = { 0 }; //충돌에 쓸 임시렉트 0
	
}

void MapToolScene::Release()
{
	//맵툴서 뉴할당받은건 타일들
	SAFE_DELETE_ARRAY(arrField);
}

void MapToolScene::Update()
{
	if (!isMovingDone) moveWindowFrame(); //윈도우 이동완료까지 이동
	moveCamera(); //방향키 입력에 따른 화면 이동
	RECT rc = { 158,138,158+815,138+688 };
	if (PtInRect(&rc, _ptMouse))
	{
		IMAGEMANAGER->FindImage("Cursor")->SetAngle(228);
	}
	else
		IMAGEMANAGER->FindImage("Cursor")->SetAngle(0);

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

	//▼양피지 스르르 이동하며 펼쳐지는곳
	SetWindowPos(_hWnd, 0, 150, 130, moveWindowRateX, 900, SWP_NOZORDER| SWP_NOMOVE);
	//▼랜더타겟변경으로 변경된 해상도에 맞게 세팅
	D2DRENDERER->GetRenderTarget()->Resize({ static_cast<UINT32>(moveWindowRateX),900 });
	paperScrollX = moveWindowRateX - IMAGEMANAGER->FindImage("MapToolBg2")->GetWidth();
}

void MapToolScene::moveCamera()
{
	//▼눌린키 표시미 네모 매 업뎃마다 일단은 안눌린상태로 초기화
	RightLeft.pos = {-50,-50};
	RightLeft.pressed = false;
	UpDown.pos = {-50,-50};
	UpDown.pressed = false;

	//▼키입력에 따른 카메라 이동
	if (KEYMANAGER->IsStayKeyDown(VK_RIGHT) || KEYMANAGER->IsStayKeyDown('D'))
	{
		if (CAMERA.GetCameraRc().right < TILESIZE * TILECOLX - 570)
		{
			CAMERA.RefCameraRc().left += 5;
			CAMERA.RefCameraRc().right += 5;
		}
		//▼좌우 네모박스 눌린여부 및 위치
		RightLeft.pos = { 592,851 };
		RightLeft.pressed = true;

		moveAngle++;
	}

	//▼키입력에 따른 카메라 이동
	else if (KEYMANAGER->IsStayKeyDown(VK_LEFT) || KEYMANAGER->IsStayKeyDown('A'))
	{
		if (CAMERA.GetCameraRc().left > 0)
		{
			CAMERA.RefCameraRc().left -= 5;
			CAMERA.RefCameraRc().right -= 5;
		}
		//▼좌우 네모박스 눌린여부 및 위치
		RightLeft.pos = { 488,851 };
		RightLeft.pressed = true;
		moveAngle--;
	}

	//▼키입력에 따른 카메라 이동
	if (KEYMANAGER->IsStayKeyDown(VK_DOWN) || KEYMANAGER->IsStayKeyDown('S'))
	{
		if (CAMERA.GetCameraRc().top < TILESIZE * TILEROWY - 680)
		{
			CAMERA.RefCameraRc().top += 5;
			CAMERA.RefCameraRc().bottom += 5;
		}
		//▼위아래 네모박스 눌린여부 및 위치
		UpDown.pos = { 540,862 };
		UpDown.pressed = true;
	}

	//▼키입력에 따른 카메라 이동
	else if (KEYMANAGER->IsStayKeyDown(VK_UP) || KEYMANAGER->IsStayKeyDown('W'))
	{
		if (CAMERA.GetCameraRc().top >0)
		{
			CAMERA.RefCameraRc().top -= 5;
			CAMERA.RefCameraRc().bottom -= 5;
		}
		//▼위아래 네모박스 눌린여부 및 위치
		UpDown.pos = { 540,834 };
		UpDown.pressed = true;
	}
}

RECT MapToolScene::reLocate(RECT _rc)
{
	_rc.left += 158;
	_rc.right += 158;
	_rc.top += 138;
	_rc.bottom += 138;
	_rc = CAMERA.RelativeCameraRect(_rc);
	return _rc;
}

void MapToolScene::Render()
{
	
	IMAGEMANAGER->FindImage("MapToolBg1")->Render(0, 0);

	for (int i = 0; i < TILEROWY * TILECOLX; i++)
	{
		if (IntersectRect(&tempC, &arrField[i].GetPosition(), &CAMERA.GetCameraRc()))
		{
			
			//RECT toDraw = arrField[i].GetPosition();
			//toDraw.left += 158;
			//toDraw.right += 158;
			//toDraw.top += 138;
			//toDraw.bottom += 138;
			//toDraw = CAMERA.RelativeCameraRect(toDraw);
			
			D2DRENDERER->DrawRectangle(reLocate(arrField[i].GetPosition()));
		}
	}

	IMAGEMANAGER->FindImage("MapToolMat2")->Render(1200, 140);
	IMAGEMANAGER->FindImage("MapToolMapbox")->Render(108, 90);

	if (UpDown.pressed)
	{
		IMAGEMANAGER->FindImage("Red")->SetAlpha(0.5);
		IMAGEMANAGER->FindImage("Red")->SetSize({ 36,24 });
		IMAGEMANAGER->FindImage("Red")->Render(UpDown.pos.x, UpDown.pos.y);
	}
	if (RightLeft.pressed)
	{
		IMAGEMANAGER->FindImage("Red")->SetAlpha(0.5);
		IMAGEMANAGER->FindImage("Red")->SetSize({ 36,24 });
		IMAGEMANAGER->FindImage("Red")->Render(RightLeft.pos.x, RightLeft.pos.y);
	}
	IMAGEMANAGER->FindImage("MapToolTitle")->Render(800, 70, Pivot::Centre);
	IMAGEMANAGER->FindImage("oldpaper")->SkewRenderRotate(WINSIZEX / 2, WINSIZEY / 2,moveAngle + 18, 0);
	IMAGEMANAGER->FindImage("oldpaper")->SkewRenderRotate(WINSIZEX / 2, WINSIZEY / 2,moveAngle + 16, 0);
	IMAGEMANAGER->FindImage("oldpaper")->SkewRenderRotate(WINSIZEX / 2, WINSIZEY / 2,moveAngle + 14, 0);
	IMAGEMANAGER->FindImage("oldpaper")->SkewRenderRotate(WINSIZEX / 2, WINSIZEY / 2,moveAngle + 12, 0);
	IMAGEMANAGER->FindImage("oldpaper")->SkewRenderRotate(WINSIZEX / 2, WINSIZEY / 2,moveAngle + 10, 0);
	IMAGEMANAGER->FindImage("oldpaper")->SkewRenderRotate(WINSIZEX / 2, WINSIZEY / 2,moveAngle + 8, 0);
	IMAGEMANAGER->FindImage("oldpaper")->SkewRenderRotate(WINSIZEX / 2, WINSIZEY / 2,moveAngle + 6, 0);
	IMAGEMANAGER->FindImage("oldpaper")->SkewRenderRotate(WINSIZEX / 2, WINSIZEY / 2,moveAngle + 4, 0);
	IMAGEMANAGER->FindImage("oldpaper")->SkewRenderRotate(WINSIZEX / 2, WINSIZEY / 2,moveAngle + 2, 0);
	IMAGEMANAGER->FindImage("oldpaper")->SkewRenderRotate(WINSIZEX / 2, WINSIZEY / 2,moveAngle + 0, 0);
	D2DRENDERER->RenderText(300, 100,std::to_wstring( moveAngle),50);
	IMAGEMANAGER->FindImage("MapToolBg2")->Render(paperScrollX, 0);
}

