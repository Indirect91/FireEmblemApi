#pragma once
#include "GameNode.h"

class GameObject : public GameNode
{
public:
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

	//▼캐릭터 드래깅,이동 방향
	typedef enum class DraggingDirection : INT
	{
		IDLE = 0, LEFT = 2, RIGHT, BOTTOM, TOP, LEFTBOTTOM, RIGHTBOTTOM, LEFTTOP, RIGHTTOP,
	} MovingDirection, AttackingDirection;

protected:
	RECT position;  //모든 게임상의 물체는 포지션을 가진다
	POINT index;	//실제 포지션이 아닌 인덱스로 이동하기도 한다
	class Cursor* cursor; //커서는 너무 커서
public:

	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;

	//▼겟터. 참조자로 반환하되 변경되지않게 상수화.
	const RECT & GetPosition()const { return this->position; }
	const POINT& GetIndex()const { return this->index; }

	//▼셋터. 상수화시켜서 변동되지않게 된 참조자로 받아 변수에 세팅해줌.
	void SetPosition(const RECT &_position) { this->position = _position; }
	void SetIndex(const POINT& _index) { this->index = _index; }
	void SetPositionViaIndex() { position = RectMake(index.x*48, index.y* 48,48,48); }
	GameObject();
	~GameObject() {};
};

