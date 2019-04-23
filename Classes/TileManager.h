#pragma once
#include "GameObject.h"

class TileManager : public GameObject
{
private:
	
	class Tiles* field = nullptr;		//��¥ Ÿ�ϵ� ��� �迭

	std::map <std::string, GameObject*> &clippedTiles = DATACENTRE.RefObjects(ObjType::ClippedTile) ;	//ī�޶� �ӿ��� �ִ� Ÿ�ϵ�
	POINT clippedIndex = { 0 };			//���� ������Ʈ�� Ŭ���� ������ �ε���
	UINT clippedX = 0;					//���� ������Ʈ���� ī�޶� �ӿ� ���� Ÿ��X����
	UINT clippedY = 0;					//���� ������Ʈ�� ī�޶� �ӿ� ���� Y����
	void ClipTiles();					//���������� ī�޶� ���� Ŭ���� ���ִ� �Լ�
	void UpdateClippedTiles();			//Ŭ���ε� Ÿ�ϵ鸸 ������Ʈ ����
public:

	void Init();
	void Update();
	void Release();
	void Render();

	TileManager();
	~TileManager() {};
};

