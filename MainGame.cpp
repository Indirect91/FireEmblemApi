#include "stdafx.h"
#include "MainGame.h"
#include "TitleScene.h"

MainGame::MainGame()
{
	srand(GetTickCount()); //·£´ıÃÊ±âÈ­
}


MainGame::~MainGame()
{
}

void MainGame::Init()
{
	SOUNDMANAGER->Init();

}

void MainGame::Release()
{
	//½Ì±ÛÅæµé ÇØÁ¦ÇØÁÖÀÚ
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
