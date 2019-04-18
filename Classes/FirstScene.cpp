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

	player = new Player;
	enemy = new Enemy;

	arrField = new Tiles[TILECOLX * TILEROWY];

	for (int i = 0; i < TILEROWY; i++)
	{
		for (int j = 0; j < TILECOLX; j++)
		{
			arrField[i * TILEROWY + j].Init(); //생성된 타일 초기화
			arrField[i * TILEROWY + j].SetPosition(RectMake(j * TILESIZE, i * TILESIZE, TILESIZE, TILESIZE));
			DATACENTRE.AddObj(ObjType::Tile, std::to_string(i * TILECOLX + j), (GameObject*)(&arrField[i * TILECOLX + j]));
		}
	}
	
	player->SetTurnStart();
}

void FirstScene::Release()
{
	//현재 씬에서 뉴할당 받은 애들 역순으로 제거
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
	for (int i = 0; i < TILEROWY * TILECOLX; i++)
	{
		if (IntersectRect(&tempC, &arrField[i].GetPosition(), &CAMERA.GetCameraRc()))
		{
			D2DRENDERER->DrawRectangle((arrField[i].GetPosition()),D2DRenderer::DefaultBrush::White,2);
		}
	}
	DATACENTRE.Render();
	
	player->Render();
	
	enemy->Render();
}

