#pragma once
#include "GameObject.h"
class Character : public GameObject
{
private:
	Image * portaitImg; //모든 캐릭터는 초상화 이미지를 가진다
	Image* frameImg;	//모든 캐릭터는 프레임 이미지를 가진다
	BOOL isActionTaken;	//모든 캐릭터는 턴당 행동을 가진다

	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;
	  
public:
	Character();
	~Character() {};
};

