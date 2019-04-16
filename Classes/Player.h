#pragma once
#include "GameObject.h"
class Player final : public GameObject //플레이어 더 상속받지 못하게 파이널 박아넣음
{
private:

	//플레이어가 존재한다면 있어야 할 것?
	
	//플레이어가 보유중인 캐릭터 목록
	//플레이어 인벤토리
	//보유 골드
	//스토리 진행상황

public:

	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;

	Player() {};
	~Player() {};
};

