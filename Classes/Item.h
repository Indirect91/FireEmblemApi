#pragma once
#include "GameObject.h"
class Item : public GameObject
{
private:
	UINT gold;


public:

	void Init();
	void Update();
	void Release();
	void Render();

	Item();
	~Item() {};
};

