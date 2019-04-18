#include "../stdafx.h"
#include "testScene.h"


testScene::testScene()
{
	IMAGEMANAGER->AddImage("temp", L"IMAGE/ETC/3.bmp");
	IMAGEMANAGER->AddFrameImage("cherry", L"IMAGE/ETC/cherry.png",33,1);
	testAdditional = 0;
	original = 0;
	line = 1;
	cherryVisible = false;
	cherryLoc = WINSIZEX / 2+100;
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
			//testVec.push_back(RectMake(200 + j * (48 + j), i * (48 + i), 48 + j * 2, 48 + i * 2));
		}
		IMAGEMANAGER->AddFrameImage("cherry", L"IMAGE/ETC/cherry.png", 33, 0);
	}
	
	SetCoverStatue::FadeIn;
	testAdditional = 0.2f;
	for (int i = 0; i < 25*25; i++)
	{

		testAngle[i] = testAdditional * i;
	}
	original = IMAGEMANAGER->FindImage("temp")->GetWidth();
	testAdditional = 0.f;
}

void testScene::Release()
{
}

void testScene::Update()
{
	//SetWindowPos(_hWnd, 0,WINSTARTX, WINSTARTY,WINSIZEX-200,WINSIZEY, 0);
	//D2DRENDERER->GetRenderTarget()->Resize({ WINSIZEX - 200,WINSIZEY });
	if (KEYMANAGER->IsOnceKeyDown(VK_RETURN))
		SCENEMANAGER->LoadScene("TitleScene");

	if (KEYMANAGER->IsStayKeyDown(VK_UP))
	{
		testAdditional += 0.2f;
		for (int i = 0; i < 25*25; i++)
		{
			testAngle[i] = testAdditional * i;
		}
		
		cherryVisible = true;
		cherryX+=0.15f;
		cherryLoc -= 2;
		if (cherryX > 32)
			cherryX = 32;

	}



	if (KEYMANAGER->IsStayKeyDown(VK_DOWN))
	{
		cherryVisible = true;
		testAdditional -= 0.2f;
		for (int i = 0; i < 25*25; i++)
		{
			testAngle[i] = testAdditional * i;
		}



		cherryX-=0.15f;
		cherryLoc += 2;
		if (cherryX < 0)
			cherryX = 0;
	}

	if (KEYMANAGER->IsStayKeyDown(VK_RIGHT))
	{
		stat = state::up;
		cherryVisible = true;
		cherryX += 0.15f;
		cherryLoc -= 2;
		if (cherryX > 32)
			cherryX = 32;
	}

	if (KEYMANAGER->IsStayKeyDown(VK_LEFT))
	{
		stat = state::down;
		cherryVisible = true;
		cherryX += 0.15f;
		cherryLoc -= 2;
		if (cherryX > 32)
			cherryX = 32;
	}

	switch (stat)
	{
	case testScene::state::up:
		if(testAdditional<=2)
			testAdditional += 0.2f;
		else if (testAdditional > 2)
		{
			testAdditional = 2;
			stat = state::idle;
		}
		break;
	case testScene::state::down:
		if (testAdditional > 0)
			testAdditional -= 0.2f;
		else if (testAdditional <= 0)
		{
			testAdditional = 0;
			stat = state::idle;
		}
		break;
	case testScene::state::idle:
		break;
	default:
		break;
	}


}

void testScene::Render()
{
	//for (auto a : testVec)
	//{
	//	D2DRENDERER->DrawRectangle(a, D2DRenderer::DefaultBrush::Blue);
	//}

	line = 0.8;


	for (float i = 0; i < 25; i++)
	{
		for (float j = 0; j < 25; j++)
		{


			IMAGEMANAGER->FindImage("temp")->SetSize(
				{
					(FLOAT)(48 + (j * 2 * (testAdditional)) * ((FLOAT)(i) / 25.f)),
					(FLOAT)(48 + (i * 2 * (testAdditional)))
				});


			IMAGEMANAGER->FindImage("temp")->SkewRenderStretch
			(
				j* (48 + j* (testAdditional )* (FLOAT)(i) / 25.f),
				i* (48 + i* (testAdditional)),
				j * testAdditional, 0
			);
			if (i == 5 && j == 5)
			{
				IMAGEMANAGER->FindImage("캐릭터Virion")->SetSize({ 48, 48 });
				IMAGEMANAGER->FindImage("캐릭터Virion")->FrameRender
				(j * (48 + j * (testAdditional) * (FLOAT)(i) / 25.f) + ((FLOAT)(48 + (j * 2 * (testAdditional)) * ((FLOAT)(i) / 25.f))/2), 
					i * (48 + i * (testAdditional)) + ((FLOAT)(48 + (i * 2 * (testAdditional)))/2),
					0, 0,Pivot::Centre);
			}
		}
	}
	if(KEYMANAGER->IsToggleKey('Q'))
	{
		IMAGEMANAGER->FindImage("cherry")->SetSize(IMAGEMANAGER->FindImage("cherry")->GetFrameSize());
		if(cherryVisible)
		IMAGEMANAGER->FindImage("cherry")->FrameRender(cherryLoc, WINSIZEY / 2, (INT)cherryX, 0,Pivot::Centre);
	}
}
