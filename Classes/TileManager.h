#pragma once
#include "GameObject.h"

class TileManager : public GameObject
{
private:
	
	class Tiles* field = nullptr;		//진짜 타일들 담길 배열

	std::map <std::string, GameObject*> &clippedTiles = DATACENTRE.RefObjects(ObjType::ClippedTile) ;	//카메라 속에만 있는 타일들
	POINT clippedIndex = { 0 };			//현재 업데이트서 클리핑 시작할 인덱스
	UINT clippedX = 0;					//현재 업데이트에서 카메라 속에 잡힌 타일X갯수
	UINT clippedY = 0;					//현재 업데이트서 카메라 속에 잡힌 Y갯수
	void ClipTiles();					//내부적으로 카메라에 대해 클리핑 해주는 함수
	void UpdateClippedTiles();			//클리핑된 타일들만 업데이트 돌림
public:

	void Init();
	void Update();
	void Release();
	void Render();

	TileManager();
	~TileManager() {};
};

