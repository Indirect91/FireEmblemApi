#pragma once
#include "GameNode.h"
#include "Loading.h"

class LoadingScene : public GameNode
{
private:
	Loading* _loading; //�ε������� �ε� �ϳ� �̴��ؼ� ���ű⿡ ������ ����
	float per;		   //�ۼ�������

public:
	void Init();
	void Release();
	void Update();
	void Render();

	
	void LoadingImage(); //�ε��� �̹��� �־�δ� ��
	void LoadingSound(); //���� �ε��ϴ� ��
	void LoadScenes();	 //���� ��Ƽ� �ε��ϴ°�

	LoadingScene() {}
	~LoadingScene() {}
};

