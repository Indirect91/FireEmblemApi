#include "../stdafx.h"
#include "MapToolScene.h"
#include "Tiles.h"



void MapToolScene::Init()
{
	//�������� �� Ŀ�� �ʱ�ȭ
	SCENEMANAGER->RefCoverSpeed() = 0.035f;
	SetCoverStatue::FadeIn;
	moveWindowRateX = 789.5;	//������ �̵���ų �ʱⰪ
	moveWindowRateAccX = 3; //������ �̵���ų ��ġ ����
	isMovingDone = false;
	paperScrollX = moveWindowRateX - IMAGEMANAGER->FindImage("MapToolBg2")->GetWidth();
	moveAngle = 0;

	//������ �ʱ�ȭ. ������ Ÿ��Ʋ�����͹ۿ� ���Ѿ�´�
	SOUNDMANAGER->pause("Ÿ��ƲBGM");
	SOUNDMANAGER->play("����BGM");
	SOUNDMANAGER->setFadeIn();
	
	//�崭��Ű �׸� �ʱ�ȭ
	RightLeft.pos = { -50,-50 };
	RightLeft.pressed = false;
	UpDown.pos = { -50,-50 };
	UpDown.pressed = false;
	
	////��� �̹��� 4��
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


	//��ī�޶� �� Ÿ�ϵ� �ʱ�ȭ
	CAMERA.SetCamera({0,0,815,688});
	arrField = new Tiles[TILECOLX * TILEROWY];
	
	for (int i = 0; i < TILEROWY; i++)
	{
		for (int j = 0; j < TILECOLX; j++)
		{
			arrField[i * TILEROWY + j].Init(); //������ Ÿ�� �ʱ�ȭ
			arrField[i * TILEROWY + j].SetPosition(RectMake(j * TILESIZE,i * TILESIZE, TILESIZE, TILESIZE));
		}
	}
	tempC = { 0 }; //�浹�� �� �ӽ÷�Ʈ 0
	
}

void MapToolScene::Release()
{
	//������ ���Ҵ������ Ÿ�ϵ�
	SAFE_DELETE_ARRAY(arrField);
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
	paperScrollX = moveWindowRateX - IMAGEMANAGER->FindImage("MapToolBg2")->GetWidth();
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
		if (CAMERA.GetCameraRc().right < TILESIZE * TILECOLX - 570)
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
		if (CAMERA.GetCameraRc().top < TILESIZE * TILEROWY - 680)
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

