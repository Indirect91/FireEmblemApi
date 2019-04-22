#pragma once
#include "GameNode.h"

class MapToolScene : public GameNode
{
private:
	struct moveDir //방향키 눌렸을때
	{
		POINT pos;
		BOOL pressed;
	};
private:
	float moveWindowRateX;	  //윈도우 이동시킬 수치
	float moveWindowRateAccX; //윈도우 이동시킬 수치 가속
	BOOL isMovingDone;		  //윈도우 이동 끝났는지
	void moveWindowFrame();	  //윈도우 움직일 함수. 외부서 호출할 일 없을테니 프라이빗
	UINT paperScrollX;		  //윈도우 이동에 따른 페이퍼 스크롤

	//std::vector<class Tiles> vecField;    //
	float moveAngle;
	class Tiles *field;					//타일들 보유할 배열. 저장에 사용
	RECT tempC;								//충돌에 사용할 임시 렉트 초기화
	void moveCamera();
	RECT reLocate(RECT _rc);
	RECT palletes[9];
	
	moveDir RightLeft;
	moveDir UpDown;

public:
	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	MapToolScene() {};
	~MapToolScene() {};
};

