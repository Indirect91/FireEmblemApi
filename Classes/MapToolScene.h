#pragma once
#include "GameNode.h"
#include "Tiles.h"

class MapToolScene : public GameNode
{
private:
	struct moveDir //방향키 눌렸을때
	{
		moveDir() { pos = { 0 }; pressed = false; }
		POINT pos;
		BOOL pressed;
	};

	struct MapToolCursor
	{
		std::string terrain;//타일이 들고있는 지형
		POINT terrainFrame;	//타일위의 지형 프레임렌더용

		std::string objectT; //타일이 들고있는 오브젝트
		POINT objTFrame;		//타일위의 오브젝트 프레임렌더용

		std::string movingT;//타일이 들고있는 움직이는 타일
		POINT movingtFrame;	//움직이는 타일 프레임렌더용

		INT occupation;
		std::string name = "General";
		POINT enemyPos;

	};

	enum class WhichSheet
	{
		MovingTile,
		rock,
		grass,
		object,
		Enemy,
	};

private:
	float moveWindowRateX;	  //윈도우 이동시킬 수치
	float moveWindowRateAccX; //윈도우 이동시킬 수치 가속
	BOOL isMovingDone;		  //윈도우 이동 끝났는지
	void moveWindowFrame();	  //윈도우 움직일 함수. 외부서 호출할 일 없을테니 프라이빗
	UINT paperScrollX;		  //윈도우 이동에 따른 페이퍼 스크롤

	//std::vector<class Tiles> vecField;    //
	float moveAngle;
	class Tiles *field;					//타일들 보유할 포인터
	Tiles toSave[TILECOLX * TILEROWY];
	void moveCamera();
	RECT reLocate(RECT _rc);
	
	moveDir RightLeft;
	moveDir UpDown;




	MapToolCursor maptoolCursor;
	WhichSheet whichSheet;
	std::vector<std::wstring> sheetV;
	RECT exitRc = {};
	RECT changePalletPrev = {1228,167, 1320,207};
	RECT changePalletNext = { 1336,167, 1428,207};
	std::vector<RECT> pallet;

	RECT saveButton = { 1220,610,1460,680 };
	RECT exitButton = {1220,720,1460,780};
	RECT selectArea = { 156,138, 970, 828};
	INT movingTilex = 0;
	INT counter = 0;

public:
	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	MapToolScene();
	~MapToolScene() {};
};

