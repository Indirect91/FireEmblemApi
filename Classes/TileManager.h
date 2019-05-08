#pragma once
#include "GameObject.h"
#include "Tiles.h"

class TileManager : public GameObject
{
private:
	
	class Tiles* field = nullptr;		//��¥ Ÿ�ϵ� ��� �迭
	Tiles toLoad[TILECOLX * TILEROWY];
	std::map <std::string, GameObject*> &clippedTiles = DATACENTRE.RefObjects(ObjType::ClippedTile) ;	//ī�޶� �ӿ��� �ִ� Ÿ�ϵ�
	POINT clippedIndex = { 0 };			//���� ������Ʈ�� Ŭ���� ������ �ε���
	UINT clippedX = 0;					//���� ������Ʈ���� ī�޶� �ӿ� ���� Ÿ��X����
	UINT clippedY = 0;					//���� ������Ʈ�� ī�޶� �ӿ� ���� Y����
	void ClipTiles();					//���������� ī�޶� ���� Ŭ���� ���ִ� �Լ�
	void UpdateClippedTiles();			//Ŭ���ε� Ÿ�ϵ鸸 ������Ʈ ����

	INT movingTilex = 0;				//�����ӷ�����
	INT counter = 0;					//�����ӷ�����2
public:

	void Init();
	void Update();
	void Release();
	void Render();

	TileManager();
	~TileManager() {};
};

