#include "../stdafx.h"
#include "MainGame.h"
#include "LoadingScene.h"

MainGame::MainGame()
{
	srand(GetTickCount64()); //랜덤초기화
}


MainGame::~MainGame()
{
}

void MainGame::Init()
{
	SOUNDMANAGER->Init();
	SCENEMANAGER->AddScene("LoadingScene", new LoadingScene);
	SCENEMANAGER->LoadScene("LoadingScene");
	ShowCursor(false); //나중에 최후에 다시 주석을 풀도록 하자
}

void MainGame::Release()
{
	//싱글톤들 해제해주자
	ImageManager::Delete();
	D2DRenderer::Delete();
	KeyManager::Delete();
	SceneManager::Delete();
	TimeManager::Delete();
	SOUNDMANAGER->Release();
	SoundManager::Delete();
	DATACENTRE.Release();
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
	IMAGEMANAGER->FindImage("Cursor")->Render(_ptMouse.x,_ptMouse.y);

	/********************************************************/
	D2DRENDERER->EndRender();
}
