#pragma once
#include "Lords.h"
class Enemy : public Lords
{
private:

public:

	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;

	Enemy();
	~Enemy() {};
};

