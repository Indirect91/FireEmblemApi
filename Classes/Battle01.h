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

	Battle01();
	~Battle01() {};
};

