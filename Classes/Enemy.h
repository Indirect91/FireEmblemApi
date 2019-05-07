#pragma once
#include "Lords.h"
class Enemy : public Lords
{
private:
	std::map<std::string, GameObject*>& enemyTroop = DATACENTRE.RefObjects(ObjType::EnemyArmy); //Àû ºÎ´ë

public:

	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;

	Enemy();
	~Enemy() {};
};

