#include "../stdafx.h"
#include "testScene.h"


testScene::testScene()
{
	IMAGEMANAGER->AddImage("temp", L"IMAGE/ETC/3.bmp");
}


testScene::~testScene()
{
}

void testScene::Init()
{
	for (int i = 0; i < 25; i++)
	{
		for (int j = 0; j < 25; j++)
		{
			testVec.push_back(RectMake(200 + j * 48, i * 48, 48, 48));
		}
	}
	SetCoverStatue::FadeIn;
}

void testScene::Release()
{
}

void testScene::Update()
{
	if (KEYMANAGER->IsOnceKeyDown(VK_RETURN))
		SCENEMANAGER->LoadScene("TitleScene");
}

void testScene::Render()
{
	for (auto a : testVec)
	{
		D2DRENDERER->DrawRectangle(a, D2DRenderer::DefaultBrush::Blue);
		//	IMAGEMANAGER->FindImage("temp")->SkewRender(a.left, a.top, 10, 0);
	}
	for (float i = 0; i < 25 * 25; i++)
	{
		//for (int j = 0; j < 25; j++)
		{
			IMAGEMANAGER->FindImage("temp")->SkewRender(testVec[i].left, testVec[i].top, (i / 25.f), 0);
		}
	}
}
