#pragma once
#include "GameObject.h"

//▼직업
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

//▼캐릭터 드래깅 방향
enum class DraggingDirection : UINT
{
	LEFT = 2,RIGHT,DOWN,UP
};

class Character : public GameObject
{
public:
	//▼캐릭터 상태
	enum class CharStatus : UINT
	{
		Idle,
		IsActed,
		IsCursorOn,
		IsClicked,
		IsMoving,
		IsAttacking,
		IsDragging,
		IsCheckingOut,
		Disable,
	};

	//▼캐릭터 데이터
	struct CharData
	{
		INT Range = 0;		//행동 범위
		INT Attack = 0;		//공격력
		INT Defence = 0;	//방어력
		INT Health = 0;		//체력
		FLOAT Luck = 0;		//행운, 명중률 및 크리율
		INT Move = 0;		//이동력
	};

	//▼소유주
	enum class OwnedBy
	{
		Enemy = 0,
		Player = 4,
		Ally = 8,
	};

private:

	//▼통상
	OwnedBy whosChar;
	CharStatus charStatus;					//캐릭터 상태
	Occupation occupation;					//직업
	class Item *item = nullptr;				//보유중인 아이템
	Image* frameImg;						//프레임 이미지
	BOOL isInCamera;						//카메라 안에 있어야만 업데이트/랜더됨
	class Cursor* cursor;

	//▼이미지쪽 변수
	Image * portraitImg;		//초상화 이미지
	FLOAT portraitAlpha;		//초상화 알파값
	BOOL isActionTaken;			//모든 캐릭터는 턴당 행동을 가진다
	UINT frameCounter;			//업데이트별 프레임 제어자
	INT frameInterval;			//프레임 도는 빈도
	UINT frameLoop;				//프레임 추가어치
	POINT frame;				//모든 캐릭터는 프레임을 가진다

	//▼스탯데이터쪽 변수
	CharData occupationData;
	CharData additionalData;
	CharData baseData;

	//▼이동관련 변수
	std::set<class Tiles*> AvailableTiles;	//행동가능 타일들 모아둠
	std::set<class Tiles*> purpleTiles;		//적 행동범위
	std::set<class Tiles*> greenTiles;		//힐범위 표시된 타일들 모아둠
	std::set<class Tiles*> blueTiles;		//이동범위 표시된 타일들 모아둠
	std::set<class Tiles*> allyTiles;		//아군 저장 컨테이너
	std::set<class Tiles*> foeTiles;		//상대방 저장 컨테이너
	std::set<class Tiles*> redTiles;		//공격범위 표시된 타일들 모아둠

	//▼거리계산관련
	BOOL isRangeCalculated;						//이동범위 계산 한번 했는지 체크용
	BOOL isMoveCalculated;						//이동범위 계산 한번 했는지 체크용
	INT RangeCalculator;					//이동범위 계산용

	//▼드래깅 관련
	DraggingDirection draggindDirection;	//드래깅 방향
	std::set<class Tiles*> toMove;		//이동 화살표 표시용
	POINT draggingIndex;					//드래깅 위치
	UINT dragValidity;						//이동 가능한지

private:
	BOOL CheckInCamera();										//카메라 속에 캐릭이 있는지 체크
	void AdjustFrame();



public:
	void Release()override;
	void Render()override;
	void Update()override;
	void Init() override;

	void SetInitialChar(Occupation _job, std::string _charName, POINT _index, OwnedBy _whos);
	void SetActionTaken(BOOL _actionTaken) { this->isActionTaken = _actionTaken; }
	void SetOccupation(Occupation _job);
	void SetFrame(POINT _frame) { this->frame = _frame; }
	void SetFrameAuto(Occupation _job, OwnedBy _whos);
	void SetImg(std::string _CharName);

	void SetAdditionalAttack(INT _attack) { this->additionalData.Attack = _attack; }
	void SetAdditionalDefence(INT _defence) { this->additionalData.Defence = _defence; }
	void SetAdditionalLuck(FLOAT _luck) { this->additionalData.Luck = _luck; }
	void SetAdditionalMove(INT _move) { this->additionalData.Move = _move; }

	const BOOL & GetIsActionTaken() const { return this->isActionTaken; }
	const Occupation &GetOccupation() const { return this->occupation; }
	const POINT &GetFrame() const { return this->frame; }
	const BOOL GetIsClicked() const { return charStatus == CharStatus::IsClicked; }
	const CharStatus& GetStatus() const { return charStatus; }

	//▼범위표시요청관련
	void ShowMoveRange(void (Character::* colourShow)(INT));	//함수포인터로 어디를 보여줄지 확정지음
	void DisableMoveRange();									//본인 주변 범위 끄기
	void ShowActionRange();
	void DisableActionRange();
	void BlueRedShow(INT _actionRange);
	void PurpleShow(INT _actionRange);

	Character();
	Character(Occupation _job, std::string _charName, POINT _index, OwnedBy _whos);
	~Character() {};
};

