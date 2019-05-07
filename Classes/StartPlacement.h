#pragma once
#include "GameObject.h"
class StartPlacement : public GameObject
{
public:
	StartPlacement();
	~StartPlacement() {};



	void Release()override;
	void Update()override;
	void Render()override;
	void Init() override;
};

