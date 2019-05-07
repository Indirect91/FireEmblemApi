#pragma once
#include "GameObject.h"
class TurnManager : public GameObject
{
private:
	enum class ChangingStatus
	{
		Idle,

		toPlayer,
		toEnemy,
	};

	struct Rings
	{
		POINT RingPosition = { 0,0 };
	};

	struct TurnChangeImgs
	{
		POINT rings = {0,200};
		POINT texts = {0,WINSIZEY / 2 };
		POINT back = {0,WINSIZEY/2};
		INT phase = 0;
	};

	
	INT moveSpeedFast = 20;
	INT moveSpeedMid = 15;
	INT moveSpeedSlow = 5;
	ChangingStatus changingStatus; //������ �ٲ�� �ִ���
	TurnChangeImgs toPlayer;
	TurnChangeImgs toEnemy;

	void Revert();

public:

	void Init();
	void Update();
	void Release();
	void Render();

	void changeToPlayer();
	void changeToEnemy();


	TurnManager();
	~TurnManager() {};
};

