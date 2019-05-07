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
	//�������� �� Ŀ�� �ʱ�ȭ
	SCENEMANAGER->RefCoverSpeed() = 0.035f;
	SetCoverStatue::FadeIn;
	moveWindowRateX = 789.5;	//������ �̵���ų �ʱⰪ
	moveWindowRateAccX = 3; //������ �̵���ų ��ġ ����                 
	isMovingDone = false;
	paperScrollX = static_cast<UINT>(moveWindowRateX - IMAGEMANAGER->FindImage("MapToolBg2")->GetWidth());
	moveAngle = 0;

	//������ �ʱ�ȭ. ������ Ÿ��Ʋ�����͹ۿ� ���Ѿ�´�
	SOUNDMANAGER->pause("Ÿ��ƲBGM");
	SOUNDMANAGER->play("����BGM");
	//SOUNDMANAGER->pause("SelectionTitleComplete");
	//SOUNDMANAGER->setFadeIn();
	
	//�崭��Ű �׸� �ʱ�ȭ
	RightLeft.pos = { -50,-50 };
	RightLeft.pressed = false;
	UpDown.pos = { -50,-50 };
	UpDown.pressed = false;


	//��ī�޶� �� Ÿ�ϵ� �ʱ�ȭ
	CAMERA.SetCamera({0,0,815,688});
	field = new Tiles[TILECOLX * TILEROWY];
	
	for (int j = 0; j < TILEROWY; j++)
	{
		for (int i = 0; i < TILECOLX; i++)
		{
			field[j * TILECOLX + i].Init(); //������ Ÿ�� �ʱ�ȭ
			field[j * TILECOLX + i].SetPosition(RectMake(i * TILESIZE, j * TILESIZE, TILESIZE, TILESIZE));
			field[j * TILECOLX + i].SetIndex({ i,j });
			field[j * TILECOLX + i].SetPositionViaIndex();
			DATACENTRE.AddObj(ObjType::Tile, std::to_string(j * TILECOLX + i), &field[j * TILECOLX + i]);
		}
	}
	tempC = { 0 }; //�浹�� �� �ӽ÷�Ʈ 0
	
}

void MapToolScene::Release()
{
	//������ ���Ҵ������ Ÿ�ϵ�
	//SAFE_DELETE_ARRAY(field);
}

void MapToolScene::Update()
{
	if (!isMovingDone) moveWindowFrame(); //������ �̵��Ϸ���� �̵�
	moveCamera(); //����Ű �Է¿� ���� ȭ�� �̵�
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
	//�������� 1600���� ���ߴ� �ڵ�
	if (moveWindowRateX >= 1600)
	{
		moveWindowRateX = 1600;
		isMovingDone = true;
	}
	else moveWindowRateX += moveWindowRateAccX;

	if (moveWindowRateAccX < 300) moveWindowRateAccX += 1.5f;
	else if (moveWindowRateAccX >= 300) moveWindowRateAccX = 300;

	//������� ������ �̵��ϸ� �������°�
	SetWindowPos(_hWnd, 0, 150, 130, moveWindowRateX, 900, SWP_NOZORDER| SWP_NOMOVE);
	//�巣��Ÿ�ٺ������� ����� �ػ󵵿� �°� ����
	D2DRENDERER->GetRenderTarget()->Resize({ static_cast<UINT32>(moveWindowRateX),900 });
	paperScrollX = static_cast<UINT>(moveWindowRateX - IMAGEMANAGER->FindImage("MapToolBg2")->GetWidth());
}

void MapToolScene::moveCamera()
{
	//�崭��Ű ǥ�ù� �׸� �� �������� �ϴ��� �ȴ������·� �ʱ�ȭ
	RightLeft.pos = {-50,-50};
	RightLeft.pressed = false;
	UpDown.pos = {-50,-50};
	UpDown.pressed = false;

	//��Ű�Է¿� ���� ī�޶� �̵�
	if (KEYMANAGER->IsStayKeyDown(VK_RIGHT) || KEYMANAGER->IsStayKeyDown('D'))
	{
		if (CAMERA.GetCameraRc().right < TILESIZE * TILECOLX+2)
		{
			CAMERA.RefCameraRc().left += 5;
			CAMERA.RefCameraRc().right += 5;
		}
		//���¿� �׸�ڽ� �������� �� ��ġ
		RightLeft.pos = { 592,851 };
		RightLeft.pressed = true;

		moveAngle++;
	}

	//��Ű�Է¿� ���� ī�޶� �̵�
	else if (KEYMANAGER->IsStayKeyDown(VK_LEFT) || KEYMANAGER->IsStayKeyDown('A'))
	{
		if (CAMERA.GetCameraRc().left > 0)
		{
			CAMERA.RefCameraRc().left -= 5;
			CAMERA.RefCameraRc().right -= 5;
		}
		//���¿� �׸�ڽ� �������� �� ��ġ
		RightLeft.pos = { 488,851 };
		RightLeft.pressed = true;
		moveAngle--;
	}

	//��Ű�Է¿� ���� ī�޶� �̵�
	if (KEYMANAGER->IsStayKeyDown(VK_DOWN) || KEYMANAGER->IsStayKeyDown('S'))
	{
		if (CAMERA.GetCameraRc().bottom < TILESIZE * TILEROWY+2)
		{
			CAMERA.RefCameraRc().top += 5;
			CAMERA.RefCameraRc().bottom += 5;
		}
		//�����Ʒ� �׸�ڽ� �������� �� ��ġ
		UpDown.pos = { 540,862 };
		UpDown.pressed = true;
	}

	//��Ű�Է¿� ���� ī�޶� �̵�
	else if (KEYMANAGER->IsStayKeyDown(VK_UP) || KEYMANAGER->IsStayKeyDown('W'))
	{
		if (CAMERA.GetCameraRc().top >0)
		{
			CAMERA.RefCameraRc().top -= 5;
			CAMERA.RefCameraRc().bottom -= 5;
		}
		//�����Ʒ� �׸�ڽ� �������� �� ��ġ
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

