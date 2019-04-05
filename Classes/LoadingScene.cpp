#include "../stdafx.h"
#include "LoadingScene.h"
#include "TitleScene.h"
#include "MapToolScene.h"
#include "testScene.h"

void LoadingScene::Init()
{
	//��ε�Ŭ���� ���Ҵ� ���ְ� �ʱ�ȭ
	_loading = new Loading;
	_loading->Init();

	//���̹��� ���� �� �ε�
	this->LoadingImage();
	this->LoadingSound();
	this->LoadScenes();

}

void LoadingScene::Release()
{
	//�ε�Ŭ���� ���Ҵ� ������ ������ ����
	_loading->Release();
	SAFE_DELETE(_loading);
}

//�������Ʈ �ѹ��� �ϳ��� ������ �ε���. �ε��� �����̴� �̹��� �߰����൵ �ǰ�, �ε����� ���൵ �ǰ�.
void LoadingScene::Update()
{
	_loading->Update(); //�ε�Ŭ���� �ֵ� ������Ʈ

	if (_loading->LoadingDone()) //���� ���ǹ��� Ȯ���ϴ� ���ÿ� �� �Ʒ� �ִ� �Լ������ �����
	{
		//��ε��� ������ �̵��� ��
		SCENEMANAGER->LoadScene("TitleScene");
	}
}

//���ۼ������� ȭ�鿡 �׷���. 
void LoadingScene::Render()
{
	float num = _loading->GetLoadItem().size();
	float cur = _loading->GetCurrentGauge();
	per = cur / num * 100;

	std::wstring str = std::to_wstring(per);
	str += L" %";
	D2DRENDERER->RenderText(WINSIZEX/2, WINSIZEY/2, (std::wstring)str, str.size(), D2DRenderer::DefaultBrush::White);

}

//����� ����ϴ� �̹��� �ε����
void LoadingScene::LoadingImage()
{
	//��������, �Ϲ� �̹��� �ε� ����
	//_loading->LoadFrameImage("NPC-����", L"image/NPC/NPC-���� �Ҿƹ���_12��.bmp",12, 1);
	//_loading->LoadImg("����Ʈ-��ǳ��", L"image/NPC/����Ʈ - ����.bmp");

	//�������
	_loading->LoadImg("MapToolBg1", L"IMAGE/MapToolScene/MapToolBg1.png");
	_loading->LoadImg("MapToolBg2", L"IMAGE/MapToolScene/MapToolBg2.png");
	_loading->LoadImg("MapToolTitle", L"IMAGE/MapToolScene/MaptoolTitle.png");
	_loading->LoadImg("MapToolMapbox", L"IMAGE/MapToolScene/MaptoolMapbox.png");
	_loading->LoadImg("MapToolMat2", L"IMAGE/MapToolScene/MaptoolMat2.png");
	_loading->LoadImg("oldpaper", L"IMAGE/MapToolScene/oldpaper.png");
	
	//��Ÿ��Ʋ��
	_loading->LoadImg("Ÿ��Ʋ��׶���", L"IMAGE/TitleScene/TitleBackground.png");
	_loading->LoadImg("Ÿ��Ʋ���ùڽ�", L"IMAGE/TitleScene/TitlePick.png");
	_loading->LoadImg("Ÿ��Ʋ�ؽ�Ʈ", L"IMAGE/TitleScene/TitleLogo.png");
	_loading->LoadFrameImage("Ÿ��Ʋ�Ƚÿ�", L"IMAGE/TitleScene/TitleFalchion.png", 34, 1);



	//���۵��׽�Ʈ
	for (int i = 0; i < 100; i++)
	{
		_loading->LoadImg("oldpaper", L"IMAGE/MapToolScene/oldpaper.png");
	}
}

//����� ����ϴ� �ε����
void LoadingScene::LoadingSound()
{
	//���׶��� ���� �ε� ����
	//_loading->LoadSound("�ó׸�ƽ", "SOUND/Intro_Cinematic.ogg", true, true);

	//��ȿ���� �ε� ����
	//_loading->LoadSound("����", "SOUND/villager_save.ogg", false, false);



	//�������
	_loading->LoadSound("����BGM", "SOUND/FE/MapTool_Road Taken.mp3", true, true);
	//��Ÿ��Ʋ��
	_loading->LoadSound("Ÿ��ƲBGM", "SOUND/FE/TitleBgm.mp3", true, true);
}

//��ε����
void LoadingScene::LoadScenes()
{
	_loading->LoadScene("TitleScene",new TitleScene);
	_loading->LoadScene("MapToolScene", new MapToolScene);
	_loading->LoadScene("test", new testScene);
}
