#pragma once
#include "GameObject.h"
class testScene : public GameObject
{
public:

	float testAngle[25*25];
	float testAdditional;
	float original;
	float line;
	float cherryX = 0;
	int cherryLoc;
	std::vector<RECT> testVec;
	bool cherryVisible;

	enum class state
	{
		up,down,idle
	};
	state stat = state::idle;


	testScene();
	~testScene();
	

	void Init();
	void Release();
	void Update();
	void Render();
};

