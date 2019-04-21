#pragma once
#include "GameObject.h"
class TurnManager : public GameObject
{
public:

	void Init();
	void Update();
	void Release();
	void Render();




	TurnManager();
	~TurnManager() {};
};

