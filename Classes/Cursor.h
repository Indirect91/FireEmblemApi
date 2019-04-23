#pragma once
#include "GameObject.h"
class Cursor : public GameObject
{
private:
	UINT cursorFrame;						//Ŀ�� ������
	UINT cursorCounter;						//Ŀ�� �����ӿø� ī����
	BOOL isCursorVisible;					//Ŀ���� ���̴���

	void CursorFrameManage();				//Ŀ�� ������ ���� �Լ� 
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

