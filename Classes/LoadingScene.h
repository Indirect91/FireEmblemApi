#pragma once
#include "GameNode.h"
#include "Loading.h"

class LoadingScene : public GameNode
{
private:
	Loading* _loading; //로딩씬에선 로딩 하나 이닛해서 쓸거기에 포인터 보유
	float per;		   //퍼센테이지

public:
	void Init();
	void Release();
	void Update();
	void Render();

	
	void LoadingImage(); //로딩할 이미지 넣어두는 곳
	void LoadingSound(); //사운드 로드하는 곳
	void LoadScenes();	 //씬들 모아서 로딩하는곳

	LoadingScene() {}
	~LoadingScene() {}
};

