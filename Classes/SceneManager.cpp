#include "../stdafx.h"
#include "SceneManager.h"

SceneManager::SceneManager()
{
	IMAGEMANAGER->AddImage("Ŀ��", L"IMAGE/ETC/cover.png");
	coverAlpha = 1;
	coverStatue = tagCoverStatue::FadeIn;
	coverSpeed = 0.02f;
	_currentScene = nullptr;
}

SceneManager::~SceneManager()
{
	miSceneList iter = _mSceneList.begin();
	for (; iter != _mSceneList.end();)
	{
		if (iter->second != NULL)
		{
			if (iter->second == _currentScene) iter->second->Release();
			SAFE_DELETE(iter->second);
			iter = _mSceneList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	_mSceneList.clear();
}

void SceneManager::Update(void)
{
	//������� ����� ������Ʈ
	if (_currentScene) _currentScene->Update();

	
	//Ŀư�� ���¿� ���� �ൿ
	switch (coverStatue)
	{
	case tagCoverStatue::Idle : //Ŀư ��������
		break;

	case tagCoverStatue::FadeIn: //���� ������ ���������
		if (coverAlpha >= 0.f) coverAlpha -= coverSpeed;
		else if (coverAlpha <= 0.f)
		{
			coverAlpha = 0.f;
			coverStatue = tagCoverStatue::Idle;
		}
		break;

	case tagCoverStatue::FadeOut: //�Ȱ��� �帮��� ��ο�������
		if (coverAlpha <= 1.f) coverAlpha += coverSpeed;
		else if (coverAlpha >= 1.f)
		{
			coverAlpha = 1.f;
			coverStatue = tagCoverStatue::Idle;
		}
		break;
	}
	//������ ���İ����� ����
	IMAGEMANAGER->FindImage("Ŀ��")->SetAlpha(coverAlpha);
}

void SceneManager::Render(void)
{
	//������� �����ϸ� �ش��ϴ� ������� �����ض�
	if (_currentScene) _currentScene->Render();
	IMAGEMANAGER->FindImage("Ŀ��")->Render(0, 0);
}

GameNode * SceneManager::AddScene(std::string sceneName, GameNode * scene)
{
	//���� ������ �׳� �� ����
	if (!scene) return NULL;

	//���� ������ �ʿ� ���
	_mSceneList.insert(make_pair(sceneName, scene));

	return scene;
}

HRESULT SceneManager::LoadScene(std::string sceneName)
{
	miSceneList find = _mSceneList.find(sceneName);

	//��ã�Ҵٸ� E_FAIL
	if (find == _mSceneList.end()) return E_FAIL;

	//�ٲٷ��� ���� ������� ���ٸ� E_FAIL
	if (find->second == _currentScene) return E_FAIL;

	//������� ������ ������ ������ ���� ����ó�� ����
	find->second->Init();
	{
		SetWindowPos(_hWnd, 0, WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY, SWP_NOMOVE);
		_currentScene = find->second;
		return S_OK;
	}

	return E_FAIL;
}
