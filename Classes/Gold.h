#pragma once
#include "GameObject.h"
class Gold : public GameObject
{
private:
	UINT money; //°ñµå´Â µ·

public:
	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;

	Gold();
	~Gold() {};
};

