#pragma once
#include "GameObject.h"

class Gold : public GameObject
{
private:
	UINT money; //골드는 돈

public:
	void Init() override {}; //이닛할게 없다..
	void Release()override{}; //릴리즈 할게 없다..
	void Update()override {}; //업뎃할게 없다..
	void Render()override {}; //그릴게 없다..

	//▼돈 가져오기
	const UINT &GetMoney() const {return money; }
	//▼돈 세팅하기
	void SetMoney(UINT _money) { money = _money; }
	//▼빠른 돈계산을 위한 참조자
	UINT& RefMoney() { return money; }

	Gold() { money = 10000; }; //초기금액 만원으로 출고, 보통 세이브파일에 의해 덮어씌워짐
	~Gold() {};
};

