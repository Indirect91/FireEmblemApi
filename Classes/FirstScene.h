#pragma once
#include "GameNode.h"
class FirstScene : public GameNode
{
private:
	class Tiles* arrField;					//Ÿ�ϵ� ������ �迭. ���忡 ���
	class Player* player;
	class Enemy* enemy;

	Image* bg;
	Image* bg2;


public:
	virtual void Init() override;
	virtual void Release()override;
	virtual void Update()override;
	virtual void Render()override;


	//TODO:���Ϸκ��� Ÿ������ �о����, �������� �� ���Ͽ� ����
	void LoadFromFile();
	void SaveToFile();

	FirstScene();
	~FirstScene() {};
};

