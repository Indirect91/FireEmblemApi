#include "../stdafx.h"
#include "MapToolScene.h"
#include "Tiles.h"

MapToolScene::MapToolScene()
{
	moveWindowRateX = 0;	  
	moveWindowRateAccX = 0;
	isMovingDone = 0;
	paperScrollX = 0;

	moveAngle = 0;
	field =0;
	sheetV.push_back(L"MovingTile");
	sheetV.push_back(L"rock");
	sheetV.push_back(L"grass");
	sheetV.push_back(L"object");

	for (UINT j = 0; j < 5; j++)
	{
		for (UINT i = 0; i < 4; i++)
		{
			pallet.push_back(RectMake( 1230 + i*TILESIZE,240 + j*TILESIZE,TILESIZE,TILESIZE ));

		}
	}
}


void MapToolScene::Init()
{
	//▼윈도우 및 커버 초기화
	SCENEMANAGER->RefCoverSpeed() = 0.035f;
	SetCoverStatue::FadeIn;
	moveWindowRateX = 789.5;	//윈도우 이동시킬 초기값
	moveWindowRateAccX = 3; //윈도우 이동시킬 수치 가속                 
	isMovingDone = false;
	paperScrollX = static_cast<UINT>(moveWindowRateX - IMAGEMANAGER->FindImage("MapToolBg2")->GetWidth());
	moveAngle = 0;

	//▼음악 초기화. 맵툴은 타이틀서부터밖에 못넘어온다
	SOUNDMANAGER->pause("타이틀BGM");
	SOUNDMANAGER->play("맵툴BGM");
	//SOUNDMANAGER->pause("SelectionTitleComplete");
	//SOUNDMANAGER->setFadeIn();
	
	//▼눌린키 네모 초기화
	RightLeft.pos = { -50,-50 };
	RightLeft.pressed = false;
	UpDown.pos = { -50,-50 };
	UpDown.pressed = false;


	//▼카메라 및 타일들 초기화
	CAMERA.SetCamera({0,0,815,688});
	field = new Tiles[TILECOLX * TILEROWY];
	
	for (int j = 0; j < TILEROWY; j++)
	{
		for (int i = 0; i < TILECOLX; i++)
		{
			field[j * TILECOLX + i].Init(); //생성된 타일 초기화
			field[j * TILECOLX + i].SetPosition(RectMake(i * TILESIZE, j * TILESIZE, TILESIZE, TILESIZE));
			field[j * TILECOLX + i].SetIndex({ i,j });
			field[j * TILECOLX + i].SetPositionViaIndex();
			DATACENTRE.AddObj(ObjType::Tile, std::to_string(j * TILECOLX + i), &field[j * TILECOLX + i]);
		}
	}
	tempC = { 0 }; //충돌에 쓸 임시렉트 0
	
}

void MapToolScene::Release()
{
	//맵툴서 뉴할당받은건 타일들
	//SAFE_DELETE_ARRAY(field);
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


	if ((PtInRect(&changePalletPrev, _ptMouse) && whichSheet != (WhichSheet)0))
	{
		if(KEYMANAGER->IsOnceKeyDown(VK_LBUTTON))
		{
			whichSheet = (WhichSheet)((INT)whichSheet - 1);
			
		}
	}
	else if ((PtInRect(&changePalletNext, _ptMouse) && whichSheet != (WhichSheet)3))
	{
		if (KEYMANAGER->IsOnceKeyDown(VK_LBUTTON))
		{
			whichSheet = (WhichSheet)((INT)whichSheet + 1);
		}

	}











	switch (whichSheet)
	{
	case MapToolScene::WhichSheet::MovingTile:
		break;
	case MapToolScene::WhichSheet::rock:
		break;
	case MapToolScene::WhichSheet::grass:
		break;
	case MapToolScene::WhichSheet::object:
		break;
	default:
		break;
	}













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
	paperScrollX = static_cast<UINT>(moveWindowRateX - IMAGEMANAGER->FindImage("MapToolBg2")->GetWidth());
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
		if (CAMERA.GetCameraRc().right < TILESIZE * TILECOLX+2)
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
		if (CAMERA.GetCameraRc().bottom < TILESIZE * TILEROWY+2)
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
		if (IntersectRect(&tempC, &field[i].GetPosition(), &CAMERA.GetCameraRc()))
		{
			D2DRENDERER->DrawRectangle(reLocate(field[i].GetPosition()));
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

	

	IMAGEMANAGER->FindImage(WStringToString(sheetV[(INT)whichSheet]))->Render(1230,240);

	D2DRENDERER->RenderText(1232, 498, sheetV[(INT)whichSheet],20);
		
	for (auto plt : pallet)
	{
		D2DRENDERER->DrawRectangle(plt);
	}






	IMAGEMANAGER->FindImage("MapToolBg2")->Render(paperScrollX, 0);
}

