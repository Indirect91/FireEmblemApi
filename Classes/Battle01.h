#pragma once
#include "BattleScenes.h"
class Battle01 : public BattleScenes
{
private:

	BOOL gameoverInit = false;
	BOOL gameEndReady = false;
	INT gameOverPhase = 0;


public:

	virtual void Init() override;
	virtual void Release()override;
	virtual void Update()override;
	virtual void Render()override;


	
	Battle01();
	~Battle01() {};
};

