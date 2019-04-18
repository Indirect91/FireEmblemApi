#pragma once
#include "GameObject.h"

class Item : public GameObject
{
private:


public:

	void Init();
	void Update();
	void Release();
	void Render();

	Item();
	~Item() {};
};

