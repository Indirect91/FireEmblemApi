#pragma once
#include "GameNode.h"
class WorldScene : public GameNode
{
private:


public:
	virtual void Init() override;
	virtual void Release()override;
	virtual void Update()override;
	virtual void Render()override;


	WorldScene();
	~WorldScene() {};
};

