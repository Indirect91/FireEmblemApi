#pragma once
#include "GameNode.h"
class FirstScene : public GameNode
{
private:
	class Tiles* arrField;					//타일들 보유할 배열. 저장에 사용
	RECT chrome;
	RECT virion;
//	Player* player;



public:
	virtual void Init() override;
	virtual void Release()override;
	virtual void Update()override;
	virtual void Render()override;


	//TODO:파일로부터 타일정보 읽어오기, 전투종료 후 파일에 쓰기
	void LoadFromFile();
	void SaveToFile();

	FirstScene();
	~FirstScene() {};
};

