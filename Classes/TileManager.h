#pragma once
#include "GameObject.h"

class TileManager : public GameObject
{
private:
	
	class Tiles* field; //진짜 타일들 담길 배열
	class Cursor* cursor; //현재 커서

public:

	void Init();
	void Update();
	void Release();
	void Render();

	const Tiles* getFieldPtr() const { return field; }

	TileManager();
	~TileManager() {};
};

