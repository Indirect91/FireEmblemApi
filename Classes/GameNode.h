#pragma once
#include "Image.h"
class GameNode
{

protected:
	RECT tempC = { 0 };	//게임노드로부터 파생된 모든 씬은 충돌용 렉트를 가진다

	//▼인게임 상태
	enum class IngameStatus : UINT
	{
		ESCMenu,
		EnemyTurn,
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

