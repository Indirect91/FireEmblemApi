#pragma once
#include "GameObject.h"

//▼군주, 지배자 클래스. 캐릭터 개개인이 아닌 군단을 지휘하는 플레이어들
class Lords : public GameObject
{
protected: //군주로부터 파생된 자식들만 부대를 가진다

	BOOL turnFlag; //모든 군주들은 본인의 턴을 가진다.

public:

	void SetTurnEnd() { turnFlag = false; }
	void SetTurnStart() { turnFlag = true; }
	void SetTurnStatue(BOOL _turn) { turnFlag = _turn; }
	
	const BOOL &GetTurnStatus() const { return turnFlag; }
	
	Lords();
	~Lords() {};
};

