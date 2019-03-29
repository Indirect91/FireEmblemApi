#pragma once
#include "GameNode.h"
class TitleScene : public GameNode
{

public:

	void Init();
	void Release();
	void Update();
	void Render();

	TitleScene() {};
	~TitleScene() {};
};

