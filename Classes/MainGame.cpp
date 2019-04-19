#include "../stdafx.h"
#include "MainGame.h"
#include "LoadingScene.h"

MainGame::MainGame()
{
	srand(GetTickCount64()); //�����ʱ�ȭ
}


MainGame::~MainGame()
{
}

void MainGame::Init()
{
	SOUNDMANAGER->Init();
	SCENEMANAGER->AddScene("LoadingScene", new LoadingScene);
	SCENEMANAGER->LoadScene("LoadingScene");
	ShowCursor(false); //���߿� ���Ŀ� �ٽ� �ּ��� Ǯ���� ����
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
