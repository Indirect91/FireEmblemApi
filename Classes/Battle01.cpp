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
	escMenu = new ESCMenu; //ESC UI를 뉴할당받음
	DATACENTRE.AddObj(ObjType::UI, "escMenu", dynamic_cast<GameObject*>(escMenu)); //쓸 수 있게 등록함
	turnManager = new TurnManager; //턴관련 UI를 뉴할당받을
	DATACENTRE.AddObj(ObjType::UI, "TurnManager", dynamic_cast<GameObject*>(turnManager)); //쓸 수 있게 등록함
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

	DATACENTRE.GetObjects(ObjType::Tile).clear();
	DATACENTRE.GetObjects(ObjType::UI).clear();


	//▼현재 씬에서 뉴할당 받은 애들 역순으로 제거
	SAFE_DELETE_ARRAY(arrField);
	SAFE_DELETE(enemy);
	SAFE_DELETE(player);
	SAFE_DELETE(turnManager);
	SAFE_DELETE(escMenu);
}

void Battle01::Update()
{
	CursorFrameManage();

	switch (currentState)
	{
	case BattleScenes::ingameStatus::UI:
		
		break;
	case BattleScenes::ingameStatus::PlayerTurn:
		player->Update();
		break;
	case BattleScenes::ingameStatus::EnemyTurn:
		enemy->Update();
		break;
	}
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
			IMAGEMANAGER->FindImage("타일커서")->SetSize({ TILESIZE, TILESIZE });
			IMAGEMANAGER->FindImage("타일커서")->FrameRender(arrField[i].GetPosition().left, arrField[i].GetPosition().top,cursorFrame,0);
		}

	}
	DATACENTRE.Render();
	
	
	player->Render();
	enemy->Render();
}

