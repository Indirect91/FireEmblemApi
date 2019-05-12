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

	struct EnemyData
	{
		INT occupation;
		POINT realPos;
	};

	struct MapToolCursor
	{
		std::string terrain;//타일이 들고있는 지형
		POINT terrainFrame;	//타일위의 지형 프레임렌더용

		std::string objectT; //타일이 들고있는 오브젝트
		POINT objTFrame;		//타일위의 오브젝트 프레임렌더용

		std::string movingT;//타일이 들고있는 움직이는 타일
		POINT movingtFrame;	//움직이는 타일 프레임렌더용

		std::string EnemyName = "";
		INT occupation = 0;
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

	enum class Occupation : UINT
	{
		Assassin = 0,		//단검				--						상급	A
		Swordsman = 10,		//장검				--						상급	A
		Myrmidon = 32,		//휜검				상위:SwordMaster			중급	B				
		Mercenary = 42,		//방패+검			상위:Hero				중급	B
		SwordMaster = 52,	//쌍검				--						상급	A
		Archer = 62,		//모자 궁수			상위:Sniper				하급	C
		Sniper = 72,		//큰활 궁수			상위:BowNight			중급	B
		GreatLord = 82,		//군주				--						최상	S
		Knight = 92,		//중장갑				상위:General(미구현)		중급	B
		WarCleric = 102,	//줄무니옷 힐러		--						상급	A
		Hero = 114,			//큰방패 + 검			--						상급	A
		Sage = 124,			//로브				--						상급	A
		Mage = 136,			//모자 + 로브			상위:Sage				중급	B
		Cleric = 146,		//일반 힐러			상위:War Cleric			중급	B
		Grandmaster = 158,	//백야전술가			--						상급	A
		Tactician = 168,	//전술가				상위:GrandMaster			중급	B
		BowKnight = 178,	//말탄 궁수			--						상급	A
		DarkKnight = 188,	//검 기사			--						상급	A
		GreatKnight = 198,	//도끼 기사			--						상급	A
		Cavalier = 208,		//창 기사			상위:Paladin				중급	B
		Paladin = 218,		//창 기사 상위		--						상급	A
		Troubadour = 228,	//기마힐러			상위:Valkyrie			중급	B
		Valkyrie = 240,		//기마힐러상위		--						상급	A
		Dancer = 252,		//무희				독립						최상	S
		DarkMage = 263,		//암흑마법			상위:Sorcerer			중급	B
		Sorcerer = 273,		//암흑마법상위		--						상급	A
		Theif = 283,		//도둑				상위:Assassin			하급	C
	};

private:
	float moveWindowRateX;	  //윈도우 이동시킬 수치
	float moveWindowRateAccX; //윈도우 이동시킬 수치 가속
	BOOL isMovingDone;		  //윈도우 이동 끝났는지
	void moveWindowFrame();	  //윈도우 움직일 함수. 외부서 호출할 일 없을테니 프라이빗
	UINT paperScrollX;		  //윈도우 이동에 따른 페이퍼 스크롤

	float moveAngle;
	class Tiles *field;					//타일들 보유할 포인터
	Tiles toSave[TILECOLX * TILEROWY];
	EnemyData toSaveEnemy[100];
	std::vector<EnemyData> EnemiesV;
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
	INT enemyX = 0;


public:
	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	MapToolScene();
	~MapToolScene() {};
};

