#include "../stdafx.h"
#include "SaveloadScene.h"

SaveloadScene::SaveloadScene()
{
	SaveSlots = {0};
	SelectionBox = {0};
}

SaveloadScene::~SaveloadScene()
{

}

void SaveloadScene::Init()
{

}

void SaveloadScene::Release()
{
	//뉴할당 해준것이 없으니 무시
}

void SaveloadScene::Update()
{
	if (KEYMANAGER->IsOnceKeyDown(VK_RETURN))
	{

		SCENEMANAGER->LoadScene("Battle01");
	}
	else if (KEYMANAGER->IsOnceKeyDown('Q'))
	{
		SCENEMANAGER->LoadScene("TitleScene");
	}
}
 
void SaveloadScene::Render()
{
	IMAGEMANAGER->FindImage("타이틀백그라운드")->Render(0, 0);

}

