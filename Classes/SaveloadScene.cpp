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
	//���Ҵ� ���ذ��� ������ ����
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
	IMAGEMANAGER->FindImage("Ÿ��Ʋ��׶���")->Render(0, 0);

}

