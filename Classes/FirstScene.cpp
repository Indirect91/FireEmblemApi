#include "../stdafx.h"
#include "FirstScene.h"
#include "Tiles.h"

FirstScene::FirstScene()
{
	arrField = nullptr;
	chrome = {48*5,48*5,48*5+TILESIZE,48*5+TILESIZE};
	virion = { 48 * 10,48 * 5,48 * 10 + TILESIZE,48 * 5 + TILESIZE };
}

void FirstScene::Init()
{
	CAMERA.SetCamera({ 0,0,WINSIZEX,WINSIZEY });

	arrField = new Tiles[TILECOLX * TILEROWY];

	for (int i = 0; i < TILEROWY; i++)
	{
		for (int j = 0; j < TILECOLX; j++)
		{
			arrField[i * TILEROWY + j].Init(); //생성된 타일 초기화
			arrField[i * TILEROWY + j].SetPosition(RectMake(j * TILESIZE, i * TILESIZE, TILESIZE, TILESIZE));
		}
	}
}

void FirstScene::Release()
{
}

void FirstScene::Update()
{
	
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
			IMAGEMANAGER->FindImage("캐릭터Chrome")->SetSize({ 48, 48 });
			IMAGEMANAGER->FindImage("캐릭터Chrome")->SetReverseX(true);
			IMAGEMANAGER->FindImage("캐릭터Chrome")->FrameRender(chrome.left, chrome.top, 0, 0);

			IMAGEMANAGER->FindImage("캐릭터Virion")->SetSize({ 48, 48 });
			IMAGEMANAGER->FindImage("캐릭터Virion")->FrameRender(virion.left, virion.top, 0, 0);

		}
	}
}

