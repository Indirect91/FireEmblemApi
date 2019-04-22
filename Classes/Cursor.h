#pragma once
#include "GameObject.h"
class Cursor : public GameObject
{
private:
	UINT cursorFrame;						//Ŀ�� ������
	UINT cursorCounter;						//Ŀ�� �����ӿø� ī����
	BOOL isCursorVisible;

	void CursorFrameManage();				//Ŀ�� ������ ���� �Լ� 

public:
	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;


	UINT &GetCursorFrame() { return cursorFrame; }
	BOOL &GetIsCursorVisible() { return isCursorVisible; }
	
	void SetCursorVisibility(FLOAT _isCursorVisible) { isCursorVisible = _isCursorVisible; }

	Cursor();
	~Cursor() {};
};

