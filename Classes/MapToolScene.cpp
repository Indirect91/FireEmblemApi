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

	//���ķ�Ʈ�� ��Ʈ��
	for (UINT j = 0; j < 5; j++)
	{
		for (UINT i = 0; i < 4; i++)
		{
			pallet.push_back(RectMake( 1230 + i*TILESIZE,240 + j*TILESIZE,TILESIZE,TILESIZE ));

		}
	}
	field = new Tiles[TILECOLX * TILEROWY];

	movingTilex = 0;

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
	
	
	for (int j = 0; j < TILEROWY; j++)
	{
		for (int i = 0; i < TILECOLX; i++)
		{
			field[j * TILECOLX + i].Init(); //������ Ÿ�� �ʱ�ȭ
			field[j * TILECOLX + i].SetPosition(RectMake(i * TILESIZE, j * TILESIZE, TILESIZE, TILESIZE));
			field[j * TILECOLX + i].SetIndex({ i,j });
			field[j * TILECOLX + i].SetPositionViaIndex();
			//DATACENTRE.AddObj(ObjType::Tile, std::to_string(j * TILECOLX + i), &field[j * TILECOLX + i]);
						//��4���� �̿� �߰�
			//if (i > 0) //�� ���� ����ó��
			//	field[j * TILECOLX + i].RefNeighbours().push_back(&field[(j + 0) * TILECOLX + (i - 1)]);
			//if (i < TILECOLX - 1) //�� ���� ����ó��
			//	field[j * TILECOLX + i].RefNeighbours().push_back(&field[(j + 0) * TILECOLX + (i + 1)]);
			//if (j > 0) //���κ� ����ó��
			//	field[j * TILECOLX + i].RefNeighbours().push_back(&field[(j - 1) * TILECOLX + (i + 0)]);
			//if (j < TILEROWY - 1) // �ٴ� ����ó��
			//	field[j * TILECOLX + i].RefNeighbours().push_back(&field[(j + 1) * TILECOLX + (i + 0)]);
		}
	}
	movingTilex = 0;
	whichSheet = WhichSheet::MovingTile;
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


	if ((PtInRect(&changePalletPrev, _ptMouse) && whichSheet != (WhichSheet)0))
	{
		maptoolCursor.movingT = "";
		maptoolCursor.objectT = "";
		maptoolCursor.terrain = "";
		if(KEYMANAGER->IsOnceKeyDown(VK_LBUTTON))
		{
			whichSheet = (WhichSheet)((INT)whichSheet - 1);
			
		}
	}
	else if ((PtInRect(&changePalletNext, _ptMouse) && whichSheet != (WhichSheet)3))
	{
		maptoolCursor.movingT = "";
		maptoolCursor.objectT = "";
		maptoolCursor.terrain = "";
		if (KEYMANAGER->IsOnceKeyDown(VK_LBUTTON))
		{
			whichSheet = (WhichSheet)((INT)whichSheet + 1);
		}
	}

	if (KEYMANAGER->IsOnceKeyDown(VK_RBUTTON))
	{
		maptoolCursor.movingT = "";
		maptoolCursor.objectT = "";
		maptoolCursor.terrain = "";
	}

	for (UINT i = 0; i< pallet.size(); i++)
	{
		if (PtInRect(&pallet[i], _ptMouse))
		{
			if (KEYMANAGER->IsOnceKeyDown(VK_LBUTTON))
			{
				if (sheetV[(INT)whichSheet] == L"MovingTile")
				{
					if (i == TwoDimentionArrayToInt({ 0,0 }, 4))
					{
						maptoolCursor.movingT = "LavaOrange";
						maptoolCursor.movingtFrame = { 0,0 };
						maptoolCursor.terrain = "";
						maptoolCursor.objectT = "";
						break;
					}
					else if (i == TwoDimentionArrayToInt({ 1,0 }, 4))
					{
						maptoolCursor.movingT = "Sand";
						maptoolCursor.movingtFrame = { 0,0 };
						maptoolCursor.terrain = "";
						maptoolCursor.objectT = "";
						break;
					}
					else if (i == TwoDimentionArrayToInt({ 2,0 }, 4))
					{
						maptoolCursor.movingT = "LavaYellow";
						maptoolCursor.movingtFrame = { 0,0 };
						maptoolCursor.terrain = "";
						maptoolCursor.objectT = "";
						break;
					}
					else if (i == TwoDimentionArrayToInt({ 0,1 }, 4))
					{
						maptoolCursor.movingT = "WaterDeep";
						maptoolCursor.movingtFrame = { 0,0 };
						maptoolCursor.terrain = "";
						maptoolCursor.objectT = "";
						break;
					}
					else if (i == TwoDimentionArrayToInt({ 1,1 }, 4))
					{
						maptoolCursor.movingT = "WaterShallow";
						maptoolCursor.movingtFrame = {0,0};
						maptoolCursor.terrain = "";
						maptoolCursor.objectT = "";
						break;
					}
				}
				else if (sheetV[(INT)whichSheet] == L"object")
				{
					if (i != TwoDimentionArrayToInt({ 3,1 }, 4) && i != TwoDimentionArrayToInt({ 3,2 }, 4) && i != TwoDimentionArrayToInt({ 3,3 }, 4) && i != TwoDimentionArrayToInt({ 3,4 }, 4))
					{
						maptoolCursor.objectT = WStringToString(sheetV[(INT)whichSheet]);
						maptoolCursor.objTFrame = SingleArrayToPoint(i, 4);
						maptoolCursor.terrain = "";
						maptoolCursor.movingT = "";
						break;
					}
				}
				else if (sheetV[(INT)whichSheet] == L"grass")
				{
					maptoolCursor.terrain = WStringToString(sheetV[(INT)whichSheet]);
					maptoolCursor.terrainFrame = SingleArrayToPoint(i, 4);
					maptoolCursor.movingT = "";
					maptoolCursor.objectT = "";
					break;
				}
				else
				{
					if (i != TwoDimentionArrayToInt({ 2,0 }, 4) && i != TwoDimentionArrayToInt({ 3,0 }, 4) && i != TwoDimentionArrayToInt({ 3,1 }, 4) && i != TwoDimentionArrayToInt({1,3}, 4) && i != TwoDimentionArrayToInt({ 2,3 }, 4) && i != TwoDimentionArrayToInt({ 3,3 }, 4) && i != TwoDimentionArrayToInt({1,4 }, 4) && i != TwoDimentionArrayToInt({2,4 }, 4) && i != TwoDimentionArrayToInt({3,4 }, 4))
					{
						maptoolCursor.objectT = WStringToString(sheetV[(INT)whichSheet]);
						maptoolCursor.objTFrame = SingleArrayToPoint(i, 4);
						maptoolCursor.movingT = "";
						maptoolCursor.terrain = "";
						break;
					}
				}
			}
		}
	}
	if (PtInRect(&selectArea, _ptMouse) && !((maptoolCursor.terrain == "") && (maptoolCursor.objectT == "") && (maptoolCursor.movingT == "")))
	{
		for (UINT i = 0; i < TILECOLX * TILEROWY; i++)
		{
			RECT toCompare = reLocate(field[i].GetPosition());
			if (PtInRect(&toCompare, _ptMouse))
			{
				if (KEYMANAGER->IsStayKeyDown(VK_LBUTTON))
				{
					if (maptoolCursor.terrain != "")
					{
						field[i].SetTerrain(maptoolCursor.terrain);
						field[i].SetTerrainFrame(maptoolCursor.terrainFrame);
						break;
					}
					else if (maptoolCursor.objectT != "")
					{
						field[i].SetObjT(maptoolCursor.objectT);
						field[i].SetObjTFrame(maptoolCursor.objTFrame);
						break;
					}
					else if (maptoolCursor.movingT != "")
					{
						field[i].SetMovingT(maptoolCursor.movingT);
						field[i].SetMovingtFrame({ 0,0 });
						break;
					}
					else
					{
						assert(!"���� ����� �ȵǴµ�");
					}
				}
			}
		}
	}


	if (PtInRect(&saveButton, _ptMouse) && KEYMANAGER->IsOnceKeyDown(VK_LBUTTON))
	{
		HANDLE file;
		DWORD write;

		file = CreateFile(L"saveFile.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);
		WriteFile(file, field, sizeof(Tiles)* TILECOLX* TILEROWY, &write, NULL);
		CloseHandle(file);
	}
	else if (PtInRect(&exitButton, _ptMouse) && KEYMANAGER->IsOnceKeyDown(VK_LBUTTON))
	{
		//SCENEMANAGER->LoadScene("TitleScene");
		HANDLE file;
		DWORD read;

		file = CreateFile(L"saveFile.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL);
		ReadFile(file, field, sizeof(Tiles)* TILECOLX* TILEROWY, &read, NULL);
		CloseHandle(file);
	}














	counter++;
	if (counter % 5 == 0)
	{
		movingTilex++;
		if (movingTilex > 14) movingTilex = 0;
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
			RECT relocatedRc = reLocate(field[i].GetPosition());
			D2DRENDERER->DrawRectangle(relocatedRc);
			if (field[i].GetMovingT() != "")
			{
				IMAGEMANAGER->FindImage(field[i].GetMovingT())->SetSize(IMAGEMANAGER->FindImage(field[i].GetMovingT())->GetFrameSize());
				IMAGEMANAGER->FindImage(field[i].GetMovingT())->FrameRender(relocatedRc.left, relocatedRc.top, movingTilex, 0);
			}
			if (field[i].GetTerrain() != "")
			{
				IMAGEMANAGER->FindImage(field[i].GetTerrain())->SetSize(IMAGEMANAGER->FindImage(field[i].GetTerrain())->GetFrameSize());
				IMAGEMANAGER->FindImage(field[i].GetTerrain())->FrameRender(relocatedRc.left, relocatedRc.top, field[i].GetTerrainFrame().x, field[i].GetTerrainFrame().y);
			}
			if (field[i].GetObjT() != "")
			{
				IMAGEMANAGER->FindImage(field[i].GetObjT())->SetSize(IMAGEMANAGER->FindImage(field[i].GetObjT())->GetFrameSize());
				IMAGEMANAGER->FindImage(field[i].GetObjT())->FrameRender(relocatedRc.left, relocatedRc.top, field[i].GetObjTFrame().x, field[i].GetObjTFrame().y);
			}
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

	if (maptoolCursor.terrain != "")
	{
		IMAGEMANAGER->FindImage("CursorBlue")->Render(_ptMouse.x, _ptMouse.y);
		IMAGEMANAGER->FindImage(maptoolCursor.terrain)->SetSize(IMAGEMANAGER->FindImage(maptoolCursor.terrain)->GetFrameSize());
		IMAGEMANAGER->FindImage(maptoolCursor.terrain)->FrameRender(_ptMouse.x+20, _ptMouse.y+20, maptoolCursor.terrainFrame.x, maptoolCursor.terrainFrame.y);
	}
	else if (maptoolCursor.movingT != "")
	{
		IMAGEMANAGER->FindImage("CursorBlue")->Render(_ptMouse.x, _ptMouse.y);
		IMAGEMANAGER->FindImage(maptoolCursor.movingT)->SetSize(IMAGEMANAGER->FindImage(maptoolCursor.movingT)->GetFrameSize());
		IMAGEMANAGER->FindImage(maptoolCursor.movingT)->FrameRender(_ptMouse.x+20, _ptMouse.y+20, maptoolCursor.movingtFrame.x, maptoolCursor.movingtFrame.y);
	}
	else if (maptoolCursor.objectT != "")
	{
		IMAGEMANAGER->FindImage("CursorBlue")->Render(_ptMouse.x, _ptMouse.y);
		IMAGEMANAGER->FindImage(maptoolCursor.objectT)->SetSize(IMAGEMANAGER->FindImage(maptoolCursor.objectT)->GetFrameSize());
		IMAGEMANAGER->FindImage(maptoolCursor.objectT)->FrameRender(_ptMouse.x+20, _ptMouse.y+20, maptoolCursor.objTFrame.x, maptoolCursor.objTFrame.y);
	}
	else
	{
		IMAGEMANAGER->FindImage("CursorBlue")->Render(_ptMouse.x, _ptMouse.y);
	}


	D2DRENDERER->DrawRectangle(exitButton);
	D2DRENDERER->DrawRectangle(saveButton);
	D2DRENDERER->DrawRectangle(selectArea);

	IMAGEMANAGER->FindImage("MapToolBg2")->Render(paperScrollX, 0);
}
