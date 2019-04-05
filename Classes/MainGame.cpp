#include "../stdafx.h"
#include "MainGame.h"
#include "LoadingScene.h"

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
	SCENEMANAGER->AddScene("LoadingScene", new LoadingScene);
	SCENEMANAGER->LoadScene("LoadingScene");
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
