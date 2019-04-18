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
			arrField[i * TILEROWY + j].Init(); //������ Ÿ�� �ʱ�ȭ
			arrField[i * TILEROWY + j].SetPosition(RectMake(j * TILESIZE, i * TILESIZE, TILESIZE, TILESIZE));
		}
	}
	
	player->SetTurnStart();
}

void FirstScene::Release()
{
	//���� ������ ���Ҵ� ���� �ֵ� �������� ����
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
			IMAGEMANAGER->FindImage("ĳ����Chrome")->SetSize({ 48, 48 });
			IMAGEMANAGER->FindImage("ĳ����Chrome")->SetReverseX(true);
			IMAGEMANAGER->FindImage("ĳ����Chrome")->FrameRender(48*6, 48*6, 0, 0);
			
			IMAGEMANAGER->FindImage("ĳ����Virion")->SetSize({ 48, 48 });
			IMAGEMANAGER->FindImage("ĳ����Virion")->FrameRender(48 * 12, 48 * 12, 0, 0);

		}
	}
}
