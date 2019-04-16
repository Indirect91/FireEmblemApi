#pragma once
#include "GameNode.h"
class SaveloadScene : public GameNode
{
	POINT SaveSlots;
	POINT SelectionBox;

public:
	void Init()override;
	void Release()override;
	void Update() override;
	void Render()override;

	SaveloadScene();
	~SaveloadScene();
};

