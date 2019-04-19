#include "../stdafx.h"
#include "FirstScene.h"
#include "Tiles.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"

FirstScene::FirstScene()
{
	player = nullptr;
	arrField = nullptr;
	enemy = nullptr;
	
}

void FirstScene::Init()
{
	CAMERA.SetCamera({ 0,0,WINSIZEX,WINSIZEY });
	bg = IMAGEMANAGER->AddImage("tmp", L"IMAGE/Tiles/temp.png");
	bg2 = IMAGEMANAGER->AddImage("tmp", L"IMAGE/Tiles/temp2.png");
	player = new Player;
	enemy = new Enemy;

	arrField = new Tiles[TILECOLX * TILEROWY];

	for (int j = 0; j < TILEROWY; j++)
	{
		for (int i = 0; i < TILECOLX; i++)
		{
			arrField[j * TILECOLX + i].Init(); //������ Ÿ�� �ʱ�ȭ
			arrField[j * TILECOLX + i].SetPosition(RectMake(i * TILESIZE, j * TILESIZE, TILESIZE, TILESIZE));
			arrField[j * TILECOLX + i].SetIndex({ i,j });
			if ((i == 2 && j == 4) || (i == 5 && j == 4) || (i == 6 && j == 5))
			{
				arrField[j * TILECOLX + i].SetObj("tmp");
			}
			DATACENTRE.AddObj(ObjType::Tile, std::to_string(j * TILECOLX + i), (GameObject*)(&arrField[j * TILECOLX + i]));
		}
	}
	
	player->SetTurnStart();
}

void FirstScene::Release()
{
	//���� ������ ���Ҵ� ���� �ֵ� �������� ����

	DATACENTRE.GetObjects(ObjType::Tile).clear();
	SAFE_DELETE_ARRAY(arrField);

	SAFE_DELETE(enemy);
	SAFE_DELETE(player);
}

void FirstScene::Update()
{

	if (player->GetTurnStatus()) player->Update();
	else if (enemy->GetTurnStatus()) enemy->Update();
	
}
void FirstScene::LoadFromFile()
{
}

void FirstScene::SaveToFile()
{
}


void FirstScene::Render()
{
	bg->Render(0, 0);
	bg2->Render(288, 0);
	for (int i = 0; i < TILEROWY * TILECOLX; i++)
	{
		if (IntersectRect(&tempC, &arrField[i].GetPosition(), &CAMERA.GetCameraRc()))
		{
			D2DRENDERER->DrawRectangle((arrField[i].GetPosition()),D2DRenderer::DefaultBrush::White,2);
			if (arrField[i].isBlue())
			{
				D2DRENDERER->FillRectangle(arrField[i].GetPosition(), D2D1::ColorF::Blue, 0.3f);
			}
			else if (arrField[i].GetObj() != "")
			{
				D2DRENDERER->FillRectangle(arrField[i].GetPosition(), D2D1::ColorF::Red, 0.2f);
			}
		}

		if (PtInRect(&arrField[i].GetPosition(), _ptMouse))
		{
			IMAGEMANAGER->FindImage("SelectedTile")->SetSize({ TILESIZE, TILESIZE });
			IMAGEMANAGER->FindImage("SelectedTile")->Render(arrField[i].GetPosition().left, arrField[i].GetPosition().top);
		}

	}
	DATACENTRE.Render();
	
	player->Render();
	
	enemy->Render();
}

