#pragma once
#include "GameNode.h"
class MapToolScene : public GameNode
{
private:
	



public:
	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	MapToolScene();
	~MapToolScene();
};

