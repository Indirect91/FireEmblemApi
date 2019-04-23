#include "../stdafx.h"
#include "SceneManager.h"

SceneManager::SceneManager()
{
	IMAGEMANAGER->AddImage("커버", L"IMAGE/ETC/cover.png");
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
	//현재씬이 존재시 업데이트
	if (_currentScene) _currentScene->Update();

	
	//커튼의 상태에 따른 행동
	switch (coverStatue)
	{
	case tagCoverStatue::Idle : //커튼 변동없음
		break;

	case tagCoverStatue::FadeIn: //막이 걷히며 밝아지는중
		if (coverAlpha >= 0.f) coverAlpha -= coverSpeed;
		else if (coverAlpha <= 0.f)
		{
			coverAlpha = 0.f;
			coverStatue = tagCoverStatue::Idle;
		}
		break;

	case tagCoverStatue::FadeOut: //안개가 드리우며 어두워지는중
		if (coverAlpha <= 1.f) coverAlpha += coverSpeed;
		else if (coverAlpha >= 1.f)
		{
			coverAlpha = 1.f;
			coverStatue = tagCoverStatue::Idle;
		}
		break;
	}
	//변동된 알파값으로 세팅
	IMAGEMANAGER->FindImage("커버")->SetAlpha(coverAlpha);
}

void SceneManager::Render(void)
{
	//현재씬이 존재하면 해당하는 현재씬을 렌더해라
	if (_currentScene) _currentScene->Render();
	IMAGEMANAGER->FindImage("커버")->Render(0, 0);
}

GameNode * SceneManager::AddScene(std::string sceneName, GameNode * scene)
{
	//씬이 없으면 그냥 널 리턴
	if (!scene) return NULL;

	//씬이 있으면 맵에 담기
	_mSceneList.insert(make_pair(sceneName, scene));

	return scene;
}

HRESULT SceneManager::LoadScene(std::string sceneName)
{
	miSceneList find = _mSceneList.find(sceneName);

	//못찾았다면 E_FAIL
	if (find == _mSceneList.end()) return E_FAIL;

	//바꾸려는 씬과 현재씬이 같다면 E_FAIL
	if (find->second == _currentScene) return E_FAIL;

	//여기까지 왔으면 문제가 없으니 씬을 변경처리 하자
	find->second->Init();
	{
		SetWindowPos(_hWnd, 0, WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY, SWP_NOMOVE);
		_currentScene = find->second;
		return S_OK;
	}

	return E_FAIL;
}
