#pragma once
#include "GameObject.h"
class Cursor : public GameObject
{
private:
	UINT cursorFrame;						//커서 프레임
	UINT cursorCounter;						//커서 프레임올릴 카운터
	BOOL isCursorVisible;					//커서가 보이는지

	void CursorFrameManage();				//커서 프레임 돌릴 함수 
	std::map <std::string, GameObject*>& clippedTiles = DATACENTRE.RefObjects(ObjType::ClippedTile);

public:
	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;


	UINT &GetCursorFrame() { return cursorFrame; }
	BOOL &GetIsCursorVisible() { return isCursorVisible; }
	
	void SetCursorVisibility(BOOL _isCursorVisible) { isCursorVisible = _isCursorVisible; }

	Cursor();
	~Cursor() {};
};

