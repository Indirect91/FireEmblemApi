#pragma once
#include "BattleScenes.h"
class Battle01 : public BattleScenes
{
private:

	Image* bg = nullptr;
	Image* bg2 = nullptr;


public:
	virtual void Init() override;
	virtual void Release()override;
	virtual void Update()override;
	virtual void Render()override;


	//TODO:파일로부터 타일정보 읽어오기, 전투종료 후 파일에 쓰기
	void LoadFromFile();
	void SaveToFile();

	Battle01();
	~Battle01() {};
};

