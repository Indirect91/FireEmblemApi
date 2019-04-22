#include "../stdafx.h"
#include "TileManager.h"
#include "Tiles.h"
#include "Cursor.h"

TileManager::TileManager()
{
	field = nullptr;
}

void TileManager::Init()
{
	field = new Tiles[TILECOLX * TILEROWY];

	for (int j = 0; j < TILEROWY; j++)
	{
		for (int i = 0; i < TILECOLX; i++)
		{
			field[j * TILECOLX + i].Init(); //������ Ÿ�� �ʱ�ȭ
			field[j * TILECOLX + i].SetPosition(RectMake(i * TILESIZE, j * TILESIZE, TILESIZE, TILESIZE));
			field[j * TILECOLX + i].SetIndex({ i,j });
			if ((i == 2 && j == 4) || (i == 5 && j == 4) || (i == 6 && j == 5))
			{
				field[j * TILECOLX + i].SetObj("tmp");
			}
			DATACENTRE.AddObj(ObjType::Tile, std::to_string(j * TILECOLX + i), &field[j * TILECOLX + i]);
		}
	}
	cursor = dynamic_cast<Cursor*>(DATACENTRE.GetCertainObject(ObjType::UI, "Cursor"));
}

void TileManager::Update()
{
	for (int i = 0; i < TILEROWY * TILECOLX; i++)
	{
		if (PtInRect(&field[i].GetPosition(), _ptMouse))
		{
			cursor->SetCursorVisibility(true);
			cursor->SetIndex(field[i].GetIndex());
		}
		else
		{
			cursor->SetCursorVisibility(false);
		}
	}
}

void TileManager::Release()
{
	DATACENTRE.ClearObjects(ObjType::Tile); //������ ���ͼ� ����ϴ� Ÿ�� �� �� ���
	for (int i = 0; i < TILEROWY * TILECOLX; i++) //���⼭ �̴ֶ� ���� ��� Ÿ�ϵ� ���鼭 ������
	{field[i].Release();}
	SAFE_DELETE_ARRAY(field);
}

void TileManager::Render()
{
	for (int i = 0; i < TILEROWY * TILECOLX; i++)
	{
		if (IntersectRect(&tempC, &field[i].GetPosition(), &CAMERA.GetCameraRc()))
		{
			D2DRENDERER->DrawRectangle((field[i].GetPosition()), D2DRenderer::DefaultBrush::White, 2);
			if (field[i].GetIsBlue())
			{
				//D2DRENDERER->FillRectangle(arrField[i].GetPosition(), D2D1::ColorF::Blue, arrField[i].GetBlueAlpha());
				IMAGEMANAGER->FindImage("Blue")->SetAlpha(field[i].GetBlueAlpha());
				IMAGEMANAGER->FindImage("Blue")->SetSize({ 48,48 });
				IMAGEMANAGER->FindImage("Blue")->Render(field[i].GetPosition().left, field[i].GetPosition().top);
			}
			else if (field[i].GetObj() != "")
			{
				D2DRENDERER->FillRectangle(field[i].GetPosition(), D2D1::ColorF::Red, field[i].GetBlueAlpha());
			}
		}
	}
	//DATACENTRE.Render();

}
