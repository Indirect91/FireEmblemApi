#pragma once
#include "Image.h"
constexpr UINT TILESIZE = 48; //타일 하나의 사이즈
constexpr UINT TILEROWY = 20; //타일 Y 갯수
constexpr auto TILECOLX = 32; //타일 X 갯수, constexpr 자동추론 테스트

class GameNode
{

protected:
	RECT tempC = { 0 };	//게임노드로부터 파생된 모든 씬은 충돌용 렉트를 가진다

	//▼인게임 상태
	enum class IngameStatus : UINT
	{
		ESCMenu,
		GameOver,
		EnemyTurn,
		PlayerWon,
		PlayerTurn,
		SelectionUI,
		TurnChanging,
		StartPlacement,
		ExecutingBattle,
	};

	//▼무기 타입
	enum class WeaponType
	{
		Bow,				//활
		Axe,				//도끼
		Tomb,				//마법서
		Sword,				//칼
		Lance,				//창
		Staff,				//지팡이
	};



public:
	GameNode() {};
	virtual ~GameNode() {}; //가상 소멸자를 통해, 자식의 소멸자가 씹히는걸 방지한다

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0; 
	virtual void Release() = 0;
};

