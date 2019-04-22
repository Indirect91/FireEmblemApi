#pragma once
#include "GameObject.h"

class TileManager : public GameObject
{
private:
	
	class Tiles* field; //��¥ Ÿ�ϵ� ��� �迭
	class Cursor* cursor; //���� Ŀ��

public:

	void Init();
	void Update();
	void Release();
	void Render();

	const Tiles* getFieldPtr() const { return field; }

	TileManager();
	~TileManager() {};
};

