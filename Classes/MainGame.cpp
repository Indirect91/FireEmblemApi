#include "../stdafx.h"
#include "MainGame.h"
#include "TitleScene.h"
#include "MapToolScene.h"
#include "testScene.h"

MainGame::MainGame()
{
	srand(GetTickCount()); //�����ʱ�ȭ
}


MainGame::~MainGame()
{
}

void MainGame::Init()
{
	SOUNDMANAGER->Init();
	SCENEMANAGER->AddScene("TitleScene", new TitleScene);
	SCENEMANAGER->AddScene("MapToolScene", new MapToolScene);
	SCENEMANAGER->AddScene("test", new testScene);
	SCENEMANAGER->LoadScene("test");
	//SCENEMANAGER->LoadScene("TitleScene");
}

void MainGame::Release()
{
	//�̱���� ����������
	ImageManager::Delete();
	D2DRenderer::Delete();
	KeyManager::Delete();
	SceneManager::Delete();
	TimeManager::Delete();
	SOUNDMANAGER->Release();
	SoundManager::Delete();
}

void MainGame::Update()
{
	SOUNDMANAGER->Update();
	SCENEMANAGER->Update();
}

void MainGame::Render()
{
	D2DRENDERER->BeginRender();
	/********************************************************/
	
	SCENEMANAGER->Render();
	TIMEMANAGER->Render();

	/********************************************************/
	D2DRENDERER->EndRender();
}
