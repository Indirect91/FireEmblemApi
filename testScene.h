#pragma once
#include "GameObject.h"
class testScene : public GameObject
{
public:

	std::vector<RECT> testVec;
	testScene();
	~testScene();

	void Init();
	void Release();
	void Update();
	void Render();
};

