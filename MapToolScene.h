#pragma once
#include "GameNode.h"
class MapToolScene : public GameNode
{
private:
	float moveWindowRateX;	  //윈도우 이동시킬 수치
	float moveWindowRateAccX; //윈도우 이동시킬 수치 가속
	BOOL isMovingDone;		  //윈도우 이동 끝났는지
	void moveWindowFrame();	  //윈도우 움직일 함수. 외부서 호출할 일 없을테니 프라이빗




public:
	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	MapToolScene() {};
	~MapToolScene() {};
};

