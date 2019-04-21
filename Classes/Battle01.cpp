#include "../stdafx.h"
#include "Battle01.h"
#include "Tiles.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "TurnManager.h"
#include "ESCMenu.h"

Battle01::Battle01()
{
	bg = nullptr;
	bg2 = nullptr;
}

void Battle01::Init()
{
	escMenu = new ESCMenu;
	DATACENTRE.AddObj(ObjType::UI, "TurnManager", dynamic_cast<GameObject*>(escMenu));
	turnManager = new TurnManager;
	DATACENTRE.AddObj(ObjType::UI, "TurnManager", dynamic_cast<GameObject*>(turnManager));
	player = new Player;
	enemy = new Enemy;

	CAMERA.SetCamera({ 0,0,WINSIZEX,WINSIZEY });
	bg = IMAGEMANAGER->AddImage("tmp", L"IMAGE/Tiles/temp.png");
	bg2 = IMAGEMANAGER->AddImage("tmp", L"IMAGE/Tiles/temp2.png");

	arrField = new Tiles[TILECOLX * TILEROWY];

	for (int j = 0; j < TILEROWY; j++)
	{
		for (int i = 0; i < TILECOLX; i++)
		{
			arrField[j * TILECOLX + i].Init(); //생성된 타일 초기화
			arrField[j * TILECOLX + i].SetPosition(RectMake(i * TILESIZE, j * TILESIZE, TILESIZE, TILESIZE));
			arrField[j * TILECOLX + i].SetIndex({ i,j });
			if ((i == 2 && j == 4) || (i == 5 && j == 4) || (i == 6 && j == 5))
			{
				arrField[j * TILECOLX + i].SetObj("tmp");
			}
			DATACENTRE.AddObj(ObjType::Tile, std::to_string(j * TILECOLX + i), (GameObject*)(&arrField[j * TILECOLX + i]));
		}
	}
	player->SetTurnStart(); //수정필요
}

void Battle01::Release()
{
	//현재 씬에서 뉴할당 받은 애들 역순으로 제거

	DATACENTRE.GetObjects(ObjType::Tile).clear();
	DATACENTRE.GetObjects(ObjType::UI).clear();


	SAFE_DELETE_ARRAY(arrField);
	SAFE_DELETE(enemy);
	SAFE_DELETE(player);
	SAFE_DELETE(turnManager);
	SAFE_DELETE(escMenu);
}

void Battle01::Update()
{
	if (player->GetTurnStatus()) player->Update();
	else if (enemy->GetTurnStatus()) enemy->Update();
	
}
void Battle01::LoadFromFile()
{
}

void Battle01::SaveToFile()
{
}


void Battle01::Render()
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

