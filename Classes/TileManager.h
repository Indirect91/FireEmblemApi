#pragma once
#include "GameObject.h"

class TileManager : public GameObject
{
private:
	
	class Tiles* field = nullptr;		//��¥ Ÿ�ϵ� ��� �迭
	class Cursor* cursor = nullptr;		//���� Ŀ��

	std::vector <Tiles*> clippedTiles;	//ī�޶� �ӿ��� �ִ� Ÿ�ϵ�
	POINT clippedIndex = { 0 };			//���� ������Ʈ�� Ŭ���� ������ �ε���
	UINT clippedX = 0;					//���� ������Ʈ���� ī�޶� �ӿ� ���� Ÿ��X����
	UINT clippedY = 0;					//���� ������Ʈ�� ī�޶� �ӿ� ���� Y����
	void ClipTiles();					//���������� ī�޶� ���� Ŭ���� ���ִ� �Լ�

public:

	void Init();
	void Update();
	void Release();
	void Render();

	const Tiles* GetFieldPtr() const { return field; }
	std::vector <Tiles*> & GetClippedTiles() { return clippedTiles; }

	TileManager();
	~TileManager() {};
};

