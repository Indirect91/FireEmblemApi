#pragma once
#include "GameObject.h"

class Gold : public GameObject
{
private:
	UINT money; //골드는 돈

public:
	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;

	//▼돈 가져오기
	const UINT &GetMoney() const {return money; }
	//▼돈 세팅하기
	void SetMoney(UINT _money) { money = _money; }
	//▼빠른 돈계산을 위한 참조자
	UINT& RefMoney() { return money; }

	Gold() { money = 10000; }; //초기금액 만원으로 출고, 추후 변동 가능
	~Gold() {};
};

