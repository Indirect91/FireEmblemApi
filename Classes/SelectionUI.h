#pragma once
#include "GameObject.h"
class SelectionUI : public GameObject
{
private:
	RECT temp;
public:

	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;


	SelectionUI();
	~SelectionUI() {};
};

