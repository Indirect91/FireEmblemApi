#pragma once
#include "Lords.h"
class Enemy : public Lords
{
private:
	class Cursor* cursor;		//커서
	std::map<std::string, GameObject*>& enemyTroop = DATACENTRE.RefObjects(ObjType::EnemyArmy); //적 부대

public:

	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;

	Enemy();
	~Enemy() {};
};

