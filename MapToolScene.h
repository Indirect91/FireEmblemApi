#pragma once
#include "GameNode.h"

class MapToolScene : public GameNode
{

private:
	float moveWindowRateX;	  //윈도우 이동시킬 수치
	float moveWindowRateAccX; //윈도우 이동시킬 수치 가속
	BOOL isMovingDone;		  //윈도우 이동 끝났는지
	void moveWindowFrame();	  //윈도우 움직일 함수. 외부서 호출할 일 없을테니 프라이빗
	UINT paperScrollX;		  //윈도우 이동에 따른 페이퍼 스크롤

	UINT fieldX_Column = 30;				//타일들 가로 갯수
	UINT fieldY_Row = 30;					//타일들 세로 갯수
	UINT tileSize = 32;						//타일 가로세로
	//std::vector<class Tiles> vecField;    //
	class Tiles *arrField;					//타일들 보유할 배열. 저장에 사용
	RECT showArea;							//타일들 보여줄 영역(카메라)
	RECT tempC;
	void moveCamera();

public:
	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	MapToolScene() {};
	~MapToolScene() {};
};

